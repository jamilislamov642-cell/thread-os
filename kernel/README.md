# THREAD kernel strategy

This directory captures the first-release kernel strategy for THREAD OS.

## Why Linux as the base

The project uses a Linux kernel foundation for a realistic and maintainable 1.0 release. This is not a fake claim: the project is intentionally transparent that the kernel base is Linux while the THREAD identity layer is custom.

## Kernel requirements

- x86_64 CPU support
- EFI support
- USB stack support
- NVMe and AHCI support
- ext4 and FAT32 filesystems
- ACPI and device enumeration
- initramfs support
- module loading

## Suggested configuration areas

- CONFIG_X86_64=y
- CONFIG_EFI=y
- CONFIG_EFI_STUB=y
- CONFIG_BLK_DEV_SD=y
- CONFIG_NVME=y
- CONFIG_USB=y
- CONFIG_AHCI=y
- CONFIG_SATA_AHCI=y
- CONFIG_FS_EXT4=y
- CONFIG_VFAT_FS=y
- CONFIG_BTRFS_FS=y
- CONFIG_NET=y
- CONFIG_INPUT_KEYBOARD=y
- CONFIG_INPUT_MOUSE=y
- CONFIG_DRM=y

## Notes

The custom THREAD layer is the desktop, app framework, installer, bootstrap identity, and user workspace model. The kernel underneath remains a standard Linux kernel stack unless future work introduces a deeper independent kernel evolution.
