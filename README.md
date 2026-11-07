<div align="center">
  <img src="assets/thread-logo.svg" width="132" alt="THREAD OS logo" />
  <h1>THREAD OS</h1>
  <p><strong>Your computer. Your rules. Your system.</strong></p>
  <p>A Linux-based x86_64 OS project focused on freedom, customization, productivity, and user control.</p>

  <a href="README.md"><img alt="English" src="https://img.shields.io/badge/🇬🇧_English-0B0D12?style=for-the-badge&logoColor=white"></a>
  <a href="README.ru.md"><img alt="Русский" src="https://img.shields.io/badge/🇷🇺_Русский-0B0D12?style=for-the-badge&logoColor=white"></a>
  <a href="README.de.md"><img alt="Deutsch" src="https://img.shields.io/badge/🇩🇪_Deutsch-0B0D12?style=for-the-badge&logoColor=white"></a>
  <a href="README.az.md"><img alt="Azərbaycan dili" src="https://img.shields.io/badge/🇦🇿_Azərbaycanca-0B0D12?style=for-the-badge&logoColor=white"></a>

  <br><br>
  <a href="#-project-status"><img alt="Project status" src="https://img.shields.io/badge/status-early%20development-7BB7FF?style=for-the-badge"></a>
  <a href="ARCHITECTURE.md"><img alt="Architecture" src="https://img.shields.io/badge/architecture-Linux%20%2B%20THREAD-8AE3C5?style=for-the-badge"></a>
  <a href="LICENSE"><img alt="License: MIT" src="https://img.shields.io/badge/license-MIT-F5B75B?style=for-the-badge"></a>
</div>

---

## ✦ Welcome to THREAD

THREAD OS is an ambitious operating-system project by **Jamil Islamov**. The first release is planned around the **Linux kernel**, **GRUB**, and a custom THREAD experience: desktop, applications, workspace tools, and installer.

> **Honest status:** this repository is an early development foundation—not a finished operating system. There is currently no verified bootable installer image or tested desktop release. The existing ISO script requires a kernel and initramfs payload that are not included yet. No physical-hardware or QEMU boot test has been completed.

## ✧ Explore THREAD

| Get started | Project design | Help build it |
|:--|:--|:--|
| [**⚙ Build guide**](BUILD.md) · [**↗ Install guide**](INSTALL.md) | [**⌘ Architecture**](ARCHITECTURE.md) · [**◷ Roadmap**](ROADMAP.md) | [**✚ Contributing**](CONTRIBUTING.md) · [**☷ Issues**](https://github.com/jamilislamov642-cell/thread-os/issues) |

### Quick links

[🚀 **Build**](BUILD.md) &nbsp; [📦 **Installation**](INSTALL.md) &nbsp; [🧭 **Architecture**](ARCHITECTURE.md) &nbsp; [🗺 **Roadmap**](ROADMAP.md) &nbsp; [🤝 **Contribute**](CONTRIBUTING.md) &nbsp; [🌐 **Website**](https://jynza.cc)

## ◈ Project status

**Stage: early foundation.** The repository has project documentation, a preliminary GRUB configuration, branding, and starter scripts. These are not evidence of a working OS image: the current build script does not assemble a complete live system, and the menu configuration still needs a real kernel/initramfs and corrected live-boot behavior.

| Area | Status |
|:--|:--|
| Linux-based x86_64 architecture | Planned and documented |
| GRUB BIOS + UEFI boot | Configuration started; not boot-tested |
| Kernel and initramfs payload | Not included |
| Complete live root filesystem | Not implemented |
| THREAD desktop and working apps | Planned; not implemented as a complete desktop |
| Graphical installer and disk installation | Not implemented |
| Reproducible, validated `THREAD.iso` | Not available yet |
| QEMU / physical hardware verification | Not performed |

Features in the roadmap are goals, not promises that they already work. We will update this table when implementation and testing provide evidence.

## ⌘ Technical direction

The initial target is **x86_64** with a Linux kernel foundation and GRUB for Legacy BIOS and UEFI. The THREAD identity is intended to come from its own desktop experience, applications, settings, workspace features, visual design, and installation tooling—not from falsely claiming an independently developed kernel.

Planned platform areas include:

- **Boot:** GRUB, BIOS/UEFI paths, recovery and safe-graphics options.
- **System:** Linux drivers, device discovery, storage, networking, accounts, and logs.
- **Desktop:** a dark-first THREAD interface with launcher, panel, settings, notifications, and workspace restore.
- **Applications:** Files, Settings, Terminal, Browser, Store, Notes, Monitor, Network, Security, Update Center, Recovery, and Workspace.
- **Installer:** explicit disk selection, manual/guided partitioning, and confirmation before destructive changes.

See [ARCHITECTURE.md](ARCHITECTURE.md) for the design and [ROADMAP.md](ROADMAP.md) for the staged plan.

## 🛠 Build status and developer setup

The current scripts are **starter tooling**, not a working ISO pipeline. Do not use the generated image (if one is produced) to install a computer or overwrite a USB drive. Required kernel/root-filesystem integration, live boot, installer implementation, and boot tests remain outstanding.

Review [BUILD.md](BUILD.md) for current requirements and limitations. The repository's current script entry points are:

```bash
./scripts/build_iso.sh
./scripts/validate_iso.sh
./scripts/qemu_test.sh
```

Running these does **not** establish that the OS boots. A release must only be called bootable after producing the full image and validating BIOS and UEFI boot in a test environment.

## 🗂 Repository map

```text
bootloader/   GRUB configuration and boot notes
kernel/       Linux kernel strategy and configuration notes
desktop/      THREAD desktop design
applications/ application concepts and implementation notes
system/       service and workspace design
drivers/      hardware support strategy
installer/    installer planning
filesystem/   filesystem and disk layout design
networking/   networking design
security/     security model
themes/       visual theme configuration
assets/       brand assets
scripts/      build and test scripts
tests/        validation notes
documentation/technical documentation
iso/          ISO staging notes
```

## 🧵 Project principles

1. **Real over theatrical:** no mock interface presented as system functionality.
2. **Honest claims:** document what is implemented and what has actually been tested.
3. **User control:** make risky disk and bootloader operations explicit and consent-based.
4. **Maintainable foundations:** build on established Linux components before attempting kernel independence.
5. **Accessible customization:** make the system's appearance and behavior adaptable.

## 🤝 Contribute

Contributions and constructive feedback are welcome. Start with [CONTRIBUTING.md](CONTRIBUTING.md), then browse [open issues](https://github.com/jamilislamov642-cell/thread-os/issues) or [create an issue](https://github.com/jamilislamov642-cell/thread-os/issues/new/choose). Please report test environment and exact results; do not describe an untested feature as working.

## About

**THREAD OS** · Founder: **Jamil Islamov** · [jynza.cc](https://jynza.cc) · [MIT License](LICENSE)

<div align="center">
  <sub>Built with a clear roadmap, open development, and no pretend progress.</sub><br>
  <a href="#-welcome-to-thread">↑ Back to top</a>
</div>
