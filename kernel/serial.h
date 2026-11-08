#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#define COM1 0x3F8

void serial_init(void);
void serial_putc(char c);
void serial_puts(const char *s);
int serial_has_data(void);
char serial_getc(void);

#endif
