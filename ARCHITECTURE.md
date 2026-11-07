# ARCHITECTURE.md

## THREAD OS architecture

## Goals

The first release prioritizes:

- a real bootable ISO,
- a credible Linux-based x86_64 foundation,
- a custom THREAD desktop layer,
- a desktop installer with disk safety checks,
- a reproducible build and testing flow,
- transparent documentation of hardware support and limits.

## Architecture stack

### 1. Boot Layer

- BIOS support via GRUB
- UEFI support via GRUB EFI mode
- boot menu with safe recovery entries
- USB boot support
- SSD/HDD install support

### 2. Kernel Layer

The first release uses a Linux kernel foundation. This is intentionally clear and honest. The custom THREAD layer is built above the Linux kernel rather than pretending to be an independent kernel at release 1.0.

### 3. System Services Layer

- init and session management
- login and account handling
- workspace persistence
- device manager and mount tracking
- system logging and diagnostics
- update and recovery services

### 4. Desktop Environment Layer

The THREAD desktop environment provides:

- taskbar
- app launcher
- desktop icons
- notifications
- quick settings
- file manager
- application windows
- workspace restore prompts
- system settings and power menu

### 5. Application Layer

- THREAD Files
- THREAD Settings
- THREAD Terminal
- THREAD Browser
- THREAD Store
- THREAD Notes
- THREAD Monitor
- THREAD Network
- THREAD Security
- THREAD Update Center
- THREAD Recovery
- THREAD Workspace

### 6. Filesystem Layer

Recommended first-release layout:

- EFI: FAT32
- root: ext4 or Btrfs
- optional swap partition
- optional data partition for user files

### 7. Security Layer

- account-based access control
- permissions model using standard Unix semantics
- firewall integration
- update verification when implemented
- safe installer warnings and confirmations
- clear recovery and diagnostics workflows

### 8. Hardware Support Layer

Supported hardware classes include:

- Intel and AMD x86_64 CPUs
- common Intel/AMD GPUs
- USB keyboards and mice
- SSD/HDD/NVMe storage
- Ethernet and Wi-Fi devices
- audio devices and touchpads where supported by the Linux stack

Unsupported hardware must be shown clearly rather than claimed as working.

## Release direction

THREAD OS 1.0 is a credible first release focused on:

- real bootability,
- real installability,
- custom desktop identity,
- practical driver support from upstream Linux stacks,
- repeatable build and validation.

## Future roadmap

- deeper kernel customization where feasible,
- richer driver support,
- expanded workspace persistence and recovery,
- expanded user-facing system apps.
