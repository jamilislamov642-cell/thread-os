# THREAD boot process

## Boot flow

1. system power-on
2. firmware loads boot device
3. GRUB loads kernel and initramfs
4. THREAD boot menu is displayed
5. system chooses desktop or installer
6. service layer initializes
7. desktop environment renders

## Recovery modes

- safe graphics mode
- recovery mode
- memory diagnostics

## Safety notes

The installer must never silently format a disk or overwrite a bootloader without explicit user consent.
