#!/usr/bin/env bash

# Per-option compile tests
# Toggles one option at a time to verify each compiles independently.
#
# Usage:
#   ./test_per_option.sh [-kb <keyboard>] [-target <target>] [-layout <LAYOUT>] [-j <jobs>]
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
        *)       echo "Usage: $0 [-kb <keyboard>] [-target selenium] [-layout <LAYOUT>] [-j <jobs>]"; exit 1 ;;
    esac
    shift
done

setup_env

# Helper: enable a #define
enable() { echo "s|^// *#define $1|#define $1|"; }
# Helper: disable a #define
disable() { echo "s|^#define $1|// #define $1|"; }

# ─────────────────────────────────────────────────────────────
# Selenium per-option
# ─────────────────────────────────────────────────────────────

run_selenium() {
    log_section "Selenium — per-option tests"

    run_compile_test selenium "defaults" ""

    # Hold-tap configs
    for ht in HT_NONE HT_THUMB_TAPS HT_HOME_ROW_MODS HT_TWO_THUMB_KEYS; do
        local ht_sed=""
        for h in HT_NONE HT_THUMB_TAPS HT_HOME_ROW_MODS HT_TWO_THUMB_KEYS; do
            ht_sed+="s|^#define $h|// #define $h|;"
        done
        ht_sed+="s|^// *#define $ht|#define $ht|;"
        run_compile_test selenium "ht=$ht" "$ht_sed"
    done

    run_compile_test selenium "vim_nav=on" \
        "$(enable VIM_NAVIGATION)"

    run_compile_test selenium "hrm_shift=on" \
        "$(enable HRM_SHIFT)"

    run_compile_test selenium "left_space=on" \
        "$(enable LEFT_HAND_SPACE)"

    run_compile_test selenium "macos=on" \
        "$(enable MACOS)"

    run_compile_test selenium "mod_holds=on" \
        "$(enable ENABLE_MOD_HOLDS)"

    run_compile_test selenium "mod_holds=on vim=on left_space=on" \
        "$(enable ENABLE_MOD_HOLDS)" \
        "$(enable VIM_NAVIGATION)" \
        "$(enable LEFT_HAND_SPACE)"

    # Host layouts
    local host_sed=""
    for layout in ERGOL ERGLACE AZERTY BEPO QWERTY_LAFAYETTE QWERTY; do
        host_sed=""
        for l in ERGOL ERGLACE AZERTY BEPO QWERTY_LAFAYETTE QWERTY; do
            host_sed+="s|^#define KB_LAYOUT_$l|// #define KB_LAYOUT_$l|;"
        done
        host_sed+="s|^// *#define KB_LAYOUT_$layout|#define KB_LAYOUT_$layout|;"
        run_compile_test selenium "host=$layout" "$host_sed"
    done
}

# ─────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────

TARGET="${TARGET:-all}"

case "$TARGET" in
    selenium) run_selenium ;;
    all)      run_selenium ;;
    *)        echo "Unknown target: $TARGET"; exit 1 ;;
esac

print_summary
