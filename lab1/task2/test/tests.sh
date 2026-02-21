#!/usr/bin/env bash

set -eu

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

PROGRAM="$PROJECT_ROOT/cmake-build-debug/radix"
TMPDIR="${TMPDIR:-/tmp}"
TMP_OUT="$TMPDIR/replace_out.txt"

cleanup() {
  rm -f "$TMP_OUT" 2>/dev/null || true
}
trap cleanup EXIT

fail() {
  echo "TEST FAILED"
  echo "==========================="
  cleanup
  exit 1
}

echo "1: Basic"
"$PROGRAM" 2 10 1011 > "$TMP_OUT"
if ! grep -q "11" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "2: Negative"
"$PROGRAM" 2 10 "-101" > "$TMP_OUT"
if ! grep -q -- "-5" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "3: Zero"
"$PROGRAM" 10 2 0 > "$TMP_OUT"
if ! grep -q "^0$" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "4: Invalid character"
if "$PROGRAM" 16 10 G > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "^ERROR$" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "5: Invalid radix"
if "$PROGRAM" 1 10 5 > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "^ERROR$" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "6: Overflow"
if "$PROGRAM" 10 10 2147483648 > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "^ERROR$" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "7: Wrong args"
if "$PROGRAM" 10 2 > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "^ERROR$" "$TMP_OUT"; then fail; fi
echo "PASSED"