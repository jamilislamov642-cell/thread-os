# ThreadOS User Guide

This guide walks you through building, booting, testing, and extending ThreadOS.

---

## 1. Prerequisites

### Ubuntu / Debian (x86_64 or ARM64)

```bash
sudo apt-get update
sudo apt-get install -y gcc nasm qemu-system-x86 xorriso mtools make

# On ARM64 (e.g. Apple Silicon, Raspberry Pi, AWS Graviton) also install the
# i686 cross-compiler:
sudo apt-get install -y gcc-i686-linux-gnu
```

### macOS (Homebrew)

```bash
brew install nasm qemu xorriso
# For cross-compilation on Apple Silicon, use a Docker container (see below)
```

---

## 2. Building

From the repository root:

```bash
make
```

Build outputs in `dist/`:

| Output | Description |
|---|---|
| `threados.img` | 1.44 MB bootable floppy/raw drive image |
| `threados.iso` | Bootable El Torito ISO (works in any VM) |
| `boot.bin` | 512-byte MBR bootloader |
| `kernel.bin` | Flat kernel binary |
| `kernel.elf` | Kernel with debug symbols (for GDB) |

Clean build:

```bash
make clean && make
```

---

## 3. Running

Use the launcher script or the Makefile targets:

```bash
# GUI mode (VGA display in a QEMU window)
./scripts/run.sh
# or: make run

# Headless serial console (Ctrl-A then X to exit)
./scripts/run.sh --serial
# or: make run-serial

# Boot from the ISO image
./scripts/run.sh --iso
# or: make run-iso

# VNC mode (connect a VNC viewer to localhost:5901)
./scripts/run.sh --vnc
```

The shell and all OS output appear both in the VGA window and over COM1 serial, so you can interact through whichever you prefer.

---

## 4. Automated Testing

ThreadOS includes a self-verifying test suite that boots the OS inside QEMU, exercises every shell command through the serial console, and asserts correct kernel behavior:

```bash
./scripts/test.sh
```

Expected output:

```
[TEST] Booting ThreadOS in QEMU and executing commands...
=== VERIFICATION RESULTS ===
  [PASS] BIOS handoff
  [PASS] MBR bootloader
  [PASS] Kernel startup banner
  [PASS] Protected-mode shell
  [PASS] Help command
  [PASS] Thread table listing
  [PASS] Thread spawn
  [PASS] Second thread spawn
  [PASS] Background thread execution
  [PASS] Atomic concurrency counting
  [PASS] Concurrency benchmark passed
  [PASS] Memory management command
  [PASS] Uptime command
  [PASS] About command
  [PASS] Clean shutdown
[TEST] All verification checks PASSED!
```

---

## 5. Shell Command Reference

| Command | Usage | Description |
|---|---|---|
| `help` | `help` | Lists all commands with short descriptions |
| `threads` / `ps` | `threads` | Live thread table (TID, name, state, priority, ticks, switches) |
| `spawn` | `spawn <name> [priority 0-3]` | Launches an asynchronous worker thread that computes primes |
| `kill` | `kill <tid>` | Terminates a thread by its ID |
| `bench` / `stress` | `bench` | 3 worker threads atomically increment a shared counter 60,000 times to verify true preemptive concurrency |
| `ticker` | `ticker` | Toggles persistent background thread printing uptime every second |
| `matrix` | `matrix` | Toggles digital rain demo thread in the background |
| `sleep` | `sleep [ms]` | Puts the shell thread to sleep; other threads keep running |
| `mem` | `mem` | Displays heap total, used, free, and thread stack statistics |
| `uptime` | `uptime` | Prints seconds and h:m:s since boot |
| `color` | `color` | Cycles VGA color themes |
| `clear` | `clear` | Clears the VGA screen |
| `about` | `about` | System information, build details, and architecture |
| `reboot` | `reboot` | Resets the CPU |
| `shutdown` | `shutdown` | Powers off the VM (QEMU/VirtualBox ACPI) |

**Try this demo sequence** after boot:

```text
ThreadOS> help
ThreadOS> spawn alpha
ThreadOS> spawn beta 2
ThreadOS> threads
ThreadOS> sleep 3000
ThreadOS> bench
ThreadOS> mem
ThreadOS> uptime
ThreadOS> shutdown
```

Notice how `alpha` and `beta` print asynchronously while you keep typing, and how `sleep` lets background work continue unimpeded.

---

## 6. Writing Your First Kernel Thread

Every thread is just a C function with signature `void fn(void *arg)`. The scheduler handles stack allocation, context switching, and cleanup automatically.

### Example: Fibonacci worker thread

Edit `kernel/main.c` (or add a new shell command in `kernel/shell.c`):

```c
#include "thread.h"
#include "vga.h"

static void fib_worker(void *arg) {
    (void)arg;
    uint32_t a = 0, b = 1;
    for (int i = 0; i < 30; i++) {
        vga_printf("[fib] F(%d) = %u\n", i, a);
        uint32_t c = a + b;
        a = b;
        b = c;
        thread_sleep(500);   // yield CPU and sleep 500 ms
    }
    vga_printf("[fib] done.\n");
    // Thread exits automatically when fn returns
}
```

Launch it anywhere in the kernel:

```c
// name, entry function, argument, priority (0=high, 1=normal, 2=low, 3=idle)
thread_create("fibonacci", fib_worker, NULL, 1);
```

### Thread API quick reference

| Function | Description |
|---|---|
| `int thread_create(const char *name, thread_entry_fn fn, void *arg, int priority)` | Creates and enqueues a new thread. Returns TID on success, -1 on failure. |
| `void thread_yield(void)` | Voluntarily gives up the remainder of the current time slice. |
| `void thread_sleep(uint32_t ms)` | Puts the calling thread to sleep for `ms` milliseconds. |
| `void thread_exit(void)` | Terminates the calling thread immediately. |
| `int thread_kill(int tid)` | Terminates the specified thread by TID. Returns 0 on success. |
| `thread_t *thread_current(void)` | Returns a pointer to the calling thread's control block. |

---

## 7. Synchronization Primitives

For shared data accessed by multiple threads:

```c
#include "sync.h"

static spinlock_t lock;
static volatile uint32_t counter = 0;

void worker(void *arg) {
    (void)arg;
    spinlock_init(&lock);          // initialize once
    spinlock_acquire(&lock);       // acquires or yields while contended
    counter++;
    spinlock_release(&lock);
}

// Or use lock-free atomic counters directly:
atomic_inc(&counter);
atomic_add(&counter, 10);
```

Mutexes (`mutex_t`, `mutex_init`, `mutex_lock`, `mutex_unlock`) are also available with owner-TID tracking.

---

## 8. Booting on Real Hardware

1. Insert a USB flash drive and identify its device node (e.g. `/dev/sdX`):

   ```bash
   lsblk
   ```

2. Write the image directly to the drive (**replace `/dev/sdX` with your device**; this will overwrite it):

   ```bash
   sudo dd if=dist/threados.img of=/dev/sdX bs=512 status=progress conv=fdatasync
   ```

3. Reboot your PC, enter the BIOS/UEFI boot menu, and select the USB drive in **Legacy / CSM boot mode** (ThreadOS uses the classic MBR boot path).

---

## 9. Booting in Virtual Machines

### VirtualBox
1. Create a new VM: Type "Other", Version "Other/Unknown (32-bit)", RAM: 64 MB.
2. In Storage settings, either:
   - Attach `dist/threados.iso` as the Optical Drive, or
   - Add a Floppy Controller and attach `dist/threados.img`.
3. Start the VM.

### VMware Workstation / Player
1. Create a Custom VM, Guest OS: "Other" 32-bit.
2. Mount `dist/threados.iso` as CD/DVD drive.
3. Power on.

### QEMU (command line)

```bash
# ISO
qemu-system-i386 -cdrom dist/threados.iso -m 64M -serial mon:stdio

# Floppy image
qemu-system-i386 -fda dist/threados.img -m 64M -serial mon:stdio

# Raw hard-disk-style drive
qemu-system-i386 -drive file=dist/threados.img,format=raw -m 64M
```

---

## 10. Debugging with GDB

ThreadOS preserves debug symbols in `dist/kernel.elf`. To single-step the kernel:

```bash
# Terminal 1: start QEMU waiting for GDB
qemu-system-i386 -fda dist/threados.img -m 64M -s -S -serial mon:stdio

# Terminal 2: attach GDB
gdb dist/kernel.elf
(gdb) target remote localhost:1234
(gdb) break kernel_main
(gdb) continue
```

---

## 11. Troubleshooting

| Symptom | Cause & Fix |
|---|---|
| `Boot failed: could not read the boot disk` in QEMU | Ensure you passed `-fda dist/threados.img` or `-cdrom dist/threados.iso` |
| Image says `No bootable device` | Rebuild images with `make clean && make`; verify file sizes with `ls -la dist/` |
| Nothing prints in terminal | Run `make run` for GUI mode, or add `-serial mon:stdio` for serial |
| Keystrokes echoed twice in serial mode | Normal: input is printed both by the shell echo and by serial loopback in some terminal emulators |
| Build error `i686-linux-gnu-gcc: command not found` | Install the cross-compiler: `sudo apt-get install gcc-i686-linux-gnu` |
