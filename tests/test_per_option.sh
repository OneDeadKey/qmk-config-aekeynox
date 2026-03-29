#!/usr/bin/env bash

# Per-option compile tests
# Toggles one option at a time to verify each compiles independently.
#
# Usage:
#   ./test_per_option.sh [-kb <keyboard>] [-target <target>] [-j <jobs>]

set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

# Parse arguments
while [ "$#" -gt 0 ]; do
    case "$1" in
        -kb)     shift; KEYBOARD="$1" ;;
        -target) shift; TARGET="$1" ;;
        -j)      shift; JOBS="$1" ;;
        *)       echo "Usage: $0 [-kb <keyboard>] [-target arsenik|selenium] [-j <jobs>]"; exit 1 ;;
    esac
    shift
done

setup_env

# Helper: enable a #define
enable() { echo "s|^// *#define $1|#define $1|"; }
# Helper: disable a #define
disable() { echo "s|^#define $1|// #define $1|"; }

# ─────────────────────────────────────────────────────────────
# Arsenik per-option
# ─────────────────────────────────────────────────────────────

run_arsenik() {
    log_section "Arsenik — per-option tests"

    run_compile_test arsenik "defaults" ""

    run_compile_test arsenik "lafayette=off" \
        "$(disable ENABLE_SYMBOLS_LAYER)"

    run_compile_test arsenik "hrm=on" \
        "$(enable ENABLE_HRM)"

    run_compile_test arsenik "hrm=on mac=on" \
        "$(enable ENABLE_HRM)" \
        "$(enable MAC_MODIFIERS)"

    run_compile_test arsenik "simple_thumbs=on" \
        "$(enable SIMPLE_THUMBS)"

    run_compile_test arsenik "selenium_variant=on" \
        "$(enable VIM_NAVIGATION)"

    run_compile_test arsenik "selenium_variant=on left_space=on" \
        "$(enable VIM_NAVIGATION)" \
        "$(enable LEFT_HAND_SPACE)"

    run_compile_test arsenik "selenium_variant=on restore_space=on" \
        "$(enable VIM_NAVIGATION)" \
        "$(enable RESTORE_SPACE)"

    # Host layouts
    local host_sed=""
    for layout in QWERTY AZERTY ERGOL BEPO DVORAK COLEMAK WORKMAN; do
        host_sed=""
        for l in QWERTY AZERTY ERGOL BEPO DVORAK COLEMAK WORKMAN; do
            if [ "$l" = "$layout" ]; then
                host_sed+="s|^// *#define ARSENIK_HOST_LAYOUT_$l|#define ARSENIK_HOST_LAYOUT_$l|;"
            else
                host_sed+="s|^#define ARSENIK_HOST_LAYOUT_$l|// #define ARSENIK_HOST_LAYOUT_$l|;"
            fi
        done
        run_compile_test arsenik "host=$layout" "$host_sed"
    done
}

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

    # Host layouts
    local host_sed=""
    for layout in ERGOL AZERTY QWERTY_LAFAYETTE QWERTY; do
        host_sed=""
        for l in ERGOL AZERTY QWERTY_LAFAYETTE; do
            host_sed+="s|^#define KB_LAYOUT_$l|// #define KB_LAYOUT_$l|;"
        done
        if [ "$layout" != "QWERTY" ]; then
            host_sed+="s|^// *#define KB_LAYOUT_$layout|#define KB_LAYOUT_$layout|;"
        fi
        run_compile_test selenium "host=$layout" "$host_sed"
    done
}

# ─────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────

TARGET="${TARGET:-all}"

case "$TARGET" in
    arsenik)  run_arsenik ;;
    selenium) run_selenium ;;
    all)      run_arsenik; run_selenium ;;
    *)        echo "Unknown target: $TARGET"; exit 1 ;;
esac

print_summary
