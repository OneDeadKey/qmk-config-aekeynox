#!/usr/bin/env bash

# QMK keymap generator
# Assembles a keymap from a source folder + shared headers, ready to compile/flash.
# Outputs files in ./output/<keyboard_model>/keymaps/<target_name>

set -euo pipefail

# Colors for logs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

log_info()    { echo -e "${BLUE}[INFO]${NC} $1"    >&2; }
log_warn()    { echo -e "${YELLOW}[WARN]${NC} $1"  >&2; }
log_success() { echo -e "${GREEN}[SUCCESS]${NC} $1" >&2; }
log_error()   { echo -e "${RED}[ERROR]${NC} $1"    >&2; }

# Resolve the directory where this script lives
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Print usage and exit
usage() {
    echo "Usage: $0 -src <keymap_folder> [-kb <keyboard_model>] [-layout <LAYOUT>] [-name <target_name>] [--copy|-cp]"
    echo "  -src <keymap_folder>   Path to the keymap source folder (required)"
    echo "  -kb <keyboard_model>   Keyboard model (optional, falls back to qmk config user.keyboard)"
    echo "  -layout <LAYOUT>       Explicit LAYOUT macro override (e.g. LAYOUT_split_3x6_3_ex2)"
    echo "  -name <target_name>    Name for the generated keymap (default: source folder name)"
    echo "  --copy, -cp            Copy output to \$QMK_HOME/keyboards/<keyboard_model>/keymaps/<target_name>"
    exit 1
}

# Parse arguments
copy_keymap=false
keyboard_model=""
layout_override=""
src_path=""
target_name=""

while [ "$#" -gt 0 ]; do
    case "$1" in
    -src)
        shift
        src_path="$1"
        ;;
    -kb)
        shift
        keyboard_model="$1"
        ;;
    -layout)
        shift
        layout_override="$1"
        ;;
    -name)
        shift
        target_name="$1"
        ;;
    --copy | -cp)
        copy_keymap=true
        ;;
    *)
        usage
        ;;
    esac
    shift
done

# Validate source path
if [ -z "$src_path" ]; then
    log_error "Missing required argument: -src <keymap_folder>"
    usage
fi

if [ ! -d "$src_path" ]; then
    log_error "Source folder '${CYAN}$src_path${NC}' does not exist."
    exit 1
fi

# Default target name to source folder name
if [ -z "$target_name" ]; then
    target_name="$(basename "$(cd "$src_path" && pwd)")"
fi

if [ -z "$keyboard_model" ]; then
    log_info "Requirements: it is recommended to have set QMK user.keyboard and user.qmk_home using 'qmk config'."
    log_info "See https://docs.qmk.fm/cli_configuration#setting-user-defaults for more information."
    log_info "Your current configuration:"
    qmk config || true
fi

# Recent milc versions (qmk's CLI framework) annotate values with a trailing
# provenance marker like " (config)", " (env)", " (default)"; strip any such suffix.
qmk_config_value() {
    qmk config "$1" | awk -F= '{print $2}' | sed -E 's/ \([a-z_]+\)$//' | xargs
}

# If keyboard_model is empty, get from qmk config
if [ -z "$keyboard_model" ]; then
    keyboard_model=$(qmk_config_value user.keyboard)
    log_info "No keyboard_model provided with '-kb' argument, using value from qmk config: ${CYAN}$keyboard_model${NC}"
fi

# Set QMK_HOME if not already set
if [ -z "${QMK_HOME+x}" ]; then
    # Mirror qmk's own expansion: literal $HOME or leading ~ → absolute path.
    QMK_HOME=$(qmk_config_value user.qmk_home)
    QMK_HOME="${QMK_HOME//\$HOME/$HOME}"
    QMK_HOME="${QMK_HOME/#\~/$HOME}"
fi

# Validate QMK_HOME early if we're going to copy
if [ "$copy_keymap" = true ] && { [ -z "$QMK_HOME" ] || [ ! -d "$QMK_HOME" ]; }; then
    log_error "QMK_HOME is empty or not a directory: '${QMK_HOME}'"
    log_info "Set it via 'qmk config user.qmk_home=<path>' or the QMK_HOME env var."
    exit 1
fi

# Normalize a `-layout arbitrary_name` entered by the user into the ONEDEADKEY_LAYOUT_* form.
# Accepts bare ("split_3x6_3"), LAYOUT_* and ONEDEADKEY_LAYOUT_* inputs.
normalize_layout() {
    local layout="$1"
    if [[ "$layout" == ONEDEADKEY_LAYOUT_* ]]; then
        echo "$layout"
    elif [[ "$layout" == LAYOUT_* ]]; then
        echo "ONEDEADKEY_$layout"
    else
        echo "ONEDEADKEY_LAYOUT_$layout"
    fi
}

# List ONEDEADKEY_LAYOUT_* layouts supported by shared/layouts.h.
# Declarations use either `defined FOO` or `defined(FOO)` (parenthesised form
# appears inside OR-guards for keyboard families like Lily58 / Sofle).
get_supported_layouts() {
    grep -oE 'defined[[:space:](]+ONEDEADKEY_LAYOUT_[A-Za-z0-9_]+' "$SCRIPT_DIR/shared/layouts.h" \
        | sed -E 's/defined[[:space:](]+//' \
        | sort -u
}

# Resolve the ONEDEADKEY_LAYOUT_* name to target for $keyboard_model.
# Source of truth: QMK's own keyboard metadata (`qmk info -f json`) — the
# `layouts` field declares every physical layout the board exposes. We
# intersect that set with layouts supported by shared/layouts.h.
detect_onedeadkey_layout_name() {
    local keyboard_model="$1"
    local override="${2:-}"

    if [ -n "$override" ]; then
        local layout supported
        layout=$(normalize_layout "$override")
        supported=$(get_supported_layouts)
        if ! echo "$supported" | grep -qx "$layout"; then
            log_error "Layout '$layout' is not supported by shared/layouts.h."
            log_info "  Supported layouts: $(echo "$supported" | tr '\n' ' ')"
            log_info "  Add it to shared/layouts.h, or pass a supported name."
            return 1
        fi
        echo "$layout"
        return
    fi

    if ! command -v jq >/dev/null 2>&1; then
        log_error "jq is required for layout detection but was not found in PATH."
        log_info "Install jq, or pass '-layout <LAYOUT_NAME>' to skip auto-detection."
        return 1
    fi

    local kb_layouts
    kb_layouts=$(qmk info -kb "$keyboard_model" -km default -f json 2>/dev/null \
        | jq -r '.layouts | to_entries[] | [.key, (.value.layout | length)] | @tsv')

    if [ -z "$kb_layouts" ]; then
        log_error "Could not read layouts for '$keyboard_model' from 'qmk info'."
        return 1
    fi

    # Translate each QMK layout name to a candidate ONEDEADKEY_LAYOUT_* and
    # keep only those supported by shared/layouts.h.
    # A bare "LAYOUT" macro (used by some single-layout boards) maps to the
    # keyboard's slug, e.g. lily58/rev1 -> ONEDEADKEY_LAYOUT_lily58.
    local slug
    slug=$(echo "$keyboard_model" | sed 's,/,_,g' | sed 's/_rev[0-9]*$//')
    local supported
    supported=$(get_supported_layouts)

    local candidates=""
    while IFS=$'\t' read -r kb_layout key_count; do
        [ -z "$kb_layout" ] && continue
        local candidate
        if [ "$kb_layout" = "LAYOUT" ]; then
            candidate="ONEDEADKEY_LAYOUT_$slug"
        else
            candidate="ONEDEADKEY_${kb_layout}"
        fi
        if echo "$supported" | grep -qx "$candidate"; then
            candidates+="$candidate"$'\t'"$key_count"$'\n'
        fi
    done <<< "$kb_layouts"

    candidates="${candidates%$'\n'}"
    local n_candidates=0
    [ -n "$candidates" ] && n_candidates=$(echo "$candidates" | wc -l)

    if [ "$n_candidates" -eq 0 ]; then
        log_error "No supported layout matches what '$keyboard_model' exposes."
        log_info "  Keyboard exposes: $(echo "$kb_layouts" | cut -f1 | tr '\n' ' ')"
        log_info "  Add a matching ONEDEADKEY_LAYOUT_* to shared/layouts.h, or override with '-layout <name>'."
        return 1
    fi

    if [ "$n_candidates" -eq 1 ]; then
        echo "$candidates" | cut -f1
        return
    fi

    # Multiple supported candidates. Sort once by key count desc, then
    # reverse-alpha for stable tiebreak ("_ex2" wins over its base).
    local sorted
    sorted=$(echo "$candidates" | sort -t$'\t' -k2,2 -nr -k1,1 -r)

    if [ ! -t 0 ]; then
        # Non-interactive (piped / scripted) — the prompt is for humans only.
        # Refuse to guess; require an explicit -layout.
        log_error "Keyboard '$keyboard_model' has multiple supported layouts; pass '-layout' explicitly."
        log_info "  Candidates: $(echo "$sorted" | cut -f1 | tr '\n' ' ')"
        return 1
    fi

    log_info "Multiple supported layouts available for '$keyboard_model':"
    local i=1
    while IFS=$'\t' read -r name keys; do
        echo "  $i) $name ($keys keys)" >&2
        i=$((i + 1))
    done <<< "$sorted"
    local choice picked
    read -p "Choose [1-$n_candidates, default=1]: " choice
    choice="${choice:-1}"
    picked=$(echo "$sorted" | sed -n "${choice}p" | cut -f1)
    if [ -z "$picked" ]; then
        log_error "Invalid choice: '$choice'."
        return 1
    fi
    echo "$picked"
}

create_output_dir() {
    local output_dir="$1"
    if [ -d "$output_dir" ]; then
        log_warn "Output directory ${CYAN}$output_dir${NC} already exists."
        read -p "Do you want to remove it and continue? [y/N]: " confirm
        if [[ ! "$confirm" =~ ^[Yy]$ ]]; then
            log_info "Aborted by user."
            exit 0
        fi
        log_info "Removing existing output directory: ${CYAN}$output_dir${NC}"
        rm -rf "$output_dir"
    fi
    log_info "Creating output directory: ${CYAN}$output_dir${NC}"
    mkdir -p "$output_dir"
}

generate_keymap() {
    local keyboard_model="$1"
    local output_dir="$2"
    local src="$3"

    local layout_name
    if ! layout_name=$(detect_onedeadkey_layout_name "$keyboard_model" "$layout_override"); then
        exit 1
    fi
    if [ -n "$layout_override" ]; then
        log_info "Using explicit layout (from -layout): ${CYAN}$layout_name${NC}"
    else
        log_info "Detected layout name: ${CYAN}$layout_name${NC}"
    fi
    log_info "Using keymap source: ${CYAN}$src${NC}"

    log_info "Copying shared files to output directory..."
    cp -R "$SCRIPT_DIR/shared/." "$output_dir/"

    log_info "Copying keymap files to output directory..."
    cp -R "$src/." "$output_dir/"

    # Flatten include paths (../shared/ -> same directory).
    # `-i.bak` is the only in-place form accepted by both GNU and BSD sed (macOS).
    log_info "Flattening include paths..."
    sed -i.bak 's|"../shared/|"|g' "$output_dir"/*.h "$output_dir"/*.c 2>/dev/null || true
    rm -f "$output_dir"/*.h.bak "$output_dir"/*.c.bak

    # Replace placeholder in config.h template file
    log_info "Set ${CYAN}$layout_name${NC} layout name in config.h template file..."
    sed -i.bak "s/ONEDEADKEY_PLACEHOLDER_LAYOUT/$layout_name/" "$output_dir/config.h"
    rm -f "$output_dir/config.h.bak"

    log_success "Generated keymap in ${CYAN}$output_dir${NC}"
}

# Main
output_dir="./output/$keyboard_model/keymaps/$target_name"

create_output_dir "$output_dir"
generate_keymap "$keyboard_model" "$output_dir" "$src_path"

if [ "$copy_keymap" = true ]; then
    dest_dir="$QMK_HOME/keyboards/$keyboard_model/keymaps/$target_name"
    if [ -d "$dest_dir" ]; then
        log_warn "Destination directory ${CYAN}$dest_dir${NC} already exists."
        read -p "Do you want to overwrite it and continue? [y/N]: " confirm
        if [[ ! "$confirm" =~ ^[Yy]$ ]]; then
            log_info "Aborted by user."
            exit 0
        fi
        log_info "Removing existing destination directory: ${CYAN}$dest_dir${NC}"
        rm -rf "$dest_dir"
    fi
    log_info "Copying ${CYAN}$output_dir${NC} to ${CYAN}$dest_dir${NC} ..."
    mkdir -p "$dest_dir"
    cp -R "$output_dir/." "$dest_dir/"
    log_success "Copied keymap to ${CYAN}$dest_dir${NC}"
    log_info "Finally, you may want to:"
    log_info "    qmk compile -kb $keyboard_model -km $target_name    # compile the keymap"
    log_info "    # connect the keyboard"
    log_info "    qmk flash -kb $keyboard_model -km $target_name      # flash the keyboard"
else
    log_info "Now you may want to copy the output to your QMK home directory."
    log_info "This can be done by adding the '--copy' flag."
fi
