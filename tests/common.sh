#!/usr/bin/env bash

# Shared test functions for QMK compile tests
# Source this file from test scripts.

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
QMK_DIR="$(cd "$TESTS_DIR/.." && pwd)"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Counters
PASS=0
FAIL=0
SKIP=0
FAILED_TESTS=()

log_info()    { echo -e "${BLUE}[TEST]${NC} $1"; }
log_pass()    { echo -e "${GREEN}[PASS]${NC} $1"; PASS=$((PASS + 1)); }
log_fail()    { echo -e "${RED}[FAIL]${NC} $1"; FAIL=$((FAIL + 1)); FAILED_TESTS+=("$1"); }
log_skip()    { echo -e "${YELLOW}[SKIP]${NC} $1"; SKIP=$((SKIP + 1)); }
log_section() { echo -e "\n${CYAN}═══ $1 ═══${NC}"; }

# Recent milc versions (qmk's CLI framework) annotate values with a trailing
# provenance marker like " (config)", " (env)", " (default)"; strip any such suffix.
qmk_config_value() {
    qmk config "$1" | awk -F= '{print $2}' | sed -E 's/ \([a-z_]+\)$//' | xargs
}

# ─────────────────────────────────────────────────────────────
# Environment setup
# ─────────────────────────────────────────────────────────────

setup_env() {
    KEYBOARD="${KEYBOARD:-}"
    JOBS="${JOBS:-0}"

    if [ -z "$KEYBOARD" ]; then
        KEYBOARD=$(qmk_config_value user.keyboard)
    fi

    if [ -z "${QMK_HOME+x}" ]; then
        QMK_HOME=$(qmk_config_value user.qmk_home)
        # Expand $HOME if envsubst is available, otherwise use eval
        if command -v envsubst > /dev/null 2>&1; then
            QMK_HOME=$(echo "$QMK_HOME" | envsubst)
        else
            QMK_HOME=$(eval echo "$QMK_HOME")
        fi
    fi

    log_info "Keyboard: ${CYAN}$KEYBOARD${NC}"
    log_info "QMK home: ${CYAN}$QMK_HOME${NC}"
    log_info "Jobs: ${CYAN}$JOBS${NC}"
}

# ─────────────────────────────────────────────────────────────
# Helpers: generate and patch a keymap
# ─────────────────────────────────────────────────────────────

# Generate a keymap, patch options.h, and install to QMK_HOME
# Sets KEYMAP_NAME for use by callers
# $1 = target (arsenik|selenium)
# $2... = sed expressions to patch options.h
_generate_and_install() {
    local target="$1"
    shift
    local sed_exprs=("$@")

    KEYMAP_NAME="test_${target}"
    local gen_dir="$QMK_DIR/output/$KEYBOARD/keymaps/$target"
    local output_dir="$QMK_DIR/output/$KEYBOARD/keymaps/$KEYMAP_NAME"
    local dest_dir="$QMK_HOME/keyboards/$KEYBOARD/keymaps/$KEYMAP_NAME"

    # Generate (generator outputs relative to cwd, so run from QMK_DIR)
    rm -rf "$output_dir" "$gen_dir"
    (cd "$QMK_DIR" && bash generator.sh -src "./$target" -kb "$KEYBOARD" > /dev/null 2>&1)
    mv "$gen_dir" "$output_dir"

    # Patch options.h
    for expr in "${sed_exprs[@]}"; do
        [ -n "$expr" ] && sed -i "$expr" "$output_dir/options.h"
    done

    # Copy to QMK
    rm -rf "$dest_dir"
    mkdir -p "$(dirname "$dest_dir")"
    cp -r "$output_dir" "$dest_dir"
}

# Cleanup generated and installed files
_cleanup() {
    local target="$1"
    local output_dir="$QMK_DIR/output/$KEYBOARD/keymaps/test_${target}"
    local dest_dir="$QMK_HOME/keyboards/$KEYBOARD/keymaps/test_${target}"
    rm -rf "$dest_dir" "$output_dir"
}

# ─────────────────────────────────────────────────────────────
# Core: compile
# ─────────────────────────────────────────────────────────────

# Run a compile test with sed expressions
# $1 = target (arsenik|selenium)
# $2 = test name
# $3... = sed expressions to patch options.h
run_compile_test() {
    local target="$1"
    local test_name="$2"
    shift 2
    local sed_exprs=("$@")

    _generate_and_install "$target" "${sed_exprs[@]}"

    if (cd "$QMK_HOME" && qmk compile -kb "$KEYBOARD" -km "$KEYMAP_NAME" -j "$JOBS" > /dev/null 2>&1); then
        log_pass "$target: $test_name"
    else
        log_fail "$target: $test_name"
    fi

    _cleanup "$target"
}

# ─────────────────────────────────────────────────────────────
# Matrix file runner
# ─────────────────────────────────────────────────────────────

# Run a compile test from a matrix line
# $1 = target
# $2 = test name
# $3 = space-separated list of +DEFINE / -DEFINE directives
run_matrix_test() {
    local target="$1"
    local test_name="$2"
    local directives="$3"

    local sed_exprs=()
    for directive in $directives; do
        local prefix="${directive:0:1}"
        local define="${directive:1}"
        case "$prefix" in
            +) sed_exprs+=("s|^// *#define $define|#define $define|") ;;
            -) sed_exprs+=("s|^#define $define|// #define $define|") ;;
            *) echo "Invalid directive: $directive (must start with + or -)"; return 1 ;;
        esac
    done

    if [ ${#sed_exprs[@]} -eq 0 ]; then
        sed_exprs=("")
    fi

    run_compile_test "$target" "$test_name" "${sed_exprs[@]}"
}

# Parse and run all compile tests from a matrix file
# $1 = target
# $2 = matrix file path
run_matrix_file() {
    local target="$1"
    local matrix_file="$2"

    if [ ! -f "$matrix_file" ]; then
        echo "Matrix file not found: $matrix_file"
        return 1
    fi

    log_section "$target — exhaustive ($(basename "$matrix_file"))"

    while IFS='|' read -r name directives; do
        # Skip comments and empty lines
        [[ "$name" =~ ^[[:space:]]*# ]] && continue
        [[ -z "${name// /}" ]] && continue

        # Trim whitespace
        name="$(echo "$name" | xargs)"
        directives="$(echo "$directives" | xargs)"

        run_matrix_test "$target" "$name" "$directives"
    done < "$matrix_file"
}

# ─────────────────────────────────────────────────────────────
# Summary
# ─────────────────────────────────────────────────────────────

print_summary() {
    log_section "Results"
    echo -e "${GREEN}Passed: $PASS${NC}  ${RED}Failed: $FAIL${NC}  ${YELLOW}Skipped: $SKIP${NC}"
    if [ ${#FAILED_TESTS[@]} -gt 0 ]; then
        echo -e "\n${RED}Failed tests:${NC}"
        for t in "${FAILED_TESTS[@]}"; do
            echo -e "  ${RED}✗${NC} $t"
        done
        return 1
    fi
}
