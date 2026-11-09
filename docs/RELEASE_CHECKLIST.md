# THREAD OS release checklist

Use this checklist before publishing a bootable package.

1. Build from a clean tree with `make clean && make`.
2. Confirm the bootloader is 512 bytes and the kernel fits the linker limit.
3. Run `./scripts/test.sh` and record every check.
4. Boot the ISO and raw image in QEMU.
5. Capture a desktop screenshot when the UI changes.
6. Create the complete source package without build caches.
7. Write release notes with assets and a quick test command.
8. Verify the release asset names and download links.
9. Push the matching source commit and tag.
10. Update the README current-release buttons.
