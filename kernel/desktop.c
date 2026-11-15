#include "desktop.h"
#include "vga.h"
#include "thread.h"
#include "timer.h"
#include "mem.h"
#include "string.h"

extern uint8_t shell_theme_fg(void);
extern uint8_t shell_theme_bg(void);
extern const char *shell_theme_name(void);

static void line(char ch, uint8_t color) {
    for (int x = 0; x < VGA_WIDTH; x++) vga_putc_at(x, 0, ch, color);
}

static void fill_row(int y, char ch, uint8_t color) {
    for (int x = 0; x < VGA_WIDTH; x++) vga_putc_at(x, y, ch, color);
}

static void text_at(int x, int y, const char *s, uint8_t color) {
    while (*s && x < VGA_WIDTH) vga_putc_at(x++, y, *s++, color);
}

static void num_at(int x, int y, uint32_t n, uint8_t color) {
    char b[16];
    utoa(n, b, 10);
    text_at(x, y, b, color);
}

static void panel(int x, int y, int w, int h, const char *title, uint8_t border, uint8_t body) {
    for (int i = 0; i < w; i++) {
        vga_putc_at(x + i, y, i == 0 || i == w - 1 ? '+' : '-', border);
        vga_putc_at(x + i, y + h - 1, i == 0 || i == w - 1 ? '+' : '-', border);
    }
    for (int j = 1; j < h - 1; j++) {
        vga_putc_at(x, y + j, '|', border);
        vga_putc_at(x + w - 1, y + j, '|', border);
        for (int i = 1; i < w - 1; i++) vga_putc_at(x + i, y + j, ' ', body);
    }
    text_at(x + 2, y, title, border);
}

void desktop_draw(void) {
    uint8_t fg = shell_theme_fg();
    uint8_t bg = shell_theme_bg();
    uint8_t white = VGA_COLOR_WHITE;
    uint8_t silver = VGA_COLOR_LIGHT_GREY;
    uint8_t blue = VGA_COLOR_LIGHT_CYAN;
    uint8_t green = VGA_COLOR_LIGHT_GREEN;
    uint8_t violet = VGA_COLOR_LIGHT_MAGENTA;
    uint8_t amber = VGA_COLOR_LIGHT_BROWN;
    uint8_t red = VGA_COLOR_LIGHT_RED;

    vga_clear();
    /* THREAD Workspace: a macOS-inspired shell rendered directly by the kernel. */
    fill_row(0, ' ', VGA_COLOR_DARK_GREY);
    text_at(2, 0, "@", white);
    text_at(5, 0, "THREAD", white);
    text_at(14, 0, "File", silver);
    text_at(21, 0, "Edit", silver);
    text_at(29, 0, "View", silver);
    text_at(37, 0, "Window", silver);
    text_at(47, 0, "Help", silver);
    text_at(61, 0, "PIT 100Hz", green);
    text_at(72, 0, "ON", green);
    fill_row(1, ' ', bg);
    text_at(3, 2, "THREAD / WORKSPACE", blue);
    text_at(58, 2, "uptime", silver);
    num_at(66, 2, timer_seconds(), fg);
    text_at(72, 2, "sec", silver);

    panel(8, 4, 64, 15, "THREAD Workspace", blue, bg);
    /* Window controls and Finder-like sidebar. */
    text_at(11, 5, "o o o", red);
    text_at(18, 5, "native desktop", silver);
    text_at(11, 7, "FAVORITES", silver);
    text_at(11, 8, "[H] Home", white);
    text_at(11, 9, "[F] Files", amber);
    text_at(11, 10, "[A] Apps", violet);
    text_at(11, 12, "SYSTEM", silver);
    text_at(11, 13, "[M] Monitor", blue);
    text_at(11, 14, "[S] Settings", green);
    text_at(11, 15, "[P] Dashboard", VGA_COLOR_LIGHT_CYAN);
    text_at(11, 16, "THREAD OS", blue);
    text_at(11, 17, "i686 native", silver);

    text_at(28, 7, "Good morning, operator.", white);
    text_at(28, 8, "This Mac is actually a kernel.", blue);
    panel(28, 10, 19, 5, "THIS THREAD", violet, bg);
    text_at(30, 12, "protected mode", green);
    text_at(30, 13, "scheduler live", green);
    text_at(30, 14, "threads", silver);
    num_at(39, 14, thread_count_active(), fg);
    panel(49, 10, 19, 5, "STORAGE", amber, bg);
    text_at(51, 12, "kernel heap", silver);
    num_at(51, 13, kmem_available() / 1024, fg);
    text_at(58, 13, "K free", silver);
    text_at(51, 14, "filesystem", green);
    text_at(61, 14, "READY", green);
    text_at(28, 17, "Press a shortcut or type a command to open a view.", silver);

    /* Dock: compact, centered, and keyboard-first until a mouse driver arrives. */
    fill_row(21, ' ', bg);
    text_at(19, 21, ".------------------- DOCK -------------------.", blue);
    text_at(20, 22, "[@] Finder [>] Terminal [#] Monitor [~] Dashboard", white);
    text_at(23, 23, "[$] Apps   [*] Settings   [!] Power", silver);
    vga_set_cursor(0, 24);
}

void desktop_show_apps(void) {
    uint8_t a = VGA_COLOR_LIGHT_CYAN, f = shell_theme_fg(), d = VGA_COLOR_LIGHT_GREY;
    vga_clear();
    text_at(2, 1, "THREAD APP LAUNCHER", a);
    text_at(2, 2, "Select an app by typing its command.", d);
    panel(2, 4, 24, 5, "TERMINAL", a, shell_theme_bg());
    text_at(4, 6, "shell + kernel tools", f);
    text_at(4, 7, "command: help", d);
    panel(28, 4, 24, 5, "FILES", VGA_COLOR_LIGHT_BROWN, shell_theme_bg());
    text_at(30, 6, "system files + projects", f);
    text_at(30, 7, "command: files", d);
    panel(54, 4, 24, 5, "MONITOR", VGA_COLOR_LIGHT_MAGENTA, shell_theme_bg());
    text_at(56, 6, "live scheduler metrics", f);
    text_at(56, 7, "command: monitor", d);
    panel(2, 11, 24, 5, "BENCH", VGA_COLOR_LIGHT_GREEN, shell_theme_bg());
    text_at(4, 13, "atomic concurrency test", f);
    text_at(4, 14, "command: bench", d);
    panel(28, 11, 24, 5, "SETTINGS", VGA_COLOR_LIGHT_RED, shell_theme_bg());
    text_at(30, 13, "themes + power controls", f);
    text_at(30, 14, "command: color", d);
    panel(54, 11, 24, 5, "ABOUT", VGA_COLOR_LIGHT_CYAN, shell_theme_bg());
    text_at(56, 13, "kernel information", f);
    text_at(56, 14, "command: about", d);
    fill_row(21, '-', a);
    text_at(2, 23, "apps: terminal  files  monitor  bench  settings  about", f);
    vga_set_cursor(0, 24);
}

void desktop_show_files(void) {
    uint8_t a = VGA_COLOR_LIGHT_BROWN, f = shell_theme_fg(), d = VGA_COLOR_LIGHT_GREY;
    vga_clear();
    text_at(2, 1, "THREAD FILES", a);
    text_at(2, 2, "/home/operator", d);
    panel(2, 4, 36, 15, "PROJECTS", a, shell_theme_bg());
    text_at(4, 6, "<DIR>  /home", f);
    text_at(4, 8, "<DIR>  /home/operator", f);
    text_at(4, 10, "<DIR>  /projects", f);
    text_at(4, 12, "<DIR>  /projects/thread", f);
    text_at(4, 14, "README.md", d);
    text_at(4, 16, "kernel.c", d);
    panel(41, 4, 37, 15, "QUICK ACCESS", a, shell_theme_bg());
    text_at(43, 6, "[1] /home/operator", f);
    text_at(43, 8, "[2] /projects/thread", f);
    text_at(43, 10, "[3] /boot", f);
    text_at(43, 12, "[4] /kernel", f);
    text_at(43, 14, "Filesystem driver: ready", VGA_COLOR_LIGHT_GREEN);
    text_at(43, 16, "Use terminal for file commands", d);
    fill_row(21, '-', a);
    text_at(2, 23, "files: virtual workspace is ready; terminal is the active editor", f);
    vga_set_cursor(0, 24);
}

void desktop_show_monitor(void) {
    uint8_t a = VGA_COLOR_LIGHT_MAGENTA, f = shell_theme_fg(), d = VGA_COLOR_LIGHT_GREY;
    thread_t *threads = (thread_t *)0;
    int count = thread_get_all(&threads);
    vga_clear();
    text_at(2, 1, "THREAD MONITOR", a);
    text_at(2, 2, "live kernel telemetry / PIT-driven scheduler", d);
    panel(2, 4, 23, 7, "UPTIME", a, shell_theme_bg());
    text_at(4, 6, "seconds", d); num_at(15, 6, timer_seconds(), f);
    text_at(4, 8, "ticks", d); num_at(15, 8, timer_ticks(), f);
    panel(27, 4, 24, 7, "MEMORY", VGA_COLOR_LIGHT_GREEN, shell_theme_bg());
    text_at(29, 6, "heap total", d); num_at(43, 6, kmem_total()/1024, f); text_at(47, 6, "K", d);
    text_at(29, 8, "heap free", d); num_at(43, 8, kmem_available()/1024, f); text_at(47, 8, "K", d);
    panel(53, 4, 25, 7, "SCHEDULER", VGA_COLOR_LIGHT_CYAN, shell_theme_bg());
    text_at(55, 6, "active threads", d); num_at(71, 6, thread_count_active(), f);
    text_at(55, 8, "PIT frequency", d); text_at(70, 8, "100Hz", f);
    panel(2, 13, 76, 6, "THREAD ACTIVITY", a, shell_theme_bg());
    int row = 15;
    for (int i = 0; i < count && row < 18; i++) {
        if (threads[i].state == THREAD_FREE) continue;
        text_at(4, row, threads[i].name, f);
        text_at(23, row, threads[i].state == THREAD_RUNNING ? "RUNNING" :
                          threads[i].state == THREAD_READY ? "READY" :
                          threads[i].state == THREAD_SLEEPING ? "SLEEPING" : "IDLE", d);
        text_at(36, row, "ticks", d); num_at(43, row, threads[i].runtime_ticks, f);
        text_at(54, row, "switches", d); num_at(65, row, threads[i].switches, f);
        row++;
    }
    fill_row(21, '-', a);
    text_at(2, 23, "monitor: scheduler is live; run 'bench' to stress atomic concurrency", f);
    vga_set_cursor(0, 24);
}

static uint32_t dashboard_runtime(thread_t *threads, int count) {
    uint32_t total = 0;
    for (int i = 0; i < count; i++) {
        if (threads[i].state == THREAD_FREE || threads[i].state == THREAD_DEAD) continue;
        if (strcmp(threads[i].name, "idle") == 0) continue;
        total += threads[i].runtime_ticks;
    }
    return total;
}

static void dashboard_graph(const uint8_t *history, int samples, uint8_t color) {
    for (int x = 0; x < 64; x++) {
        int i = samples - 64 + x;
        uint8_t value = (i >= 0) ? history[i] : 0;
        int y = 12 - (value * 5 / 100);
        if (y < 7) y = 7;
        if (y > 12) y = 12;
        vga_putc_at(9 + x, y, '*', color);
        if (x > 0) {
            int prev_i = i - 1;
            uint8_t prev = (prev_i >= 0) ? history[prev_i] : 0;
            int py = 12 - (prev * 5 / 100);
            if (py < 7) py = 7;
            if (py > 12) py = 12;
            if (py != y) vga_putc_at(9 + x, (py + y) / 2, '.', color);
        }
    }
}

static void dashboard_draw(const uint8_t *history, int samples, uint32_t busy, uint32_t ram, uint32_t seconds) {
    uint8_t fg = shell_theme_fg(), bg = shell_theme_bg();
    vga_clear();
    fill_row(0, ' ', VGA_COLOR_DARK_GREY);
    text_at(2, 0, "@ THREAD", VGA_COLOR_WHITE);
    text_at(18, 0, "PERFORMANCE", VGA_COLOR_LIGHT_CYAN);
    text_at(59, 0, "LIVE / 100Hz", VGA_COLOR_LIGHT_GREEN);
    text_at(2, 2, "SYSTEM DASHBOARD", VGA_COLOR_LIGHT_CYAN);
    text_at(22, 2, "real kernel samples only", VGA_COLOR_LIGHT_GREY);
    text_at(58, 2, "uptime", VGA_COLOR_LIGHT_GREY);
    num_at(66, 2, seconds, fg);
    text_at(72, 2, "sec", VGA_COLOR_LIGHT_GREY);

    panel(2, 4, 76, 11, "SCHEDULER LOAD WAVE  /  non-idle runtime", VGA_COLOR_LIGHT_CYAN, bg);
    text_at(4, 6, "100%", VGA_COLOR_DARK_GREY);
    text_at(4, 12, "0%", VGA_COLOR_DARK_GREY);
    for (int x = 9; x < 73; x++) vga_putc_at(x, 12, '-', VGA_COLOR_DARK_GREY);
    dashboard_graph(history, samples, VGA_COLOR_LIGHT_CYAN);
    text_at(4, 14, "scheduler utilization", VGA_COLOR_LIGHT_GREY);
    num_at(27, 14, busy, VGA_COLOR_LIGHT_GREEN);
    text_at(31, 14, "%", VGA_COLOR_LIGHT_GREEN);
    text_at(40, 14, "active threads", VGA_COLOR_LIGHT_GREY);
    num_at(56, 14, thread_count_active(), fg);

    panel(2, 16, 24, 5, "MEMORY", VGA_COLOR_LIGHT_GREEN, bg);
    text_at(4, 18, "heap used", VGA_COLOR_LIGHT_GREY);
    num_at(15, 18, ram, VGA_COLOR_LIGHT_GREEN);
    text_at(20, 18, "%", VGA_COLOR_LIGHT_GREEN);
    text_at(4, 19, "free", VGA_COLOR_LIGHT_GREY);
    num_at(10, 19, kmem_available() / 1024, fg);
    text_at(16, 19, "K", VGA_COLOR_LIGHT_GREY);

    panel(28, 16, 24, 5, "HARDWARE", VGA_COLOR_LIGHT_BROWN, bg);
    text_at(30, 18, "CPU", VGA_COLOR_LIGHT_GREY);
    text_at(35, 18, "i686 / real", VGA_COLOR_LIGHT_GREEN);
    text_at(30, 19, "GPU / temp", VGA_COLOR_LIGHT_GREY);
    text_at(41, 19, "unavailable", VGA_COLOR_LIGHT_BROWN);

    panel(54, 16, 24, 5, "IO STATUS", VGA_COLOR_LIGHT_MAGENTA, bg);
    text_at(56, 18, "disk", VGA_COLOR_LIGHT_GREY);
    text_at(62, 18, "not exposed", VGA_COLOR_LIGHT_BROWN);
    text_at(56, 19, "network", VGA_COLOR_LIGHT_GREY);
    text_at(65, 19, "not exposed", VGA_COLOR_LIGHT_BROWN);
    fill_row(22, '=', VGA_COLOR_LIGHT_CYAN);
    text_at(2, 23, "live dashboard  |  press any key to return  |  no simulated hardware data", fg);
    vga_set_cursor(0, 24);
}

void desktop_show_dashboard(void) {
    uint8_t history[64];
    for (int i = 0; i < 64; i++) history[i] = 0;
    thread_t *threads = (thread_t *)0;
    int count = thread_get_all(&threads);
    uint32_t previous_ticks = timer_ticks();
    uint32_t previous_runtime = dashboard_runtime(threads, count);
    int samples = 0;
    for (;;) {
        uint32_t now_ticks = timer_ticks();
        uint32_t now_runtime = dashboard_runtime(threads, count);
        uint32_t dt = now_ticks - previous_ticks;
        uint32_t dr = now_runtime - previous_runtime;
        uint32_t busy = dt ? (dr * 100u) / dt : 0;
        if (busy > 100) busy = 100;
        uint32_t used = kmem_total() ? (kmem_used() * 100u) / kmem_total() : 0;
        if (used > 100) used = 100;
        if (samples < 64) history[samples++] = (uint8_t)busy;
        else {
            for (int i = 1; i < 64; i++) history[i - 1] = history[i];
            history[63] = (uint8_t)busy;
        }
        dashboard_draw(history, samples, busy, used, timer_seconds());
        previous_ticks = now_ticks;
        previous_runtime = now_runtime;
        if (kbd_has_char()) { (void)kbd_getc(); break; }
        thread_sleep(100);
    }
    desktop_draw();
}

void desktop_show_status(void) {
    uint8_t f = shell_theme_fg(), a = VGA_COLOR_LIGHT_CYAN;
    vga_clear();
    text_at(2, 1, "THREAD STATUS CENTER", a);
    text_at(2, 3, "BOOT", VGA_COLOR_LIGHT_GREEN);
    text_at(18, 3, "OK  BIOS -> MBR -> protected mode -> kernel_main", f);
    text_at(2, 5, "INPUT", VGA_COLOR_LIGHT_GREEN);
    text_at(18, 5, "OK  PS/2 keyboard + COM1 serial console", f);
    text_at(2, 7, "SCHEDULER", VGA_COLOR_LIGHT_GREEN);
    text_at(18, 7, "OK  PIT 100 Hz / priority round-robin / preemption", f);
    text_at(2, 9, "MEMORY", VGA_COLOR_LIGHT_GREEN);
    text_at(18, 9, "OK  14 MB heap / 8 KB per thread stack", f);
    text_at(2, 11, "SHELL", VGA_COLOR_LIGHT_GREEN);
    text_at(18, 11, "OK  terminal, files, monitor, apps, power", f);
    text_at(2, 14, "THEME", a);
    text_at(18, 14, shell_theme_name(), f);
    text_at(2, 16, "UPTIME", a);
    text_at(18, 16, "ticks:", f); num_at(25, 16, timer_ticks(), f);
    fill_row(21, '=', a);
    text_at(2, 23, "status: all core subsystems operational", VGA_COLOR_LIGHT_GREEN);
    vga_set_cursor(0, 24);
}


void desktop_show_settings(void) {
    uint8_t f = shell_theme_fg(), a = VGA_COLOR_LIGHT_CYAN, d = VGA_COLOR_LIGHT_GREY;
    vga_clear();
    text_at(2, 1, "THREAD SETTINGS", a);
    text_at(2, 2, "personalize the system without leaving the kernel", d);
    panel(2, 4, 36, 13, "APPEARANCE", a, shell_theme_bg());
    text_at(4, 6, "ACTIVE THEME", d);
    text_at(21, 6, shell_theme_name(), f);
    text_at(4, 8, "[1] Classic White", f);
    text_at(4, 10, "[2] Matrix Green", VGA_COLOR_LIGHT_GREEN);
    text_at(4, 12, "[3] Solar Cyan", VGA_COLOR_LIGHT_CYAN);
    text_at(4, 14, "[4] Amber Terminal", VGA_COLOR_LIGHT_BROWN);
    text_at(4, 16, "theme next  cycles themes", d);
    panel(41, 4, 37, 13, "SYSTEM CONTROLS", VGA_COLOR_LIGHT_GREEN, shell_theme_bg());
    text_at(43, 6, "[D] desktop dashboard", f);
    text_at(43, 8, "[M] live monitor", f);
    text_at(43, 9, "[P] performance dashboard", f);
    text_at(43, 10, "[R] reboot", VGA_COLOR_LIGHT_RED);
    text_at(43, 12, "[S] shutdown", VGA_COLOR_LIGHT_RED);
    text_at(43, 14, "preference storage: kernel", d);
    text_at(43, 16, "safe mode: enabled", VGA_COLOR_LIGHT_GREEN);
    fill_row(21, '=', a);
    text_at(2, 22, "THREAD // SETTINGS", a);
    text_at(2, 23, "theme next  desktop  monitor  reboot  shutdown", f);
    vga_set_cursor(0, 24);
}
