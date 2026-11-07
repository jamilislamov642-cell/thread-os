# BUILD.md

## Overview

THREAD OS is designed to produce a bootable ISO image for x86_64 systems. The initial release uses a Linux kernel base and GRUB for both BIOS and UEFI booting. The custom THREAD identity layer provides the desktop, installer, app ecosystem, and workspace experience.

## Host requirements

A Linux build host is required. Install:

- bash
- grub2 / grub-pc-bin
- grub-efi-amd64-bin
- xorriso
- mtools
- dosfstools
- qemu-system-x86_64
- a Linux kernel image and initramfs

Optional but useful:

- debootstrap
- arch-install-scripts
- genfstab
- live-build tools

## Build flow

From the repository root:

```bash
./scripts/build_iso.sh
```

This script:

- creates a clean staging directory,
- copies boot metadata and GRUB config,
- uses GRUB to create the ISO image,
- writes an output image named `THREAD.iso`.

## Output

The final image is written to:

```text
./THREAD.iso
```

## Validation

After building the image:

```bash
./scripts/validate_iso.sh
```

This checks for the existence of the ISO and runs lightweight validation commands if available.

## QEMU testing

Test the generated image in a VM:

```bash
./scripts/qemu_test.sh
```

## USB creation

Flash the ISO to a USB drive:

```bash
./scripts/usb_create.sh /dev/sdX
```

Replace `/dev/sdX` with the actual USB target device.

## Notes

This project intentionally documents the use of a Linux kernel base. That makes the system bootable and realistic. The THREAD layer is still the custom experience built around it.
