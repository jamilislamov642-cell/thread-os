#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 /dev/sdX"
  exit 1
fi

TARGET_DEVICE="$1"

if [ ! -b "${TARGET_DEVICE}" ]; then
  echo "Error: ${TARGET_DEVICE} is not a valid block device."
  exit 1
fi

read -r -p "This will overwrite ${TARGET_DEVICE}. Continue? [y/N] " answer
case "$answer" in
  y|Y|yes|YES)
    echo "Writing THREAD.iso to ${TARGET_DEVICE}..."
    sudo dd if="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/THREAD.iso" of="${TARGET_DEVICE}" bs=4M status=progress conv=fsync
    echo "Done."
    ;;
  *)
    echo "Cancelled."
    exit 1
    ;;
 esac
