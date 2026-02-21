#!/usr/bin/env bash

set -u

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PROGRAM="$PROJECT_ROOT/cmake-build-debug/invert"
TESTDATA="$SCRIPT_DIR/testdata"

TMPDIR="${TMPDIR:-/tmp}"
TMP_IN="$TMPDIR/invert_in.txt"
TMP_OUT="$TMPDIR/invert_out.txt"
TMP_EXPECTED="$TMPDIR/invert_expected.txt"
TMP_NORM_OUT="$TMPDIR/invert_out.norm.txt"
TMP_NORM_EXPECTED="$TMPDIR/invert_expected.norm.txt"

cleanup() {
  rm -f "$TMP_IN" "$TMP_OUT" "$TMP_EXPECTED" \
        "$TMP_NORM_OUT" "$TMP_NORM_EXPECTED" 2>/dev/null || true
}
trap cleanup EXIT

fail() {
  echo "TEST FAILED"
  echo "=== Output ==="
  [ -f "$TMP_OUT" ] && sed -n '1,200p' "$TMP_OUT" || true
  echo "=============="
  exit 1
}

info() {
  printf '%s\n' "$1"
}

normalize_file() {
  local src="$1" dst="$2"
  awk '{ sub(/\r$/, ""); print }' "$src" > "$dst".part
  if [ -s "$dst".part ]; then
    if [ "$(tail -c1 "$dst".part || echo x)" != $'\n' ]; then
      printf '\n' >> "$dst".part
    fi
  fi
  mv "$dst".part "$dst"
}

cmp_norm() {
  normalize_file "$TMP_EXPECTED" "$TMP_NORM_EXPECTED"
  normalize_file "$TMP_OUT" "$TMP_NORM_OUT"
  if ! cmp -s "$TMP_NORM_EXPECTED" "$TMP_NORM_OUT"; then
    diff -u "$TMP_NORM_EXPECTED" "$TMP_NORM_OUT" || true
    return 1
  fi
  return 0
}

run_allow_fail() {
  "$@" || true
}

if [ ! -x "$PROGRAM" ]; then
  echo "Program not found or not executable: $PROGRAM"
  exit 2
fi

info "Using program: $PROGRAM"
cleanup


# STDIN MODE

info "-1: 1-Matrix"
cat > "$TMP_IN" <<EOF
1	1	1
1	1	1
1	1	1
EOF

echo "Non-invertible" > "$TMP_EXPECTED"

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

info "0: 0-Matrix"
cat > "$TMP_IN" <<EOF
0	0	0
0	0	0
0	0	0
EOF

echo "Non-invertible" > "$TMP_EXPECTED"

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

info "1: Example 1 (stdin)"
cat > "$TMP_IN" <<EOF
1	2	3
0	1	4
5	6	0
EOF

cat > "$TMP_EXPECTED" <<EOF
-24.000	18.000	5.000
20.000	-15.000	-4.000
-5.000	4.000	1.000
EOF

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

info "2: Example 2 (stdin)"
cat > "$TMP_IN" <<EOF
4	7	2.3
2	1	1
3	-2	-2.31
EOF

cat > "$TMP_EXPECTED" <<EOF
-0.009	0.321	0.131
0.212	-0.448	0.017
-0.194	0.806	-0.278
EOF

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

info "3: Example 3 (Non-invertible)"
cat > "$TMP_IN" <<EOF
1	2	3
2	4	6
1	2	3
EOF

echo "Non-invertible" > "$TMP_EXPECTED"

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

info "4: Example 4 (Invalid matrix)"
cat > "$TMP_IN" <<EOF
1	2	a
2	4	6
1	2	3
EOF

echo "Invalid matrix" > "$TMP_EXPECTED"

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

# NEGATIVE STDIN TESTS

info "5: Too few rows"
printf "1\t2\t3\n" > "$TMP_IN"
echo "Invalid matrix format" > "$TMP_EXPECTED"

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

info "6: Too few columns"
printf "1\t2\n3\t4\t5\n6\t7\t8\n" > "$TMP_IN"
echo "Invalid matrix format" > "$TMP_EXPECTED"

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

info "7: Empty input"
: > "$TMP_IN"
echo "Invalid matrix format" > "$TMP_EXPECTED"

"$PROGRAM" < "$TMP_IN" > "$TMP_OUT"
cmp_norm || fail
info "PASSED"

# FILE MODE

info "8: File mode valid"
if [ -f "$TESTDATA/01_input.txt" ] && [ -f "$TESTDATA/01_out.txt" ]; then
  "$PROGRAM" "$TESTDATA/01_input.txt" > "$TMP_OUT"
  cp "$TESTDATA/01_out.txt" "$TMP_EXPECTED"
  cmp_norm || fail
  info "PASSED"
else
  echo "8: SKIPPED (missing testdata files)"
fi

info "9: Non-existent file"
run_allow_fail "$PROGRAM" "$TESTDATA/no_such_file.txt" > "$TMP_OUT" 2>&1
echo "Invalid matrix format" > "$TMP_EXPECTED"
cmp_norm || fail
info "PASSED"

info "10: No permission file"
TMP_FORBIDDEN="$TMPDIR/forbidden.txt"
echo "1	2	3" > "$TMP_FORBIDDEN"
chmod 000 "$TMP_FORBIDDEN"

run_allow_fail "$PROGRAM" "$TMP_FORBIDDEN" > "$TMP_OUT" 2>&1
echo "Invalid matrix format" > "$TMP_EXPECTED"

chmod 644 "$TMP_FORBIDDEN"
rm -f "$TMP_FORBIDDEN"

cmp_norm || fail
info "PASSED"

# HELP MODE

info "11: Help mode"
run_allow_fail "$PROGRAM" -h > "$TMP_OUT" 2>&1
if ! grep -q "Usage:" "$TMP_OUT"; then
  echo "Help text missing"
  fail
fi
info "PASSED"

cleanup
info "All tests completed."
exit 0