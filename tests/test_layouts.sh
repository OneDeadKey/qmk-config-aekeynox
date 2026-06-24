#!/usr/bin/env bash

# Layout-axis compile test.
# Compiles the default config for ONE ONEDEADKEY_LAYOUT_* branch, selected by
# -kb + -layout. Catches syntax errors (stray commas, unbalanced parens) in a
# shared/layouts.h branch that the canonical split_3x6_3 board never activates:
# the C preprocessor discards untaken #elif branches before the compiler runs,
# so a broken ortho_*/planck_* branch compiles clean on the default board.
#
# One branch per run. CI's layout matrix fans this out over the board set that
# exercises every branch (see .github/workflows/qmk-test.yml).
#
# Usage:
#   ./test_layouts.sh -kb <keyboard> -layout <LAYOUT> [-target arsenik|selenium] [-j <jobs>]

set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

# Parse arguments
while [ "$#" -gt 0 ]; do
    case "$1" in
        -kb)     shift; KEYBOARD="$1" ;;
        -target) shift; TARGET="$1" ;;
        -layout) shift; LAYOUT_OVERRIDE="$1" ;;
        -j)      shift; JOBS="$1" ;;
        *)       echo "Usage: $0 -kb <keyboard> -layout <LAYOUT> [-target arsenik|selenium] [-j <jobs>]"; exit 1 ;;
    esac
    shift
done

: "${LAYOUT_OVERRIDE:?-layout is required (the branch to compile)}"
setup_env

run_layout() {
    local target="$1"
    log_section "$target — layout compile ($LAYOUT_OVERRIDE on $KEYBOARD)"
    run_compile_test "$target" "$LAYOUT_OVERRIDE defaults" ""
}

TARGET="${TARGET:-all}"

case "$TARGET" in
    arsenik)  run_layout arsenik ;;
    selenium) run_layout selenium ;;
    all)      run_layout arsenik; run_layout selenium ;;
    *)        echo "Unknown target: $TARGET"; exit 1 ;;
esac

print_summary
