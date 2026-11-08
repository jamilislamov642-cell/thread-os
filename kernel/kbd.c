#include "kbd.h"
#include "io.h"
#include "serial.h"

#define KBD_BUFFER_SIZE 128

static char kbd_buffer[KBD_BUFFER_SIZE];
static volatile int buf_head = 0;
static volatile int buf_tail = 0;
static int shift_pressed = 0;
static int caps_lock = 0;

static const char kbd_map_lower[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' '
};

static const char kbd_map_upper[128] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t', 'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0, 'A','S','D','F','G','H','J','K','L',':','"','~',
    0, '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0, ' '
};

static void buf_push(char c) {
    int next = (buf_tail + 1) % KBD_BUFFER_SIZE;
    if (next != buf_head) {
        kbd_buffer[buf_tail] = c;
        buf_tail = next;
    }
}

void kbd_init(void) {
    buf_head = 0;
    buf_tail = 0;
    shift_pressed = 0;
    caps_lock = 0;
    register_irq_handler(1, kbd_irq_handler);
}

void kbd_irq_handler(registers_t *regs) {
    (void)regs;
    uint8_t scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) { // Left/Right shift press
        shift_pressed = 1;
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6) { // Shift release
        shift_pressed = 0;
        return;
    }
    if (scancode == 0x3A) { // CapsLock press
        caps_lock = !caps_lock;
        return;
    }
    if (scancode & 0x80) { // Key release
        return;
    }

    if (scancode < sizeof(kbd_map_lower)) {
        char ch;
        int use_upper = shift_pressed ^ caps_lock;
        char lower = kbd_map_lower[scancode];
        if (use_upper && lower >= 'a' && lower <= 'z') {
            ch = kbd_map_upper[scancode];
        } else if (shift_pressed && !(lower >= 'a' && lower <= 'z')) {
            ch = kbd_map_upper[scancode];
        } else {
            ch = lower;
        }
        if (ch) {
            buf_push(ch);
        }
    }
}

int kbd_has_char(void) {
    // Check serial input too
    if (serial_has_data()) {
        char sc = (char)inb(COM1);
        if (sc == '\r') sc = '\n';
        buf_push(sc);
    }
    return buf_head != buf_tail;
}

char kbd_getc(void) {
    if (!kbd_has_char()) return 0;
    char c = kbd_buffer[buf_head];
    buf_head = (buf_head + 1) % KBD_BUFFER_SIZE;
    return c;
}

extern void thread_yield(void);

char kbd_getc_blocking(void) {
    while (!kbd_has_char()) {
        thread_yield();
    }
    return kbd_getc();
}
