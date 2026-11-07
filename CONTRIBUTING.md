# Contributing to THREAD OS

Thank you for helping build THREAD OS 1.0.

## Principles

- Keep the project honest about what is implemented and what is planned.
- Do not claim a feature works if it has not been tested.
- Prioritize stability, safety, and reproducibility over visual placeholders.
- Keep the Linux base clearly documented.
- Prefer real implementation and safety checks over fake effects.

## Repository structure

The codebase is organized by subsystem:

- bootloader
- kernel
- desktop
- applications
- system
- drivers
- installer
- filesystem
- networking
- security
- themes
- assets
- scripts
- tests
- documentation
- iso

## Contribution workflow

1. Open an issue for the feature or bug.
2. Create a focused branch.
3. Make small, testable changes.
4. Update relevant documentation.
5. Run validation scripts where applicable.
6. Submit a PR with testing notes.

## Standards

- Use shell script safety patterns (`set -euo pipefail`).
- Keep documentation aligned with actual implementation.
- Make installer actions explicit and user-confirmed.
- Never silently wipe or overwrite a disk.
- State hardware compatibility clearly.

## Security notes

- Installer actions must clearly show the selected disk.
- Existing bootloaders must not be replaced without explicit authorization.
- Recovery and rollback paths must be transparent and safe.
- Updates should be verified whenever the project adds real signed update support.

## Development environment

Use a Linux host with:

- bash
- grub-install
- grub-mkrescue
- xorriso
- dosfstools
- mtools
- qemu-system-x86_64
- a Linux kernel and initramfs build environment

## Questions

See ARCHITECTURE.md and ROADMAP.md for the platform design and rollout plan.
