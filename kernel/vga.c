#include "vga.h"
#include "io.h"
#include "string.h"
#include "serial.h"

static volatile uint16_t * const VGA_BUFFER = (volatile uint16_t *)0xB8000;

static int cursor_x = 0;
static int cursor_y = 0;
static uint8_t current_color = 0x07; // Light grey on black

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)(uint8_t)c | ((uint16_t)color << 8);
}

static void update_hardware_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_init(void) {
    current_color = ((uint8_t)VGA_COLOR_BLACK << 4) | (uint8_t)VGA_COLOR_LIGHT_GREY;
    vga_clear();
}

void vga_clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_BUFFER[y * VGA_WIDTH + x] = vga_entry(' ', current_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    update_hardware_cursor();
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    current_color = (bg << 4) | (fg & 0x0F);
}

uint8_t vga_get_color(void) {
    return current_color;
}

void vga_set_cursor(int x, int y) {
    if (x < 0) x = 0;
    if (x >= VGA_WIDTH) x = VGA_WIDTH - 1;
    if (y < 0) y = 0;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;
    cursor_x = x;
    cursor_y = y;
    update_hardware_cursor();
}

void vga_get_cursor(int *x, int *y) {
    if (x) *x = cursor_x;
    if (y) *y = cursor_y;
}

void vga_putc_at(int x, int y, char c, uint8_t color) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        VGA_BUFFER[y * VGA_WIDTH + x] = vga_entry(c, color);
    }
}

static void scroll(void) {
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_BUFFER[y * VGA_WIDTH + x] = VGA_BUFFER[(y + 1) * VGA_WIDTH + x];
        }
    }
    for (int x = 0; x < VGA_WIDTH; x++) {
        VGA_BUFFER[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', current_color);
    }
    cursor_y = VGA_HEIGHT - 1;
}

void vga_putc(char c) {
    serial_putc(c);

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        int next_tab = (cursor_x + 4) & ~3;
        while (cursor_x < next_tab && cursor_x < VGA_WIDTH) {
            VGA_BUFFER[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', current_color);
            cursor_x++;
        }
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            VGA_BUFFER[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', current_color);
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
            VGA_BUFFER[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', current_color);
        }
    } else {
        VGA_BUFFER[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, current_color);
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT) {
        scroll();
    }
    update_hardware_cursor();
}

void vga_puts(const char *s) {
    while (*s) {
        vga_putc(*s++);
    }
}

void vga_vprintf(const char *fmt, va_list ap) {
    char numbuf[32];
    for (const char *p = fmt; *p; p++) {
        if (*p != '%') {
            vga_putc(*p);
            continue;
        }
        p++;
        switch (*p) {
            case 's': {
                const char *s = va_arg(ap, const char *);
                vga_puts(s ? s : "(null)");
                break;
            }
            case 'd':
            case 'i': {
                int v = va_arg(ap, int);
                itoa(v, numbuf, 10);
                vga_puts(numbuf);
                break;
            }
            case 'u': {
                uint32_t v = va_arg(ap, uint32_t);
                utoa(v, numbuf, 10);
                vga_puts(numbuf);
                break;
            }
            case 'x': {
                uint32_t v = va_arg(ap, uint32_t);
                utoa(v, numbuf, 16);
                vga_puts(numbuf);
                break;
            }
            case 'p': {
                uint32_t v = (uint32_t)va_arg(ap, void *);
                vga_puts("0x");
                utoa(v, numbuf, 16);
                vga_puts(numbuf);
                break;
            }
            case 'c': {
                char ch = (char)va_arg(ap, int);
                vga_putc(ch);
                break;
            }
            case '%': {
                vga_putc('%');
                break;
            }
            default:
                vga_putc('%');
                if (*p) vga_putc(*p);
                break;
        }
    }
}

void vga_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vga_vprintf(fmt, ap);
    va_end(ap);
}
