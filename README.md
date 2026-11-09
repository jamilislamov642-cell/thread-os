<div align="center">
  <img src="assets/thread-logo.svg" width="128" alt="THREAD OS logo" />
  <h1>THREAD OS</h1>
  <p><strong>your computer your rules your system</strong></p>
  <p>real bootable x86 os · preemptive kernel · kernel rendered desktop</p>

  <a href="https://github.com/jamilislamov642-cell/thread-os/releases/download/v1.2.1/threados-v1.2.1-complete.zip"><img src="https://img.shields.io/badge/DOWNLOAD-PACKAGE-7C5CFF?style=for-the-badge&logo=archive&logoColor=white" alt="Download package"></a>
  <a href="https://github.com/jamilislamov642-cell/thread-os/releases/download/v1.2.1/threados.iso"><img src="https://img.shields.io/badge/BOOT-ISO-00C2FF?style=for-the-badge&logo=linux&logoColor=071018" alt="Boot ISO"></a>
  <a href="https://github.com/jamilislamov642-cell/thread-os/releases/download/v1.2.1/threados.img"><img src="https://img.shields.io/badge/BOOT-USB-00D68F?style=for-the-badge&logo=usb&logoColor=071018" alt="Boot USB image"></a>
  <a href="https://github.com/jamilislamov642-cell/thread-os/releases"><img src="https://img.shields.io/badge/RELEASES-FF4D6D?style=for-the-badge&logo=github&logoColor=white" alt="Releases"></a>

  <br>
  <a href="https://github.com/jamilislamov642-cell/thread-os/issues"><img src="https://img.shields.io/badge/ISSUES-FFB000?style=for-the-badge&logo=github&logoColor=071018" alt="Issues"></a>
  <a href="https://github.com/jamilislamov642-cell/thread-os/pulls"><img src="https://img.shields.io/badge/PULL%20REQUESTS-FF7A45?style=for-the-badge&logo=github&logoColor=white" alt="Pull requests"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/LICENSE-MIT-26C6DA?style=for-the-badge" alt="MIT license"></a>

  <p><a href="README.md">English</a> · <a href="README.ru.md">Русский</a> · <a href="README.de.md">Deutsch</a> · <a href="README.az.md">Azərbaycanca</a></p>
</div>

---

## the vibe

THREAD OS is a small real operating system that boots from BIOS and drops into its own terminal plus a kernel-rendered desktop.

no browser mockup. no fake progress. just a bootloader a kernel a scheduler and a surprisingly colorful VGA screen.

> **latest:** `v1.2.1` · **tests:** `23/23 pass` · **arch:** `i686`

## download the first package

| file | what it is |
|:--|:--|
| [complete package](https://github.com/jamilislamov642-cell/thread-os/releases/download/v1.2.1/threados-v1.2.1-complete.zip) | source docs screenshots binaries and images |
| [bootable ISO](https://github.com/jamilislamov642-cell/thread-os/releases/download/v1.2.1/threados.iso) | QEMU VirtualBox or VMware |
| [USB image](https://github.com/jamilislamov642-cell/thread-os/releases/download/v1.2.1/threados.img) | raw floppy or legacy USB boot |
| [release notes](https://github.com/jamilislamov642-cell/thread-os/releases/tag/v1.2.1) | checksums changelog and details |

## boot it

```bash
# from source
make
./scripts/run.sh

# or just run the release image
qemu-system-i386 -cdrom threados.iso -m 64M
```

then type:

```text
desktop       apps          files
monitor       status        settings
theme next    bench         about
```

## what is inside

- 512 byte MBR bootloader
- A20 setup and 32 bit protected mode
- flat GDT and 256 gate IDT
- PIC remapping and CPU exception handlers
- PIT timer at 100 Hz
- priority round robin preemptive scheduler
- assembly context switching
- thread create sleep yield kill and exit
- 14 MB kernel heap
- VGA text output and PS2 keyboard
- COM1 serial mirror
- mutexes spinlocks and atomic counters
- THREAD Desktop dashboard
- app launcher Files Monitor Status and Settings
- four live color themes
- QEMU test suite with 23 passing checks

## the desktop

<table>
  <tr>
    <td><img src="docs/screenshot_desktop.png" alt="THREAD Desktop" /></td>
    <td><img src="docs/screenshot_monitor.png" alt="THREAD Monitor" /></td>
  </tr>
  <tr>
    <td align="center"><strong>desktop</strong></td>
    <td align="center"><strong>monitor</strong></td>
  </tr>
  <tr>
    <td><img src="docs/screenshot_apps.png" alt="THREAD Apps" /></td>
    <td><img src="docs/screenshot_settings.png" alt="THREAD Settings" /></td>
  </tr>
  <tr>
    <td align="center"><strong>apps</strong></td>
    <td align="center"><strong>settings</strong></td>
  </tr>
</table>

Everything above is rendered by the native kernel into VGA memory and uses live scheduler timer heap and thread data.

## commands worth trying

| command | mood |
|:--|:--|
| `desktop` | go home |
| `apps` | open the launcher |
| `files` | browse the native workspace view |
| `monitor` | watch the kernel breathe |
| `settings` | personalize the system |
| `theme next` | change the entire palette |
| `spawn alpha 1` | start a real worker |
| `threads` | inspect the scheduler |
| `bench` | stress preemption and atomics |
| `matrix` | turn on digital rain |
| `shutdown` | leave cleanly |

## build and verify

```bash
sudo apt-get install gcc nasm qemu-system-x86 xorriso mtools make
sudo apt-get install gcc-i686-linux-gnu
make
./scripts/test.sh
```

The test suite boots the image in QEMU and checks boot handoff desktop startup app views settings themes threads memory concurrency uptime and shutdown.

## status

| now | next |
|:--|:--|
| bootable native kernel | persistent filesystem |
| preemptive multitasking | richer Files operations |
| THREAD Desktop views | mouse and better input navigation |
| live themes and telemetry | framebuffer graphics |
| ISO and USB releases | graphical installer |

## links

[build guide](BUILD.md) · [user guide](docs/USER_GUIDE.md) · [kernel architecture](docs/ARCHITECTURE.md) · [roadmap](ROADMAP.md) · [issues](https://github.com/jamilislamov642-cell/thread-os/issues) · [pull requests](https://github.com/jamilislamov642-cell/thread-os/pulls) · [releases](https://github.com/jamilislamov642-cell/thread-os/releases)

## contribute

read [CONTRIBUTING.md](CONTRIBUTING.md) then build it run the tests and open a focused issue or pull request.

**THREAD OS** · [MIT](LICENSE) · [jynza.cc](https://jynza.cc)

<div align="center"><sub>quietly becoming a real operating system</sub></div>
