#include "vga.h"
#include "serial.h"
#include "idt.h"
#include "timer.h"
#include "kbd.h"
#include "mem.h"
#include "thread.h"
#include "shell.h"
#include "io.h"

// Heap starts at 2 MB, size 14 MB (memory 0x200000..0x1000000)
#define HEAP_START 0x00200000u
#define HEAP_SIZE  (14u * 1024u * 1024u)

void kernel_main(void) {
    // 1. Text & serial consoles
    serial_init();
    vga_init();

    // 2. CPU descriptors & interrupts
    gdt_init();
    idt_init();

    // 3. Dynamic memory allocator
    kmem_init(HEAP_START, HEAP_SIZE);

    // 4. PS/2 keyboard driver
    kbd_init();

    // 5. Multithreading subsystem (main thread + idle thread)
    thread_init();

    // 6. PIT timer @ 100 Hz (10 ms ticks)
    timer_init(100);

    // 7. Enable hardware interrupts and preemptive scheduling
    sti();
    thread_enable_preemption();

    // 8. Enter interactive shell (runs on thread 0)
    shell_init();
    shell_run();

    // Should never reach here
    for (;;) hlt();
}
