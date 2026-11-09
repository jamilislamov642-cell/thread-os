<div align="center">
  <img src="assets/thread-logo.svg" width="148" alt="THREAD OS logo" />
  <h1>THREAD OS</h1>
  <p><strong>Your computer. Your rules. Your system.</strong></p>
  <p>A real bootable x86 operating system with a preemptive kernel and a kernel-rendered desktop.</p>

  <p>
    <a href="https://github.com/jamilislamov642-cell/thread-os/releases/latest"><img src="https://img.shields.io/github/v/release/jamilislamov642-cell/thread-os?display_name=tag&style=for-the-badge&color=8AE3C5&label=latest%20release" alt="Latest release"></a>
    <a href="https://github.com/jamilislamov642-cell/thread-os/actions"><img src="https://img.shields.io/badge/QEMU%20tests-23%2F23%20PASS-4CAF50?style=for-the-badge" alt="QEMU tests passing"></a>
    <a href="LICENSE"><img src="https://img.shields.io/badge/license-MIT-F5B75B?style=for-the-badge" alt="MIT license"></a>
  </p>
  <p>
    <a href="README.md">English</a> ·
    <a href="README.ru.md">Русский</a> ·
    <a href="README.de.md">Deutsch</a> ·
    <a href="README.az.md">Azərbaycanca</a>
  </p>
</div>

---

## The short version

THREAD OS started as a vision for a personal operating system. It is now a **real 32-bit x86 system** that boots from an MBR image or ISO, enters protected mode, configures interrupts, starts a preemptive scheduler, manages memory, accepts keyboard input, and launches a kernel-rendered THREAD Desktop.

It is small, direct, inspectable, and honest about what is implemented.

> **Current release:** `v1.2.0` — Personal Desktop
>
> **Verification:** `23/23` automated QEMU checks pass
>
> **Boot path:** BIOS → MBR bootloader → protected mode → kernel → THREAD Desktop

## What makes it cool

| Layer | What is real today |
|:--|:--|
| Boot | 512-byte stage-1 MBR bootloader with A20 setup and protected-mode handoff |
| Kernel | Freestanding i686 C and assembly kernel with a flat 4 GB memory model |
| Scheduling | PIT-driven preemptive priority round-robin scheduling at 100 Hz |
| Threads | Create, sleep, yield, kill, exit, context switch, and inspect live threads |
| Memory | First-fit kernel heap with coalescing and per-thread stacks |
| Drivers | VGA text console, PS/2 keyboard, COM1 serial mirroring, power controls |
| Desktop | Kernel-drawn dashboard with dock, workspace, taskbar, system cards, and activity panels |
| Apps | Launcher, Files, Monitor, Status Center, Settings, About, Terminal, and Bench |
| Personalization | Four live themes with instant redraw from the shell |
| Proof | Reproducible build plus QEMU test suite with 23 passing assertions |

## See it boot

<table>
  <tr>
    <td width="50%"><img src="docs/screenshot_desktop.png" alt="THREAD Desktop dashboard" /></td>
    <td width="50%"><img src="docs/screenshot_monitor.png" alt="THREAD live monitor" /></td>
  </tr>
  <tr>
    <td align="center"><strong>THREAD Desktop</strong><br><sub>Kernel-rendered workspace with dock and status panels</sub></td>
    <td align="center"><strong>THREAD Monitor</strong><br><sub>Live PIT ticks heap data and thread activity</sub></td>
  </tr>
  <tr>
    <td width="50%"><img src="docs/screenshot_apps.png" alt="THREAD app launcher" /></td>
    <td width="50%"><img src="docs/screenshot_settings.png" alt="THREAD settings screen" /></td>
  </tr>
  <tr>
    <td align="center"><strong>App Launcher</strong><br><sub>Terminal Files Monitor Bench Settings and About</sub></td>
    <td align="center"><strong>Personal Desktop</strong><br><sub>Four themes and system controls</sub></td>
  </tr>
</table>

<details>
<summary>More screenshots</summary>
<br>

<img src="docs/screenshot_files.png" alt="THREAD Files view" />

<img src="docs/screenshot_boot.png" alt="THREAD boot screen" />

<img src="docs/screenshot_threads.png" alt="THREAD running threads and concurrency benchmark" />

</details>

## Try it in three commands

```bash
# Build the bootable image and ISO
make

# Open the VGA desktop in QEMU
./scripts/run.sh

# Run the complete verification suite
./scripts/test.sh
```

To boot the published release without compiling:

```bash
# Download threados.img or threados.iso from the latest release
qemu-system-i386 -fda threados.img -m 64M
# or
qemu-system-i386 -cdrom threados.iso -m 64M
```

The official binaries are published on the [latest GitHub Release](https://github.com/jamilislamov642-cell/thread-os/releases/latest).

## Desktop tour

The native desktop is not an HTML screenshot or a fake launcher. It is drawn directly into VGA memory by the kernel and uses live data from the scheduler, timer, heap allocator, and thread table.

```text
ThreadOS> desktop       # return to the main workspace
ThreadOS> apps          # open the app launcher
ThreadOS> files         # open the Files view
ThreadOS> monitor       # inspect live scheduler and memory telemetry
ThreadOS> status        # open the subsystem health center
ThreadOS> settings      # open themes and system controls
ThreadOS> theme next    # cycle the active theme and redraw the desktop
ThreadOS> open monitor  # app-style navigation
```

The four built-in themes are:

- `Classic White` — clean monochrome workstation
- `Matrix Green` — high-contrast green terminal
- `Solar Cyan` — blue desktop with cyan highlights
- `Amber Terminal` — warm retro console palette

## Shell command reference

| Command | What it does |
|---|---|
| `desktop` / `workspace` | Return to the THREAD Desktop dashboard |
| `apps` / `launcher` | Open the VGA app launcher |
| `files` | Open the THREAD Files view |
| `monitor` | Show uptime, PIT ticks, heap data, and thread activity |
| `status` | Show boot, input, scheduler, memory, and shell health |
| `settings` / `theme` | Open personalization and system controls |
| `theme next` / `color` | Cycle the active desktop theme |
| `open <app>` | Open `desktop`, `files`, `monitor`, or `apps` |
| `threads` / `ps` | List live threads with state, priority, ticks, and switches |
| `spawn <name> [priority]` | Start an asynchronous prime-computing worker |
| `kill <tid>` | Terminate a thread by ID |
| `bench` / `stress` | Run the 60,000-operation preemptive concurrency benchmark |
| `ticker` | Start or stop a background uptime ticker |
| `matrix` | Start or stop the digital-rain demo thread |
| `sleep <ms>` | Sleep the shell while other threads continue running |
| `mem` | Show kernel heap and thread stack statistics |
| `uptime` | Show system uptime and PIT ticks |
| `help` | Print the full command list |
| `about` | Print architecture and build information |
| `reboot` | Reboot the machine |
| `shutdown` | Power off QEMU or a supported virtual machine |

## Build requirements

### Ubuntu or Debian

```bash
sudo apt-get update
sudo apt-get install gcc nasm qemu-system-x86 xorriso mtools make
sudo apt-get install gcc-i686-linux-gnu
```

The cross-compiler is especially useful on ARM64 machines. The Makefile produces:

```text
dist/threados.img    bootable 1.44 MB floppy / USB image
dist/threados.iso    bootable El Torito ISO
dist/boot.bin        512-byte stage-1 bootloader
dist/kernel.bin      flat kernel binary
dist/kernel.elf      kernel with debug symbols
```

## Test it like a kernel developer

```bash
# Build and run the complete functional test suite
./scripts/test.sh

# Run the image in serial mode
./scripts/run.sh --serial

# Boot the ISO
./scripts/run.sh --iso

# Start QEMU paused for GDB
qemu-system-i386 -fda dist/threados.img -m 64M -s -S
```

The test suite boots the image in QEMU and verifies the real boot path, desktop startup, app views, settings, theme cycling, shell, thread creation, background execution, atomic concurrency, memory reporting, uptime, About, and clean shutdown.

## Architecture at a glance

```text
BIOS
  │
  ▼
512-byte MBR bootloader
  │  loads kernel and enters 32-bit protected mode
  ▼
Kernel entry + flat GDT
  │
  ├── IDT + PIC remapping + CPU exceptions
  ├── PIT timer @ 100 Hz
  ├── Priority round-robin scheduler
  ├── Assembly context switcher
  ├── Kernel heap allocator
  ├── VGA / PS2 keyboard / COM1 serial drivers
  ├── Mutexes / spinlocks / atomic counters
  └── THREAD Desktop + shell + app views
```

Read the detailed [native kernel architecture](docs/ARCHITECTURE.md) and the long-term [system architecture](ARCHITECTURE.md).

## Project status

| Area | Status |
|:--|:--|
| BIOS boot and MBR handoff | Implemented and boot-tested |
| Protected mode and flat GDT | Implemented and tested |
| IDT, PIC, CPU exception handling | Implemented and tested |
| PIT timer and preemptive scheduling | Implemented and tested |
| Assembly context switching | Implemented and tested |
| Dynamic heap and thread stacks | Implemented and tested |
| VGA, PS/2 keyboard, serial console | Implemented and tested |
| Thread synchronization and atomics | Implemented and tested |
| THREAD Desktop dashboard | Implemented and tested |
| App launcher, Files, Monitor, Status, Settings | Implemented and tested |
| Four live color themes | Implemented and tested |
| Automated QEMU verification | **23/23 PASS** |
| Bootable image and ISO | Generated by `make` |
| Persistent filesystem driver | Planned |
| Graphical installer | Planned |
| Linux-based x86_64 desktop edition | Planned |

## Roadmap

### Next up

- Persistent disk-backed filesystem and user files
- More complete file operations inside THREAD Files
- Better input navigation for desktop views
- More hardware drivers and device discovery
- Optional graphical framebuffer mode

### Long term

- x86_64 desktop edition with BIOS and UEFI support
- Installer with disk safety checks and recovery tools
- Window manager and graphical application toolkit
- THREAD Files, Settings, Terminal, Browser, Store, Notes, Monitor, and Recovery apps

## Project principles

1. **Real over theatrical.** Every feature claim should be backed by code and a test.
2. **Honest status.** Implemented native features and future desktop goals stay clearly separated.
3. **Small enough to understand.** The boot path should remain inspectable by one person.
4. **User control.** Destructive disk and power operations must be explicit.
5. **Personal by design.** Themes, tools, and workspace behavior should belong to the user.

## Repository map

```text
boot/           MBR bootloader source
kernel/         Native C and assembly kernel
docs/           Architecture, user guide, and screenshots
scripts/        Build, run, USB, validation, and QEMU tests
assets/         THREAD visual identity and logo
desktop/        Long-term desktop design notes
applications/   Application concepts
filesystem/     Filesystem design notes
networking/     Networking design notes
security/       Security model
themes/         Theme configuration
ROADMAP.md      Long-term product roadmap
BUILD.md        Build and validation guide
```

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md), build the image locally, run `./scripts/test.sh`, and keep changes focused. Good first contributions include documentation, tests, shell commands, drivers, and improvements to the native desktop views.

Open an [issue](https://github.com/jamilislamov642-cell/thread-os/issues) or start from the [latest release](https://github.com/jamilislamov642-cell/thread-os/releases/latest).

## Links

[Build guide](BUILD.md) · [User guide](docs/USER_GUIDE.md) · [Native architecture](docs/ARCHITECTURE.md) · [Roadmap](ROADMAP.md) · [Releases](https://github.com/jamilislamov642-cell/thread-os/releases) · [Website](https://jynza.cc)

## About

**THREAD OS** · Founder: **Jamil Islamov** · [jynza.cc](https://jynza.cc) · [MIT License](LICENSE)

<div align="center">
  <sub>Built with open development and no pretend progress.</sub><br>
  <a href="#thread-os">Back to top</a>
</div>
