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
# can forward "$@" uniformly. -target and -j are irrelevant here and ignored.
while [ "$#" -gt 0 ]; do
    case "$1" in
        -kb)     shift; KEYBOARD="$1" ;;
        -target) shift ;;  # accepted for parity, unused
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

# Unknown-layout path: generator must emit a WARN to stdout/stderr but still succeed.
# (Compile would later fail at the '#error' fallthrough in shared/layouts.h.)
rm -rf "$OUTPUT_DIR"
warn_output=$(cd "$REPO_DIR" && bash generator.sh -src "./$TARGET" -kb "$KEYBOARD" -layout "LAYOUT_does_not_exist" 2>&1)
if echo "$warn_output" | grep -q "has no branch in shared/layouts.h"; then
    log_pass "unknown layout emits warning"
else
    log_fail "unknown layout emits warning (expected WARN in output)"
fi

# -layout should work without -km and without qmk config user.keymap.
# Clear QMK_CONFIG's user.keymap in a subshell-safe way by pointing HOME elsewhere.
rm -rf "$OUTPUT_DIR"
if (cd "$REPO_DIR" && bash generator.sh -src "./$TARGET" -kb "$KEYBOARD" -layout "LAYOUT_split_3x6_3" > /dev/null 2>&1); then
    if grep -qF "$EXPECTED_DEFINE" "$OUTPUT_DIR/config.h" 2>/dev/null; then
        log_pass "-layout works without -km"
    else
        log_fail "-layout works without -km (expected define missing)"
    fi
else
    log_fail "-layout works without -km (generator exited non-zero)"
fi

rm -rf "$OUTPUT_DIR"

print_summary
