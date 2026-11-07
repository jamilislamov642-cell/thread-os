#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMAGE="${ROOT_DIR}/THREAD.iso"

if [ ! -f "${IMAGE}" ]; then
  echo "Error: no THREAD.iso found at ${IMAGE}."
  exit 1
fi

if ! command -v qemu-system-x86_64 >/dev/null 2>&1; then
  echo "Warning: qemu-system-x86_64 is not installed; skipping launcher."
  exit 0
fi

qemu-system-x86_64 \
  -m 2048 \
  -drive file="${IMAGE}",format=raw,media=cdrom \
  -boot d \
  -display none
