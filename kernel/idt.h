#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;           // pushed by CPU
} __attribute__((packed)) registers_t;

typedef void (*isr_handler_t)(registers_t *regs);

void idt_init(void);
void gdt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void register_interrupt_handler(uint8_t n, isr_handler_t handler);
void register_irq_handler(uint8_t irq, isr_handler_t handler);
void pic_send_eoi(uint8_t irq);
void isr_handler(registers_t *regs);
void irq_handler(registers_t *regs);

#endif
