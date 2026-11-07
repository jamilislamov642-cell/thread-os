# THREAD drivers and hardware support

This directory documents the driver strategy for mainstream hardware support.

## Priority support classes

- Intel and AMD x86_64 CPUs
- Intel integrated graphics
- AMD graphics
- NVIDIA graphics where the upstream Linux driver is available
- USB keyboards and mice
- touchpads
- Bluetooth devices
- Wi-Fi and Ethernet adapters
- SSD, HDD, and NVMe drives
- audio devices

## Driver policy

- prefer stable upstream Linux drivers
- never claim a device works without driver support or testing evidence
- display unsupported hardware clearly and explicitly
- log hardware compatibility information

## Validation workflow

1. enumerate devices
2. detect hardware class
3. match to a supported driver or mark unsupported
4. log status for diagnostics and support
