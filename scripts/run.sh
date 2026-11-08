#!/usr/bin/env bash
# =============================================================================
# ThreadOS - Launcher Script
# Usage:
#   ./scripts/run.sh          # QEMU GUI (VGA window)
#   ./scripts/run.sh --serial # headless serial console (mon:stdio)
#   ./scripts/run.sh --iso    # boot from the ISO image
#   ./scripts/run.sh --vnc    # VNC display on :1 (port 5901)
# =============================================================================
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$SCRIPT_DIR/.."
cd "$ROOT"

IMG="dist/threados.img"
ISO="dist/threados.iso"

if [ ! -f "$IMG" ] || [ ! -f "$ISO" ]; then
    echo "[ThreadOS] Building disk images first..."
    make -s
fi

MODE="${1:-}"

case "$MODE" in
    --serial|-s)
        echo "[ThreadOS] Starting QEMU (serial console, Ctrl-A then X to exit)..."
        exec qemu-system-i386 -fda "$IMG" -m 64M -nographic -serial mon:stdio -no-reboot
        ;;
    --iso|-i)
        echo "[ThreadOS] Booting from ISO image..."
        exec qemu-system-i386 -cdrom "$ISO" -m 64M -serial mon:stdio
        ;;
    --vnc)
        echo "[ThreadOS] Starting with VNC on :1 (connect to localhost:5901)..."
        exec qemu-system-i386 -fda "$IMG" -m 64M -display vnc=:1 -serial stdio -no-reboot
        ;;
    *)
        echo "[ThreadOS] Starting QEMU with VGA GUI..."
        exec qemu-system-i386 -fda "$IMG" -m 64M -serial mon:stdio
        ;;
esac
