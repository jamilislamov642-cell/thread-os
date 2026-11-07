# THREAD filesystem strategy

## Filesystem layout

A practical first-release layout uses standard Linux-compatible partitions:

- EFI System Partition: FAT32
- root filesystem: ext4 or Btrfs
- optional swap partition
- optional data partition

## Requirements

- Bootable via GRUB
- Safe install and recovery behavior
- support for USB and internal disk installation
- compatibility with Linux tools and standard install flows

## Recommended directory structure

```text
/
  boot/
  etc/
  home/
  opt/
  tmp/
  usr/
  var/
```
