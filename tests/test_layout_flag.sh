#!/usr/bin/env bash

# Smoke tests for generator.sh -layout flag.
# These tests check only the generator's own plumbing (argument parsing,
# normalization, warning). They do NOT invoke qmk compile.
#
# Usage:
#   ./test_layout_flag.sh [-kb <keyboard>]

set -uo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$TESTS_DIR/common.sh"

# CLI — accept the superset of flags used across the test suite so run_all.sh
# can forward "$@" uniformly. -target, -layout and -j are irrelevant here
# (each test below passes its own -layout explicitly) and ignored.
while [ "$#" -gt 0 ]; do
    case "$1" in
        -kb)     shift; KEYBOARD="$1" ;;
        -target) shift ;;  # accepted for parity, unused
        -layout) shift ;;  # accepted for parity, unused
        -j)      shift ;;  # accepted for parity, unused
        *)       echo "Usage: $0 [-kb <keyboard>]"; exit 1 ;;
    esac
    shift
done

setup_env

log_section "generator.sh — -layout flag"

TARGET="selenium"
OUTPUT_DIR="$REPO_DIR/output/$KEYBOARD/keymaps/$TARGET"
EXPECTED_DEFINE="#define ONEDEADKEY_LAYOUT_split_3x6_3"

# Run the generator with a -layout argument and assert the generated
# config.h contains the expected ONEDEADKEY_LAYOUT_<...> define.
# $1 = test name, $2 = layout argument, $3 = expected define (optional)
_run_layout_test() {
    local name="$1"
    local layout_arg="$2"
    local expected="${3:-$EXPECTED_DEFINE}"

    rm -rf "$OUTPUT_DIR"
    if ! (cd "$REPO_DIR" && bash generator.sh -src "./$TARGET" -kb "$KEYBOARD" -layout "$layout_arg" > /dev/null 2>&1); then
        log_fail "$name (generator exited non-zero)"
        return
    fi
    if ! grep -qF "$expected" "$OUTPUT_DIR/config.h" 2>/dev/null; then
        log_fail "$name (expected '$expected' in config.h)"
        return
    fi
    log_pass "$name"
}

# Happy path: three input forms all normalize to ONEDEADKEY_LAYOUT_split_3x6_3
_run_layout_test "bare form (split_3x6_3)"                    "split_3x6_3"
_run_layout_test "LAYOUT_ prefix (LAYOUT_split_3x6_3)"        "LAYOUT_split_3x6_3"
_run_layout_test "full prefix (ONEDEADKEY_LAYOUT_split_3x6_3)" "ONEDEADKEY_LAYOUT_split_3x6_3"

# Unknown-layout path: generator must fail-fast (exit non-zero) with a clear
# error message and must not produce config.h.
rm -rf "$OUTPUT_DIR"
err_output=$(cd "$REPO_DIR" && bash generator.sh -src "./$TARGET" -kb "$KEYBOARD" -layout "LAYOUT_does_not_exist" 2>&1)
err_status=$?
if [ "$err_status" -eq 0 ]; then
    log_fail "unknown layout fails fast (generator exited 0, expected non-zero)"
elif ! echo "$err_output" | grep -q "is not supported by shared/layouts.h"; then
    log_fail "unknown layout fails fast (expected 'is not supported' in error output)"
elif [ -f "$OUTPUT_DIR/config.h" ]; then
    log_fail "unknown layout fails fast (config.h was written despite the error)"
else
    log_pass "unknown layout fails fast"
fi

rm -rf "$OUTPUT_DIR"

print_summary
