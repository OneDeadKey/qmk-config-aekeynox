#!/usr/bin/env bash

# Run all compile tests: per-option → exhaustive
#
# Usage:
#   ./run_all.sh [-kb <keyboard>] [-target <target>] [-layout <LAYOUT>] [-j <jobs>]
#
# -layout is required for multi-layout boards (e.g. crkbd/rev1, beekeeb/piantor)
# because the generator refuses to guess in non-interactive contexts.

set -euo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "════════════════════════════════════════════════════════"
echo "  Step 1: generator.sh -layout flag"
echo "════════════════════════════════════════════════════════"
bash "$TESTS_DIR/test_layout_flag.sh" "$@"

echo ""
echo "════════════════════════════════════════════════════════"
echo "  Step 2: Per-option compile"
echo "════════════════════════════════════════════════════════"
bash "$TESTS_DIR/test_per_option.sh" "$@"

echo ""
echo "════════════════════════════════════════════════════════"
echo "  Step 3: Exhaustive compile"
echo "════════════════════════════════════════════════════════"
bash "$TESTS_DIR/test_exhaustive.sh" "$@"
