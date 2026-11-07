# THREAD Bootloader

The project uses GRUB as the primary bootloader for both BIOS and UEFI boot. This allows the system to start from a USB drive or installed disk and present the THREAD boot menu before kernel initialization.

## Boot flow

1. firmware initializes
2. boot device is selected
3. GRUB loads the kernel and initramfs
4. the boot menu is displayed
5. the selected boot path is launched
6. the system starts the desktop or installer

## Boot menu entries

- Start THREAD OS
- Install THREAD OS
- Safe Graphics Mode
- Recovery Mode
- Memory Diagnostics

## Safety requirements

- no silent disk erase
- no silent bootloader overwrite
- explicit confirmation before installation actions
