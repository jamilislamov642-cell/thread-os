# INSTALL.md

## THREAD OS installation guide

## Download or build

1. Download the generated `THREAD.iso` file or build it locally from this repository.
2. Write it to a USB flash drive using Rufus, balenaEtcher, or a Linux imaging tool.
3. Boot the target machine from the USB device.

## Boot menu

The THREAD boot menu includes:

- Start THREAD OS
- Install THREAD OS
- Safe Graphics Mode
- Recovery Mode
- Memory Diagnostics

## Installation workflow

1. Welcome screen
2. Language selection
3. Keyboard layout choice
4. Hardware compatibility check
5. Disk detection
6. Installation destination selection
7. Partitioning and mount point setup
8. Bootloader installation
9. User account creation
10. Timezone selection
11. System installation
12. Installation verification
13. Restart

## Safety requirements

- The target disk must be clearly identified before any install action.
- Users must explicitly confirm any erase or format action.
- Existing bootloaders must not be overwritten without authorization.
- The installed system must boot without the USB drive.

## Recommended disk layout

- EFI System Partition: FAT32, 512 MB or more
- Root partition: ext4 or Btrfs
- Swap: optional

## Automatic and manual partitioning

The installer supports:

- automatic partitioning
- guided manual partitioning
- preservation of existing data when explicitly selected by the user

## USB creation on Linux

```bash
sudo dd if=./THREAD.iso of=/dev/sdX bs=4M status=progress conv=fsync
```

Replace `/dev/sdX` with the actual USB device.

## Troubleshooting

If boot fails:

- verify BIOS or UEFI boot mode,
- confirm the USB is selected in the firmware boot menu,
- check that the installation target has an EFI System Partition,
- ensure the bootloader is installed to the correct drive.
