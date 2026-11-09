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
    uint8_t accent = VGA_COLOR_LIGHT_CYAN;
    uint8_t dim = VGA_COLOR_LIGHT_GREY;
    uint8_t green = VGA_COLOR_LIGHT_GREEN;
    uint8_t purple = VGA_COLOR_LIGHT_MAGENTA;
    uint8_t yellow = VGA_COLOR_LIGHT_BROWN;

    vga_clear();
    fill_row(0, ' ', bg);
    text_at(2, 0, "THREAD DESKTOP", accent);
    text_at(61, 0, "v1.1  x86", dim);
    text_at(2, 1, "Your computer. Your rules. Your system.", fg);
    text_at(59, 1, "uptime ", dim);
    num_at(66, 1, timer_seconds(), fg);
    text_at(71, 1, "s", dim);

    panel(0, 3, 17, 17, "DOCK", accent, bg);
    text_at(2, 5, "[1] WORKSPACE", fg);
    text_at(2, 7, "[2] TERMINAL", green);
    text_at(2, 9, "[3] FILES", yellow);
    text_at(2, 11, "[4] MONITOR", purple);
    text_at(2, 13, "[5] APPS", accent);
    text_at(2, 15, "[6] SETTINGS", dim);
    text_at(2, 18, "desktop", accent);

    panel(19, 3, 42, 9, "WORKSPACE  /  OVERVIEW", accent, bg);
    text_at(21, 5, "Welcome back, operator.", fg);
    text_at(21, 7, "[T] Terminal     [F] Files", green);
    text_at(21, 8, "[M] Monitor      [A] App launcher", green);
    text_at(21, 10, "Type a command below to control THREAD.", dim);

    panel(63, 3, 17, 9, "SYSTEM", purple, bg);
    text_at(65, 5, "THREAD OS", fg);
    text_at(65, 6, "protected mode", green);
    text_at(65, 7, "PIT 100 Hz", green);
    text_at(65, 8, "heap", dim);
    num_at(71, 8, kmem_available() / 1024, fg);
    text_at(76, 8, "K", dim);
    text_at(65, 10, "online", green);

    panel(19, 13, 42, 7, "ACTIVITY", yellow, bg);
    text_at(21, 15, "scheduler", dim);
    text_at(33, 15, "RUNNING", green);
    text_at(21, 16, "threads", dim);
    num_at(33, 16, thread_count_active(), fg);
    text_at(21, 17, "ticks", dim);
    num_at(33, 17, timer_ticks(), fg);
    text_at(21, 18, "ready for commands", accent);

    panel(63, 13, 17, 7, "NOTES", green, bg);
    text_at(65, 15, "Boot complete.", fg);
    text_at(65, 16, "Try 'apps'", dim);
    text_at(65, 17, "or 'help'.", dim);
    text_at(65, 18, "safe to use", green);

    fill_row(21, '=', accent);
    text_at(2, 22, "[THREAD]", accent);
    text_at(12, 22, "desktop", fg);
    text_at(24, 22, "terminal", green);
    text_at(37, 22, "files", yellow);
    text_at(47, 22, "monitor", purple);
    text_at(59, 22, "apps", accent);
    text_at(70, 22, "online", green);
    text_at(2, 23, "commands: desktop  apps  files  monitor  status  help", dim);
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
    text_at(43, 10, "[R] reboot", VGA_COLOR_LIGHT_RED);
    text_at(43, 12, "[S] shutdown", VGA_COLOR_LIGHT_RED);
    text_at(43, 14, "preference storage: kernel", d);
    text_at(43, 16, "safe mode: enabled", VGA_COLOR_LIGHT_GREEN);
    fill_row(21, '=', a);
    text_at(2, 22, "THREAD // SETTINGS", a);
    text_at(2, 23, "theme next  desktop  monitor  reboot  shutdown", f);
    vga_set_cursor(0, 24);
}
