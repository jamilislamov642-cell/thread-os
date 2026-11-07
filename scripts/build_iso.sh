#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUTPUT_ISO="${ROOT_DIR}/THREAD.iso"
WORK_DIR="${ROOT_DIR}/iso_root"

mkdir -p "${WORK_DIR}/boot/grub"
mkdir -p "${WORK_DIR}/EFI/BOOT"

if ! command -v grub-mkrescue >/dev/null 2>&1; then
  echo "Error: grub-mkrescue is required but not installed."
  exit 1
fi

if ! command -v xorriso >/dev/null 2>&1; then
  echo "Error: xorriso is required but not installed."
  exit 1
fi

if [ -f "${ROOT_DIR}/kernel/vmlinuz" ]; then
  cp "${ROOT_DIR}/kernel/vmlinuz" "${WORK_DIR}/vmlinuz"
fi

if [ -f "${ROOT_DIR}/kernel/initrd.img" ]; then
  cp "${ROOT_DIR}/kernel/initrd.img" "${WORK_DIR}/initrd.img"
fi

cp "${ROOT_DIR}/bootloader/grub/grub.cfg" "${WORK_DIR}/boot/grub/grub.cfg"
cp "${ROOT_DIR}/assets/thread-logo.svg" "${WORK_DIR}/THREAD-logo.svg"

cat > "${WORK_DIR}/EFI/BOOT/grub.cfg" <<'EOF'
set default=0
set timeout=10
menuentry "Start THREAD OS" {
  linux /vmlinuz root=/dev/sda2 ro quiet splash
  initrd /initrd.img
}
EOF

if [ -f "${WORK_DIR}/vmlinuz" ] && [ -f "${WORK_DIR}/initrd.img" ]; then
  echo "Kernel and initramfs detected; creating a full ISO image."
else
  echo "Warning: no kernel or initramfs present. Creating the staging ISO shell only."
fi

grub-mkrescue -o "${OUTPUT_ISO}" "${WORK_DIR}" -- -volid THREADOS

echo "Built: ${OUTPUT_ISO}"
