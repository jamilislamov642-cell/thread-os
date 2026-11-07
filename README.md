# THREAD OS

THREAD OS 1.0 is a Linux-based x86_64 operating system platform designed for productivity, privacy, and user control. This repository contains the foundation for a real, installable operating system using a Linux kernel base, a GRUB bootloader for BIOS and UEFI, a custom desktop identity layer, an installer workflow, and a reproducible ISO build pipeline.

## Project identity

Name: THREAD OS
Developer and Founder: Jamil Islamov
Website: https://jynza.cc
Architecture: x86_64
Bootloader: GRUB 2
Primary goal: Freedom, customization, productivity, and user control.

## What this repository includes

- Bootloader configuration and menu flow
- Kernel strategy and hardware notes
- Desktop environment plan
- Native application architecture
- System services and workspace persistence strategy
- Installer workflow and disk safety rules
- Build, validation, and QEMU testing scripts
- ISO generation pipeline and USB imaging instructions

## Build target

The project is designed to produce a real bootable ISO named:

```text
THREAD.iso
```

The ISO is intended for:
- Legacy BIOS boot
- UEFI boot
- USB flashing
- SSD/HDD installation
- VM testing with QEMU

## Repository layout

- bootloader/ — GRUB bootloader configuration
- kernel/ — Linux kernel strategy and config notes
- desktop/ — desktop environment architecture
- applications/ — native THREAD apps
- system/ — system services and session management
- drivers/ — hardware support and driver strategy
- installer/ — installer workflow and boot safety design
- filesystem/ — filesystem strategy
- networking/ — network services and UI design
- security/ — user, permissions, and update policy
- themes/ — design system and theme configuration
- assets/ — brand assets and logo
- scripts/ — build and validation automation
- tests/ — validation and smoke tests
- documentation/ — technical docs and hardware notes
- iso/ — output staging and ISO guidance

## Why this is a realistic first release

This project intentionally chooses a Linux kernel foundation for the first 1.0 release. That is the most maintainable way to deliver a real, bootable, installable operating system in a single project while keeping a custom THREAD identity around the Linux base. The custom THREAD layer includes:

- custom desktop environment
- custom branding
- application layer
- installer flow
- system services and workspace handling
- release and update model

## Quick start

From the repository root:

```bash
./scripts/build_iso.sh
./scripts/validate_iso.sh
./scripts/qemu_test.sh
```

## Installation

See INSTALL.md for the installation instructions and USB creation flow.

## Build guide

See BUILD.md for environment requirements and the ISO build process.

## Architecture guide

See ARCHITECTURE.md for the system design and module breakdown.

## Roadmap

See ROADMAP.md for the planned development phases.

## Licensing

This project is licensed under the MIT License.

## Developer and founder

Jamil Islamov
https://jynza.cc
