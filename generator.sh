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
    echo "Usage: $0 -src <keymap_folder> [-kb <keyboard_model>] [-km <ref_keymap>] [-layout <LAYOUT>] [-name <target_name>] [--copy|-cp]"
    echo "  -src <keymap_folder>   Path to the keymap source folder (required)"
    echo "  -kb <keyboard_model>   Specify the keyboard model (optional, will use qmk config if not provided)"
    echo "  -km <ref_keymap>       Reference keymap for layout detection (optional, will use qmk config if not provided)"
    echo "  -layout <LAYOUT>       Explicit LAYOUT macro (e.g. LAYOUT_split_3x6_3_ex2); skips auto-detection from -km"
    echo "  -name <target_name>    Name for the generated keymap (default: source folder name)"
    echo "  --copy, -cp            Copy output to \$QMK_HOME/keyboards/<keyboard_model>/keymaps/<target_name>"
    exit 1
}

# Parse arguments
copy_keymap=false
keyboard_model=""
keymap=""
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
    -km)
        shift
        keymap="$1"
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

if [ -z "$keyboard_model" ] || { [ -z "$keymap" ] && [ -z "$layout_override" ]; }; then
    log_info "Requirements: It is recommended to have set QMK user.keyboard, user.keymap, and user.qmk_home using 'qmk config'."
    log_info "See https://docs.qmk.fm/cli_configuration#setting-user-defaults for more information."
    log_info "Your current configuration:"
    qmk config || true
fi

# If keyboard_model is empty, get from qmk config
if [ -z "$keyboard_model" ]; then
    keyboard_model=$(qmk config user.keyboard | awk -F= '{print $2}' | xargs)
    log_info "No keyboard_model provided with '-kb' argument, using value from qmk config: ${CYAN}$keyboard_model${NC}"
fi
# -km is only needed when the layout must be auto-detected from a reference keymap
if [ -z "$keymap" ] && [ -z "$layout_override" ]; then
    keymap=$(qmk config user.keymap | awk -F= '{print $2}' | xargs)
    log_info "No keymap provided with '-km' argument, using value from qmk config: ${CYAN}$keymap${NC}"
fi

# Set QMK_HOME if not already set
if [ -z "${QMK_HOME+x}" ]; then
    QMK_HOME=$(qmk config user.qmk_home | awk -F= '{print $2}' | xargs | envsubst)
fi

# Find the keymaps folder for a given keyboard name
get_keymaps_folder() {
    local keyboard_model="$1"
    local keymap="$2"
    while [ ! -d "$QMK_HOME/keyboards/$keyboard_model/keymaps/$keymap" ]; do
        if [ "$keyboard_model" = "." ]; then
            echo "Couldn't find keymap folder for your keyboard/keymap" >&2
            exit 1
        fi
        keyboard_model=$(dirname "$keyboard_model")
    done
    echo "$QMK_HOME/keyboards/$keyboard_model/keymaps/$keymap"
}

# Detect layout name
# If $3 (override) is provided, skip auto-detection from the reference keymap.
detect_layout_name() {
    local keyboard_model="$1"
    local keymap="$2"
    local override="${3:-}"

    local layout=""
    if [ -n "$override" ]; then
        # Accept bare (split_3x6_3_ex2), LAYOUT_* or ONEDEADKEY_LAYOUT_* forms
        layout="$override"
    else
        local keymap_folder
        keymap_folder=$(get_keymaps_folder "$keyboard_model" "$keymap")

        if [ -f "$keymap_folder/keymap.c" ]; then
            layout=$(grep 'LAYOUT' "$keymap_folder/keymap.c" | sed 's/.*= \(.*\)(/\1/' | head -n 1)
        elif [ -f "$keymap_folder/keymap.json" ]; then
            layout=$(grep 'LAYOUT' "$keymap_folder/keymap.json" | sed 's/ *\"layout\": \"\(.*\)\",/\1/')
        fi

        if [ "$layout" = "LAYOUT" ]; then
            layout+="_$(echo "$keyboard_model" | sed 's,/,_,g' | sed 's/_rev[0-9]*$//')"
        fi
    fi

    # Normalize: ensure the ONEDEADKEY_LAYOUT_ prefix
    if [[ "$layout" == ONEDEADKEY_LAYOUT_* ]]; then
        :
    elif [[ "$layout" == LAYOUT_* ]]; then
        layout="ONEDEADKEY_$layout"
    else
        layout="ONEDEADKEY_LAYOUT_$layout"
    fi

    # Warn (don't fail) if the resolved layout has no matching branch in shared/layouts.h.
    # Branches may use either `#elif defined FOO` (space) or `#elif defined(FOO)` (paren, for OR-guards).
    # Compilation would later fail at the '#error "Arsenik: Unknown layout"' fallthrough.
    if ! grep -Eq "defined[[:space:](]+$layout\b" "$SCRIPT_DIR/shared/layouts.h" 2>/dev/null; then
        log_warn "Layout ${CYAN}$layout${NC} has no branch in shared/layouts.h — compile will fail unless you add one."
    fi

    echo "$layout"
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

# Generate the keymap
generate_keymap() {
    local keyboard_model="$1"
    local keymap="$2"
    local output_dir="$3"
    local src="$4"

    local layout_name="$(detect_layout_name "$keyboard_model" "$keymap" "$layout_override")"
    if [ -n "$layout_override" ]; then
        log_info "Using explicit layout (from -layout): ${CYAN}$layout_name${NC}"
    else
        log_info "Detected layout name: ${CYAN}$layout_name${NC}"
    fi
    log_info "Using keymap source: ${CYAN}$src${NC}"

    log_info "Copying shared files to output directory..."
    rsync -a "$SCRIPT_DIR/shared/" "$output_dir"

    log_info "Copying keymap files to output directory..."
    rsync -a "$src/" "$output_dir"

    # Flatten include paths (../shared/ -> same directory)
    log_info "Flattening include paths..."
    sed -i 's|"../shared/|"|g' "$output_dir"/*.h "$output_dir"/*.c 2>/dev/null || true

    # Replace placeholder in config.h template file
    log_info "Set ${CYAN}$layout_name${NC} layout name in config.h template file..."
    sed -i "s/ONEDEADKEY_PLACEHOLDER_LAYOUT/$layout_name/" "$output_dir/config.h"

    log_success "Generated keymap in ${CYAN}$output_dir${NC}"
}

# Main
output_dir="./output/$keyboard_model/keymaps/$target_name"

create_output_dir "$output_dir"
generate_keymap "$keyboard_model" "$keymap" "$output_dir" "$src_path"

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
    mkdir -p "$(dirname "$dest_dir")"
    rsync -a "$output_dir/" "$dest_dir"
    log_success "Copied keymap to ${CYAN}$dest_dir${NC}"
    log_info "Finally, you may want to :"
    log_info "    cd $dest_dir"
    log_info "    qmk compile -kb $keyboard_model -km $target_name    # compile the keymap"
    log_info "    # connect the keyboard"
    log_info "    qmk flash -kb $keyboard_model -km $target_name      # flash the keyboard"
else
    log_info "Now you may want to copy the output to your QMK home directory."
    log_info "This can be done by adding the '--copy' flag."
fi
