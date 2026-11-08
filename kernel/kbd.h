#ifndef KBD_H
#define KBD_H

#include <stdint.h>
#include "idt.h"

void kbd_init(void);
void kbd_irq_handler(registers_t *regs);
int kbd_has_char(void);
char kbd_getc(void);       // non-blocking, returns 0 if empty
char kbd_getc_blocking(void); // blocking, yields to other threads while waiting

#endif
