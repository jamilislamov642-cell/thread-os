#!/usr/bin/env bash
# =============================================================================
# ThreadOS - Automated Verification Test Suite
# Boots the OS in QEMU, exercises all shell commands via serial console,
# and verifies expected multithreading behavior and boot success.
# =============================================================================
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$SCRIPT_DIR/.."
cd "$ROOT"

OUT="/tmp/threados_test.log"
IMG="dist/threados.img"

echo "[TEST] Building ThreadOS..."
make -s

echo "[TEST] Booting ThreadOS in QEMU and executing commands..."
python3 - <<'EOF'
import subprocess, time, sys

proc = subprocess.Popen(
    ['qemu-system-i386', '-fda', 'dist/threados.img', '-m', '64M', '-nographic', '-serial', 'mon:stdio', '-no-reboot'],
    stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
)

def send(cmd, wait=1.2):
    proc.stdin.write(cmd + '\n')
    proc.stdin.flush()
    time.sleep(wait)

time.sleep(3)
send('help', 1.0)
send('apps', 0.5)
send('files', 0.5)
send('monitor', 0.5)
send('status', 0.5)
send('desktop', 0.5)
send('threads', 1.0)
send('spawn alpha', 0.5)
send('spawn beta 2', 0.5)
send('threads', 2.0)
send('bench', 3.0)
send('mem', 1.0)
send('uptime', 1.0)
send('about', 1.0)
send('shutdown', 2.0)

try:
    stdout, _ = proc.communicate(timeout=8)
except subprocess.TimeoutExpired:
    proc.kill()
    stdout, _ = proc.communicate()

with open('/tmp/threados_test.log', 'w') as f:
    f.write(stdout)

# Assertions
checks = [
    ('SeaBIOS', 'BIOS handoff'),
    ('Loading ThreadOS kernel', 'MBR bootloader'),
    ('[ThreadOS] THREAD Desktop ready', 'THREAD Desktop startup'),
    ('[desktop] app launcher ready', 'App launcher view'),
    ('[desktop] files view ready', 'Files view'),
    ('[desktop] live monitor ready', 'Monitor view'),
    ('[desktop] status center ready', 'Status center'),
    ('[desktop] workspace dashboard ready', 'Dashboard view'),
    ('ThreadOS>', 'Protected-mode shell prompt'),
    ('Available ThreadOS commands', 'Help command'),
    ('kernel_main', 'Thread table listing'),
    ('Spawned thread \'alpha\'', 'Thread spawn'),
    ('Spawned thread \'beta\'', 'Second thread spawn'),
    ('computing prime set', 'Background thread execution'),
    ('Shared counter final value : 60000', 'Atomic concurrency counting'),
    ('RESULT: SUCCESS', 'Concurrency benchmark passed'),
    ('Kernel heap total', 'Memory management command'),
    ('uptime', 'Uptime command'),
    ('Architecture : x86 Protected Mode', 'About command'),
    ('Shutting down ThreadOS', 'Clean shutdown'),
]

all_ok = True
print("\n=== VERIFICATION RESULTS ===")
for pattern, label in checks:
    if pattern in stdout:
        print(f"  [PASS] {label}")
    else:
        print(f"  [FAIL] {label}")
        all_ok = False

sys.exit(0 if all_ok else 1)
EOF

echo ""
echo "[TEST] All verification checks PASSED!"
