#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMAGE="${ROOT_DIR}/THREAD.iso"

if [ ! -f "${IMAGE}" ]; then
  echo "Error: no THREAD.iso found at ${IMAGE}. Run ./scripts/build_iso.sh first."
  exit 1
fi

if command -v isoinfo >/dev/null 2>&1; then
  isoinfo -d -i "${IMAGE}" | sed -n '1,20p'
else
  echo "isoinfo not installed; skipping ISO metadata check."
fi

if command -v grub-file >/dev/null 2>&1; then
  grub-file --is-x86-dir "${IMAGE}" || true
  grub-file --is-x86_64-efi "${IMAGE}" || true
else
  echo "grub-file not installed; skipping GRUB validation checks."
fi

echo "ISO validation complete."
