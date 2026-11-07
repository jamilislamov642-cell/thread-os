# THREAD kernel configuration

A practical first-release configuration includes:

- x86_64 support
- EFI boot support
- ACPI support
- AHCI and NVMe support
- USB host controller support
- FAT32 and ext4 filesystem support
- input device support
- graphics stack support for Intel/AMD/NVIDIA where available

## Example config shortlist

```text
CONFIG_X86_64=y
CONFIG_EFI=y
CONFIG_EFI_STUB=y
CONFIG_BLK_DEV_SD=y
CONFIG_NVME=y
CONFIG_USB=y
CONFIG_USB_XHCI_HCD=y
CONFIG_AHCI=y
CONFIG_SATA_AHCI=y
CONFIG_FS_EXT4=y
CONFIG_VFAT_FS=y
CONFIG_BTRFS_FS=y
CONFIG_NET=y
CONFIG_DRM=y
CONFIG_DRM_I915=y
CONFIG_DRM_AMDGPU=y
CONFIG_DRM_NOUVEAU=y
```
