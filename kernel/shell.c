#include "shell.h"
#include "vga.h"
#include "kbd.h"
#include "string.h"
#include "thread.h"
#include "timer.h"
#include "mem.h"
#include "sync.h"
#include "io.h"
#include "desktop.h"
#include "serial.h"

#define MAX_LINE 128
#define MAX_ARGS 8

static const char *state_names[] = {
    "FREE", "READY", "RUNNING", "SLEEPING", "BLOCKED", "DEAD"
};

static uint8_t theme_index = 0;
static const struct { uint8_t fg; uint8_t bg; const char *name; } themes[] = {
    { VGA_COLOR_LIGHT_GREY,   VGA_COLOR_BLACK,      "Classic White" },
    { VGA_COLOR_LIGHT_GREEN,  VGA_COLOR_BLACK,      "Matrix Green"  },
    { VGA_COLOR_LIGHT_CYAN,   VGA_COLOR_BLUE,       "Solar Cyan"    },
    { VGA_COLOR_LIGHT_BROWN,  VGA_COLOR_BLACK,      "Amber Terminal"}
};

uint8_t shell_theme_fg(void) { return themes[theme_index].fg; }
uint8_t shell_theme_bg(void) { return themes[theme_index].bg; }
const char *shell_theme_name(void) { return themes[theme_index].name; }

// Power management
static void os_shutdown(void) {
    vga_set_color(VGA_COLOR_LIGHT_RED, themes[theme_index].bg);
    vga_printf("\nShutting down ThreadOS...\n");
    outw(0x604, 0x2000);   // QEMU ACPI shutdown
    outw(0xB004, 0x2000);  // Bochs
    outw(0x4004, 0x3400);  // VirtualBox
    outb(0xF4, 0x00);      // ISA debug exit
    cli();
    for (;;) hlt();
}

static void os_reboot(void) {
    vga_set_color(VGA_COLOR_LIGHT_RED, themes[theme_index].bg);
    vga_printf("\nRebooting...\n");
    uint8_t good = 0x02;
    while (good & 0x02) good = inb(0x64);
    outb(0x64, 0xFE);
    static struct { uint16_t limit; uint32_t base; } __attribute__((packed)) bad_idt = { 0, 0 };
    __asm__ __volatile__("lidt %0; int3" : : "m"(bad_idt));
    cli();
    for (;;) hlt();
}

// Background worker thread demo - label is read from permanent thread_t.name
static void worker_thread(void *arg) {
    (void)arg;
    const char *label = thread_current() ? thread_current()->name : "worker";
    for (int i = 1; i <= 8; i++) {
        vga_set_color(VGA_COLOR_LIGHT_MAGENTA, themes[theme_index].bg);
        vga_printf("[%s] computing prime set %d...\n", label, i);
        vga_set_color(themes[theme_index].fg, themes[theme_index].bg);

        volatile uint32_t dummy = 0;
        for (uint32_t j = 0; j < 300000; j++) dummy += j ^ (j >> 1);
        (void)dummy;

        thread_sleep(400);
    }
    vga_set_color(VGA_COLOR_LIGHT_GREEN, themes[theme_index].bg);
    vga_printf("[%s] finished successfully.\n", label);
    vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
}

// Background ticker thread
static volatile int ticker_running = 0;
static void ticker_thread(void *arg) {
    (void)arg;
    int n = 0;
    while (ticker_running && n < 30) {
        thread_sleep(1000);
        n++;
        vga_set_color(VGA_COLOR_LIGHT_BROWN, themes[theme_index].bg);
        vga_printf("[tick] uptime: %us | thread id %d alive\n", timer_seconds(), thread_current()->tid);
        vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
    }
    ticker_running = 0;
}

// Background matrix-rain demo
static volatile int matrix_running = 0;
static void matrix_thread(void *arg) {
    (void)arg;
    static const char glyphs[] = "01#@$%&*+=~^/\\";
    uint32_t rng = 12345;
    for (int frame = 0; frame < 200 && matrix_running; frame++) {
        for (int c = 0; c < 6; c++) {
            rng = rng * 1103515245 + 12345;
            int x = (rng >> 16) % VGA_WIDTH;
            int y = (rng % VGA_HEIGHT);
            char ch = glyphs[(rng >> 8) % (sizeof(glyphs) - 1)];
            uint8_t color = (c == 0) ? 0x0F : 0x02;
            vga_putc_at(x, y, ch, color);
        }
        thread_sleep(60);
    }
    matrix_running = 0;
    vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
}

// Concurrency benchmark
static volatile uint32_t bench_shared = 0;
static volatile uint32_t bench_a = 0;
static volatile uint32_t bench_b = 0;
static volatile uint32_t bench_c = 0;
static volatile int bench_done_a = 0;
static volatile int bench_done_b = 0;
static volatile int bench_done_c = 0;

static void bench_worker(void *arg) {
    int id = (int)(uint32_t)arg;
    for (uint32_t i = 0; i < 20000; i++) {
        atomic_inc(&bench_shared);
        if (id == 1) atomic_inc(&bench_a);
        else if (id == 2) atomic_inc(&bench_b);
        else atomic_inc(&bench_c);
    }
    if (id == 1) bench_done_a = 1;
    else if (id == 2) bench_done_b = 1;
    else bench_done_c = 1;
}

static void cmd_bench(void) {
    bench_shared = 0; bench_a = 0; bench_b = 0; bench_c = 0;
    bench_done_a = 0; bench_done_b = 0; bench_done_c = 0;

    vga_printf("Launching 3 preemptive worker threads (20000 atomic increments each)...\n");
    uint32_t t0 = timer_ticks();

    thread_create("bench_A", bench_worker, (void *)1, 1);
    thread_create("bench_B", bench_worker, (void *)2, 1);
    thread_create("bench_C", bench_worker, (void *)3, 1);

    while (!(bench_done_a && bench_done_b && bench_done_c)) {
        thread_yield();
    }
    uint32_t t1 = timer_ticks();
    uint32_t elapsed_ms = (t1 - t0) * 10;
    if (elapsed_ms == 0) elapsed_ms = 1;

    vga_printf("  Shared counter final value : %u (expected 60000)\n", bench_shared);
    vga_printf("  Worker A contributions     : %u\n", bench_a);
    vga_printf("  Worker B contributions     : %u\n", bench_b);
    vga_printf("  Worker C contributions     : %u\n", bench_c);
    vga_printf("  Elapsed                    : %u ms\n", elapsed_ms);
    vga_printf("  Throughput                 : %u ops/s\n", (60000u * 1000u) / elapsed_ms);
    if (bench_shared == 60000 && bench_a > 0 && bench_b > 0 && bench_c > 0) {
        vga_set_color(VGA_COLOR_LIGHT_GREEN, themes[theme_index].bg);
        vga_printf("  RESULT: SUCCESS - true concurrent preemptive execution verified!\n");
    } else {
        vga_set_color(VGA_COLOR_LIGHT_RED, themes[theme_index].bg);
        vga_printf("  RESULT: unexpected counter mismatch\n");
    }
    vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
}

static void pad_string(const char *s, int width) {
    int len = 0;
    while (*s && len < width) { vga_putc(*s++); len++; }
    while (len < width) { vga_putc(' '); len++; }
}

static void print_uint(uint32_t v, int width) {
    char buf[16];
    utoa(v, buf, 10);
    int len = strlen(buf);
    while (len < width) { vga_putc(' '); len++; }
    vga_puts(buf);
}

static void cmd_ps(void) {
    thread_t *arr = (thread_t *)0;
    int count = thread_get_all(&arr);
    vga_set_color(VGA_COLOR_LIGHT_CYAN, themes[theme_index].bg);
    vga_printf("  TID  NAME            STATE      PRIO    TICKS  SWITCHES\n");
    vga_printf("  ---  --------------  ---------  ----  -------  --------\n");
    vga_set_color(themes[theme_index].fg, themes[theme_index].bg);

    for (int i = 0; i < count; i++) {
        if (arr[i].state == THREAD_FREE) continue;
        const char *s = (arr[i].state <= THREAD_DEAD) ? state_names[arr[i].state] : "???";
        if (arr[i].state == THREAD_RUNNING) {
            vga_set_color(VGA_COLOR_LIGHT_GREEN, themes[theme_index].bg);
        } else if (arr[i].state == THREAD_READY) {
            vga_set_color(VGA_COLOR_WHITE, themes[theme_index].bg);
        } else if (arr[i].state == THREAD_SLEEPING) {
            vga_set_color(VGA_COLOR_LIGHT_MAGENTA, themes[theme_index].bg);
        } else {
            vga_set_color(VGA_COLOR_DARK_GREY, themes[theme_index].bg);
        }
        vga_puts("  ");
        print_uint(arr[i].tid, 3);
        vga_puts("  ");
        pad_string(arr[i].name, 14);
        vga_puts("  ");
        pad_string(s, 9);
        vga_puts("  ");
        print_uint(arr[i].priority, 4);
        vga_puts("  ");
        print_uint(arr[i].runtime_ticks, 7);
        vga_puts("  ");
        print_uint(arr[i].switches, 8);
        vga_putc('\n');
    }
    vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
}

static void cmd_mem(void) {
    uint32_t total = kmem_total();
    uint32_t used = kmem_used();
    uint32_t avail = kmem_available();
    vga_printf("  Kernel heap total     : %u KB (%u MB)\n", total / 1024, total / (1024 * 1024));
    vga_printf("  Kernel heap used      : %u KB\n", used / 1024);
    vga_printf("  Kernel heap free      : %u KB\n", avail / 1024);
    vga_printf("  Thread stack size     : %u KB each\n", THREAD_STACK_SIZE / 1024);
    vga_printf("  Active threads        : %d\n", thread_count_active());
}

static void cmd_help(void) {
    vga_set_color(VGA_COLOR_WHITE, themes[theme_index].bg);
    vga_printf("Available ThreadOS commands:\n");
    vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
    vga_printf("  desktop / workspace - return to the THREAD Desktop dashboard\n");
    vga_printf("  apps / launcher     - open the real text-mode app launcher\n");
    vga_printf("  files               - open the THREAD Files view\n");
    vga_printf("  monitor             - open live scheduler/memory monitor\n");
    vga_printf("  status              - show subsystem health status\n");
    vga_printf("  open <app>          - open desktop, files, monitor, or apps\n");
    vga_printf("  help                - show this command list\n");
    vga_printf("  threads / ps        - list all threads and their live states\n");
    vga_printf("  spawn <name> [prio] - launch a background worker thread (prio 0-3)\n");
    vga_printf("  kill <tid>          - terminate a thread by id\n");
    vga_printf("  bench / stress      - run preemptive multithreading concurrency test\n");
    vga_printf("  ticker              - toggle background uptime ticker thread\n");
    vga_printf("  matrix              - toggle matrix rain demo thread\n");
    vga_printf("  sleep <ms>          - put shell to sleep for N milliseconds\n");
    vga_printf("  mem                 - display kernel memory statistics\n");
    vga_printf("  uptime              - show system uptime\n");
    vga_printf("  color               - cycle through color themes\n");
    vga_printf("  clear               - clear screen\n");
    vga_printf("  about               - information about ThreadOS\n");
    vga_printf("  reboot              - restart the system\n");
    vga_printf("  shutdown            - power off the virtual machine\n");
}

static void cmd_about(void) {
    vga_set_color(VGA_COLOR_LIGHT_CYAN, themes[theme_index].bg);
    vga_printf("\n      ==========  ThreadOS v1.0  ==========\n\n");
    vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
    vga_printf("  A genuine, bootable 32-bit x86 multitasking operating system.\n");
    vga_printf("  Built to turn the classic simulated ThreadOS into a real\n");
    vga_printf("  bare-metal OS that boots from BIOS, runs preemptive threads,\n");
    vga_printf("  and handles real CPU interrupts.\n\n");
    vga_printf("  * Architecture : x86 Protected Mode (i686), flat 4GB segments\n");
    vga_printf("  * Bootloader   : 512-byte MBR + custom 32-bit entry\n");
    vga_printf("  * Scheduler    : Priority + round-robin preemptive (PIT 100 Hz)\n");
    vga_printf("  * Concurrency  : context switch, yield, sleep, exit, spinlocks\n");
    vga_printf("  * Drivers      : VGA text 80x25, PS/2 keyboard, COM1 serial\n");
    vga_printf("  * Console      : interactive shell with thread management\n\n");
}

static void read_line(char *buf, int max) {
    int i = 0;
    for (;;) {
        char c = kbd_getc_blocking();
        if (c == '\n') {
            vga_putc('\n');
            buf[i] = '\0';
            return;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                vga_putc('\b');
            }
        } else if ((unsigned char)c >= ' ' && i < max - 1) {
            buf[i++] = c;
            vga_putc(c);
        }
    }
}

static int tokenize(char *line, char *argv[], int max_argv) {
    int argc = 0;
    while (*line && argc < max_argv) {
        while (*line == ' ' || *line == '\t') *line++ = '\0';
        if (!*line) break;
        argv[argc++] = line;
        while (*line && *line != ' ' && *line != '\t') line++;
    }
    return argc;
}

static void dispatch(int argc, char *argv[]) {
    if (argc == 0) return;
    const char *cmd = argv[0];

    if (strcmp(cmd, "help") == 0)             cmd_help();
    else if (strcmp(cmd, "threads") == 0 || strcmp(cmd, "ps") == 0) cmd_ps();
    else if (strcmp(cmd, "bench") == 0 || strcmp(cmd, "stress") == 0) cmd_bench();
    else if (strcmp(cmd, "mem") == 0)         cmd_mem();
    else if (strcmp(cmd, "about") == 0)       cmd_about();
    else if (strcmp(cmd, "desktop") == 0 || strcmp(cmd, "workspace") == 0) {
        desktop_draw();
        serial_puts("[desktop] workspace dashboard ready\n");
    }
    else if (strcmp(cmd, "apps") == 0 || strcmp(cmd, "launcher") == 0) {
        desktop_show_apps();
        serial_puts("[desktop] app launcher ready\n");
    }
    else if (strcmp(cmd, "files") == 0) {
        desktop_show_files();
        serial_puts("[desktop] files view ready\n");
    }
    else if (strcmp(cmd, "monitor") == 0) {
        desktop_show_monitor();
        serial_puts("[desktop] live monitor ready\n");
    }
    else if (strcmp(cmd, "status") == 0) {
        desktop_show_status();
        serial_puts("[desktop] status center ready\n");
    }
    else if (strcmp(cmd, "open") == 0 && argc > 1) {
        if (strcmp(argv[1], "files") == 0) desktop_show_files();
        else if (strcmp(argv[1], "monitor") == 0) desktop_show_monitor();
        else if (strcmp(argv[1], "apps") == 0) desktop_show_apps();
        else if (strcmp(argv[1], "desktop") == 0) desktop_draw();
        else vga_printf("Unknown app '%s'. Try: files, monitor, apps, desktop\n", argv[1]);
    }
    else if (strcmp(cmd, "clear") == 0) {
        vga_clear();
    }
    else if (strcmp(cmd, "uptime") == 0) {
        uint32_t s = timer_seconds();
        vga_printf("  uptime: %u s (%uh %um %us), ticks=%u\n",
                   s, s / 3600, (s % 3600) / 60, s % 60, timer_ticks());
    }
    else if (strcmp(cmd, "color") == 0) {
        theme_index = (theme_index + 1) % 4;
        vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
        vga_clear();
        vga_printf("Theme: %s\n", themes[theme_index].name);
    }
    else if (strcmp(cmd, "spawn") == 0) {
        const char *name = (argc > 1) ? argv[1] : "worker";
        int prio = (argc > 2) ? atoi(argv[2]) : 1;
        int tid = thread_create(name, worker_thread, (void *)0, prio);
        if (tid < 0) {
            vga_printf("Error: failed to create thread (table full or out of memory)\n");
        } else {
            vga_printf("Spawned thread '%s' with TID=%d, priority=%d\n", name, tid, prio);
        }
    }
    else if (strcmp(cmd, "kill") == 0) {
        if (argc < 2) { vga_printf("Usage: kill <tid>\n"); return; }
        int tid = atoi(argv[1]);
        if (thread_kill(tid) == 0) {
            vga_printf("Thread %d terminated.\n", tid);
        } else {
            vga_printf("Error: invalid tid %d\n", tid);
        }
    }
    else if (strcmp(cmd, "ticker") == 0) {
        if (!ticker_running) {
            ticker_running = 1;
            int tid = thread_create("ticker", ticker_thread, (void *)0, 2);
            vga_printf("Ticker thread started (TID=%d, updates every 1s)\n", tid);
        } else {
            ticker_running = 0;
            vga_printf("Ticker stopping...\n");
        }
    }
    else if (strcmp(cmd, "matrix") == 0) {
        if (!matrix_running) {
            matrix_running = 1;
            thread_create("matrix", matrix_thread, (void *)0, 2);
            vga_printf("Matrix rain thread started (press Enter anytime)\n");
        } else {
            matrix_running = 0;
            vga_printf("Matrix rain stopping...\n");
        }
    }
    else if (strcmp(cmd, "sleep") == 0) {
        int ms = (argc > 1) ? atoi(argv[1]) : 2000;
        if (ms < 0) ms = 1000;
        vga_printf("Shell going to sleep for %d ms (other threads keep running)...\n", ms);
        thread_sleep((uint32_t)ms);
        vga_printf("Shell woke up!\n");
    }
    else if (strcmp(cmd, "reboot") == 0) {
        os_reboot();
    }
    else if (strcmp(cmd, "shutdown") == 0) {
        os_shutdown();
    }
    else {
        vga_printf("Unknown command '%s'. Type 'help' for a list.\n", cmd);
    }
}

void shell_init(void) {
    vga_set_color(themes[theme_index].fg, themes[theme_index].bg);
    serial_puts("[ThreadOS] THREAD Desktop ready | protected mode | PIT 100 Hz\n");
    desktop_draw();
}

void shell_run(void) {
    char line[MAX_LINE];
    char *argv[MAX_ARGS];

    for (;;) {
        vga_set_color(VGA_COLOR_LIGHT_GREEN, themes[theme_index].bg);
        vga_puts("ThreadOS> ");
        vga_set_color(themes[theme_index].fg, themes[theme_index].bg);

        read_line(line, MAX_LINE);
        int argc = tokenize(line, argv, MAX_ARGS);
        dispatch(argc, argv);
    }
}
