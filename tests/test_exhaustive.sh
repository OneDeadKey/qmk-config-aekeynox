#!/usr/bin/env bash

# Exhaustive compile tests — reads matrix files
# Tests every valid combination of config options.
#
# Usage:
#   ./test_exhaustive.sh [-kb <keyboard>] [-target <target>] [-layout <LAYOUT>] [-j <jobs>]
#
# -layout is required for multi-layout boards (e.g. crkbd/rev1, beekeeb/piantor).

set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

# Parse arguments
while [ "$#" -gt 0 ]; do
    case "$1" in
        -kb)     shift; KEYBOARD="$1" ;;
        -target) shift; TARGET="$1" ;;
        -layout) shift; LAYOUT_OVERRIDE="$1" ;;
        -j)      shift; JOBS="$1" ;;
        *)       echo "Usage: $0 [-kb <keyboard>] [-target arsenik|selenium] [-layout <LAYOUT>] [-j <jobs>]"; exit 1 ;;
    esac
    shift
done

setup_env

TARGET="${TARGET:-all}"

case "$TARGET" in
    arsenik)
        run_matrix_file arsenik "$TESTS_DIR/matrix_arsenik.txt"
        ;;
    selenium)
        run_matrix_file selenium "$TESTS_DIR/matrix_selenium.txt"
        ;;
    all)
        run_matrix_file arsenik "$TESTS_DIR/matrix_arsenik.txt"
        run_matrix_file selenium "$TESTS_DIR/matrix_selenium.txt"
        ;;
    *)
        echo "Unknown target: $TARGET"; exit 1
        ;;
esac

print_summary
