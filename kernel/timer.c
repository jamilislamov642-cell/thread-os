#include "timer.h"
#include "io.h"
#include "thread.h"

static volatile uint32_t ticks = 0;
static uint32_t timer_hz = 100;

void timer_init(uint32_t hz) {
    timer_hz = hz;
    ticks = 0;
    uint32_t divisor = 1193182 / hz;
    outb(0x43, 0x36);                          // Mode 3, lobyte/hibyte, binary
    outb(0x40, (uint8_t)(divisor & 0xFF));     // Low byte
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF)); // High byte
    register_irq_handler(0, timer_irq_handler);
}

void timer_irq_handler(registers_t *regs) {
    (void)regs;
    ticks++;
    thread_tick();
}

uint32_t timer_ticks(void) {
    return ticks;
}

uint32_t timer_ms(void) {
    return ticks * (1000 / (timer_hz ? timer_hz : 100));
}

uint32_t timer_seconds(void) {
    return timer_hz ? ticks / timer_hz : 0;
}
