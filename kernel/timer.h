#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "idt.h"

void timer_init(uint32_t frequency_hz);
void timer_irq_handler(registers_t *regs);
uint32_t timer_ticks(void);
uint32_t timer_ms(void);
uint32_t timer_seconds(void);

#endif
