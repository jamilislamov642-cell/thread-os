# ThreadOS Architecture

## Overview

ThreadOS is a 32-bit x86 protected-mode multitasking operating system designed to transform the classical simulated "ThreadOS" concept (the Java-based OS simulator commonly used in computer science courses such as CSS 430) into a **genuine, bootable, bare-metal operating system** with real CPU context switching, hardware interrupt handling, preemptive scheduling, and an interactive shell.

```
                  +-----------------------------------+
                  |        Interactive Shell          |
                  |     (commands, live thread table) |
                  +-----------------------------------+
                                    |
                  +-----------------------------------+
                  |    Thread Subsystem & Scheduler    |
                  |   (priority RR, quantum, context   |
                  |     switch, yield, sleep, exit)   |
                  +-----------------------------------+
                   |         |         |           |
             +-----+   +-----+   +-----+-----+   +-----+-----+
             |         |         |           |   |           |
          +-----+   +-----+   +-----+     +-----+   +-----+  |
          | GDT |   | IDT |   | PIT |     | PS2 |   | VGA |  |
          |     |   |     |   |Timer|     | Kbd |   |Text |  |
          +-----+   +-----+   +-----+     +-----+   +-----+  |
                                                               |
                  +-----------------------------------+  |
                  |     Heap Memory Manager (14 MB)   |--+
                  +-----------------------------------+
                                    |
                  +-----------------------------------+
                  |         x86 Hardware (CPU)        |
                  +-----------------------------------+
```

---

## 1. Boot Sequence

1. **BIOS** power-on self-test selects the boot device (floppy, hard disk, or El Torito CD-ROM).
2. BIOS loads the first sector (512 bytes, MBR) to memory address **`0x7C00`** and jumps to it with `DL` set to the boot drive number.
3. The **Stage 1 bootloader** (`boot/boot.asm`):
   - Sets up real-mode segment registers (`DS = ES = SS = 0`, `SP = 0x7C00`).
   - Clears the screen via `INT 10h (AH=00h, AL=03h)` 80x25 text mode.
   - Reads the kernel sectors from the boot drive into **`0x10000`** (linear) using BIOS `INT 13h (AH=02h)` with CHS arithmetic and 5-retries-per-sector disk reset.
   - Enables the **A20 gate** (Fast A20 port 0x92 + BIOS INT 15h fallback) so addresses above 1 MB are addressable.
   - Loads a temporary GDT (flat 4 GB code and data segments).
   - Enables Protected Mode (`CR0 |= 1`).
   - Executes a far jump (`jmp 0x08:pm_entry`) to flush the instruction pipeline and reload `CS`.
4. The **Stage 2 kernel entry** (`kernel/entry.asm`):
   - Reloads data segment registers (`DS = ES = FS = GS = SS = 0x10`).
   - Sets the kernel stack pointer to `0x90000`.
   - Clears flags.
   - Calls **`kernel_main()`** in `kernel/main.c`.

---

## 2. Memory Layout

```
0x0000_0000 +----------------------------------+  0 MB
            | Real-mode IVT / BIOS Data Area    |
0x0000_7C00 +----------------------------------+
            | MBR Bootloader (512 bytes)        |
0x0001_0000 +----------------------------------+  64 KB
            | Kernel code + rodata + data + bss |
0x0008_0000 +----------------------------------+
            | Kernel stack (grows down, 64 KB)  |
0x0009_0000 +----------------------------------+  Stack top (0x90000)
            | EBDA / reserved                   |
0x000A_0000 +----------------------------------+
            | Video RAM (VGA 0xB8000 text mode) |
0x0010_0000 +----------------------------------+  1 MB
            | Reserved / kernel identity        |
0x0020_0000 +----------------------------------+  2 MB
            |                                  |
            |   Kernel Heap (14 MB)            |
            |   - dynamic allocations (kmalloc)|
            |   - thread stacks (8 KB each)    |
            |                                  |
0x0100_0000 +----------------------------------+  16 MB
            | Free extended memory             |
```

The heap allocator is a linked-list first-fit block allocator (`kernel/mem.c`) with 8-byte alignment, block splitting on allocation, and block coalescing on `kfree()`. Each thread stack (8 KB) is allocated from the heap.

---

## 3. CPU Descriptors & Interrupts

### Global Descriptor Table (GDT)
Three flat segments defined in `kernel/idt.c`:
- **0x00**: Null descriptor (required).
- **0x08**: Kernel Code segment - base 0, limit 4 GB, execute/read, ring 0, 4 KB granularity, 32-bit.
- **0x10**: Kernel Data segment - base 0, limit 4 GB, read/write, ring 0, 4 KB granularity, 32-bit.

### Interrupt Descriptor Table (IDT)
- 256 gates with type `0x8E` (32-bit interrupt gate, present, DPL 0). Interrupt gates **automatically disable interrupts** (`IF = 0`) on entry, preventing nested interrupt races.
- **Gates 0-31**: CPU exception stubs (Division by Zero, GPF, Page Fault, Double Fault, etc.) defined in `kernel/isr.asm`. Any exception dumps CPU registers and halts the machine with a descriptive message.
- **Gates 32-47**: Hardware IRQs 0-15 remapped via 8259 PIC to INT 32-47 (Master PIC 0x20..0x27, Slave PIC 0x28..0x2F).

### Interrupt Service Routine Dispatch
```
Hardware INT --> ISR stub (push int_no, err_code)
             --> irq_common_stub (pusha, push ds, set ds=0x10)
             --> send PIC EOI
             --> call irq_handler(registers_t *regs)
             --> pop ds, popa, add esp 8, iret
```

---

## 4. Timer & Keyboard Drivers

- **PIT 8254 Timer (`kernel/timer.c`)**: programmed in mode 3 (square wave) at **100 Hz** (10 ms tick), divisor = 1193182 / 100 = 11932. Each IRQ0 increments a `ticks` counter, wakes sleeping threads whose sleep deadline has passed, and triggers the preemptive scheduler tick.
- **PS/2 Keyboard (`kernel/kbd.c`)**: IRQ1 reads scancodes from port `0x60`, decodes set-1 scancodes (supporting Shift and CapsLock), and enqueues printable ASCII characters into a 128-byte ring buffer. `kbd_getc_blocking()` yields CPU to other threads while the buffer is empty, meaning typing waits do not freeze background work.
- **COM1 Serial Port (`kernel/serial.c`)**: 16550 UART at port `0x3F8` configured for 115,200 baud, 8-N-1. All console output is mirrored to serial, and keystrokes arriving on serial are fed into the keyboard ring buffer. This makes headless operation, CI testing, and SSH-redirected serial sessions possible.
- **VGA Text Driver (`kernel/vga.c`)**: memory-mapped at `0xB8000`, 80 columns x 25 rows, hardware cursor via CRT controller ports `0x3D4/0x3D5`, color attributes, backspace and tab handling, automatic line scrolling, and `vga_printf` formatted printing (`%s`, `%d`, `%u`, `%x`, `%c`, `%p`, `%%`).

---

## 5. Multithreading Subsystem

The multithreading implementation (`kernel/thread.c`, `kernel/context.asm`) provides:

### Thread Control Block (`thread_t`)
| Field | Type | Description |
|---|---|---|
| `tid` | `int` | Unique thread identifier (0 = kernel main) |
| `name` | `char[20]` | Human-readable name |
| `state` | `thread_state_t` | FREE, READY, RUNNING, SLEEPING, BLOCKED, DEAD |
| `esp` | `uint32_t *` | Saved stack pointer during context switch |
| `stack_base`, `stack_size` | pointer, size | 8 KB stack memory (heap-allocated) |
| `sleep_until` | `uint32_t` | Wake-up tick count for `thread_sleep()` |
| `runtime_ticks` | `uint32_t` | Total CPU ticks consumed |
| `switches` | `uint32_t` | Number of times this thread was switched in |
| `priority` | `int` | 0 (highest) to 3 (idle) |
| `quantum` | `int` | Remaining ticks in current time slice |

### Thread States & Transitions
```
                thread_create()
                     |
                     v
             +--------------+
      +----->|    READY     |<------+
      |      +--------------+       |
      |             |               |
      |             | pick_next_    |
      |             | thread()      |
      |             v               |
      |      +--------------+       |
      |      |   RUNNING    |-------+  (quantum expired, thread_yield)
      |      +--------------+
      |       |      |     |
      |       |      |     | thread_exit() / thread_kill()
      |       |      |     v
      |       |      |   +--------------+
      |       |      |   |     DEAD     |  (slot & stack reused)
      |       |      |   +--------------+
      |       |      |
      |       |      | thread_sleep(ms)
      |       |      v
      |       |  +--------------+
      |       |  |   SLEEPING   |
      |       |  +--------------+
      |       |          |
      |       +----------+  (timer ticks >= sleep_until)
      |
      |  (wait on blocked primitive)
      +--------------------------------- BLOCKED
```

### Context Switch Mechanics
Context switching is performed by `switch_to(uint32_t **prev_esp, uint32_t *next_esp)` in `kernel/context.asm`:

```nasm
switch_to:
    push ebx
    push esi
    push edi
    push ebp
    pushfd
    mov eax, [esp + 24]   ; prev_esp
    mov edx, [esp + 28]   ; next_esp
    mov [eax], esp        ; save old stack pointer
    mov esp, edx          ; restore new stack pointer
    popfd
    pop ebp
    pop edi
    pop esi
    pop ebx
    ret
```

For **newly created threads**, the stack is pre-loaded with a synthetic frame whose `ret` address points to `thread_entry_wrapper`:

```nasm
thread_entry_wrapper:
    sti                   ; enable interrupts in new thread context
    push esi              ; push thread argument
    call ebx              ; call entry function fn(arg)
    add esp, 4
    call thread_exit      ; terminate thread if fn returns
.halt_loop:
    hlt
    jmp .halt_loop
```

Initial stack frame layout (stack grows downwards):

```
(high address)
[top - 4]   = thread_entry_wrapper   <-- popped by 'ret'
[top - 8]   = entry_function (EBX)   <-- popped by 'pop ebx'
[top - 12]  = argument (ESI)         <-- popped by 'pop esi'
[top - 16]  = 0 (EDI)                <-- popped by 'pop edi'
[top - 20]  = 0 (EBP)                <-- popped by 'pop ebp'
[top - 24]  = 0x202 EFLAGS (IF=1)    <-- popped by 'popfd' (thread->esp)
(low address)
```

### Scheduling Algorithm
- **Priority round-robin**: priorities 0 (highest) through 3 (idle). Within each priority, threads are scheduled round-robin with a rotating cursor `rr_cursor` to ensure fairness.
- **Time slice quantum**: priority 0 gets 8 ticks (80 ms), priority 1 gets 5 ticks (50 ms), priority 2 gets 3 ticks (30 ms), priority 3 gets 1 tick (10 ms).
- **Preemption**: the PIT interrupt decrements the running thread's quantum; when it reaches zero, `thread_schedule()` selects the next ready thread and context-switches immediately. PIC EOI is sent before the switch so subsequent timer ticks are not blocked.
- **Idle thread**: an always-ready thread at priority 3 executes `for (;;) hlt;` so the CPU enters low power whenever no real thread is runnable.

### Synchronization
- **Spinlocks** (`kernel/sync.c`) using compiler built-in atomic test-and-set with acquire/release semantics. Waiting threads yield instead of spinning uselessly.
- **Mutexes** with owner tracking and yield-on-contention.
- **Atomic counters** (`atomic_inc`, `atomic_add`) using `__atomic_add_fetch` for lock-free multi-thread counters.

---

## 6. Interactive Shell

The shell (`kernel/shell.c`) runs on **Thread 0 (`kernel_main`)** and never blocks the system: keyboard input yields while idle, and all shell commands coexist with background threads. Commands include:

| Command | Description |
|---|---|
| `help` | List available commands |
| `threads` / `ps` | Live formatted table of all threads with TID, name, state, priority, runtime ticks, and context switches |
| `spawn <name> [prio]` | Launch a background worker computing primes asynchronously |
| `kill <tid>` | Terminate a thread by ID |
| `bench` / `stress` | Run preemptive multithreading concurrency benchmark |
| `ticker` | Toggle persistent uptime ticker thread |
| `matrix` | Toggle digital rain animation thread |
| `sleep <ms>` | Put shell thread to sleep (demonstrating sleep queue) |
| `mem` | Kernel heap and thread stack statistics |
| `uptime` | System uptime in seconds and h:m:s |
| `color` | Cycle VGA color themes |
| `clear` | Clear screen |
| `about` | System information and architecture |
| `reboot` | Reset CPU via PS/2 controller / triple fault |
| `shutdown` | VM power-off via QEMU/VirtualBox ACPI ports |

---

## 7. Artifact Summary

| File | Size (approx) | Purpose |
|---|---|---|
| `dist/boot.bin` | 512 bytes | Stage 1 MBR bootloader |
| `dist/kernel.elf` | ~25 KB | Kernel ELF with debug symbols |
| `dist/kernel.bin` | ~25.5 KB | Flat kernel binary loaded at 0x10000 |
| `dist/threados.img` | 1.44 MB | Bootable floppy / USB image |
| `dist/threados.iso` | 3.3 MB | Bootable El Torito ISO (CD/DVD/VM) |
