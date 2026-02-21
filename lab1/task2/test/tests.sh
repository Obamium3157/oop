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
"$PROGRAM" 11 4 2026 > "$TMP_OUT"
if ! grep -q "222002" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "2: Negative"
"$PROGRAM" 5 17 "-304" > "$TMP_OUT"
if ! grep -q -- "-4B" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "3: Zero"
"$PROGRAM" 10 2 0 > "$TMP_OUT"
if ! grep -q "0" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "4: Invalid character"
if "$PROGRAM" 16 10 Z > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "ERROR" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "5: Invalid radix"
if "$PROGRAM" "u" 30 Y > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "ERROR" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "6: Invalid radix ( = 1)"
if "$PROGRAM" 1 10 5 > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "ERROR" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "7: Overflow"
if "$PROGRAM" 10 10 2112398123712897180523123 > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "ERROR" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "8: Wrong args"
if "$PROGRAM" 10 2 > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "ERROR" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "9: radix = 36"
"$PROGRAM" 36 10 HELLO > "$TMP_OUT"
if ! grep -q "29234652" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "10: radix = 2"
"$PROGRAM" 2 11 10011010110101 > "$TMP_OUT"
if ! grep -q "7499" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "11: MAX_INT"
"$PROGRAM" 10 10 2147483647 > "$TMP_OUT"
if ! grep -q "2147483647" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "11: MIN_INT"
"$PROGRAM" 10 10 "-2147483648" > "$TMP_OUT"
if ! grep -q -- "-2147483648" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "11: MAX_INT + 1"
if "$PROGRAM" 10 10 2147483648 > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "ERROR" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "12: MIN_INT - 1"
if "$PROGRAM" 10 10 "-2147483649" > "$TMP_OUT" 2>&1; then fail; fi
if ! grep -q "ERROR" "$TMP_OUT"; then fail; fi
echo "PASSED"

echo "11: MAX_INT to hex"
"$PROGRAM" 10 16 2147483647 > "$TMP_OUT"
if ! grep -q "7FFFFFFF" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"

echo "12: MIN_INT to hex"
"$PROGRAM" 10 16 "-2147483648" > "$TMP_OUT"
if ! grep -q -- "-80000000" "$TMP_OUT" 2>&1; then fail; fi
echo "PASSED"