#!/usr/bin/env bash
set -euo pipefail

# Simple helper to run the emdash embedded binary under QEMU's user mode.
# It builds the project if the executable is missing and then invokes
# qemu-x86_64 to emulate execution.

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BUILD_DIR="$SCRIPT_DIR/../build"

if [ ! -f "$BUILD_DIR/emdash_embedded" ]; then
  cmake -S "$SCRIPT_DIR/.." -B "$BUILD_DIR"
  cmake --build "$BUILD_DIR" --target emdash_embedded
fi

exec qemu-x86_64 "$BUILD_DIR/emdash_embedded"
