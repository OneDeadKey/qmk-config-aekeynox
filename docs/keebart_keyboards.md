# Keebart keyboards

Keebart sells split keyboards that are not all in mainline QMK. Building aekeynox for them requires switching `qmk_home` to [`Keebart/vial-qmk-keebart`](https://github.com/Keebart/vial-qmk-keebart).

Upstream chain: `qmk/qmk_firmware` ← `vial-kb/vial-qmk` ← `Keebart/vial-qmk-keebart` (default branch `vial`).

> **Tested:** `keebart/corne_choc_pro` (verified built and flashed).
> **Untested:** `keebart/corne_mx_pro` — listed here for completeness based on the fork's keyboard definitions; not built or flashed from this repo.

## Boards covered

| Target                            | Default layout       | Notes                                                                                        |
| --------------------------------- | -------------------- | -------------------------------------------------------------------------------------------- |
| `keebart/corne_choc_pro/mini`     | `LAYOUT_split_3x5_3` | Single layout, auto-detected.                                                                |
| `keebart/corne_choc_pro/standard` | `LAYOUT_split_3x6_3` | Single layout, auto-detected.                                                                |
| `keebart/corne_mx_pro/mini`       | _prompts_            | Generator prompts; see [Layout selection](#layout-selection-for-corne_mx_pro). **Untested.** |
| `keebart/corne_mx_pro/standard`   | _prompts_            | Generator prompts; see [Layout selection](#layout-selection-for-corne_mx_pro). **Untested.** |

`keebart/sofle_choc_pro` is **in mainline QMK** and does not need the fork. It is documented in [supported_keyboards.md#sofle](supported_keyboards.md#sofle) with its required `-layout LAYOUT_sofle` override.

## One-time fork setup

### 1. Clone Keebart's fork

```sh
git clone https://github.com/Keebart/vial-qmk-keebart
```

### 2. Initialize submodules

Initialize submodules in the clone:

```sh
cd path/to/vial-qmk-keebart && qmk git-submodule
```

Equivalent to `git submodule update --init --recursive`. Takes a few minutes.

### 3. Point `qmk config` at the fork

```sh
qmk config user.qmk_home=/path/to/vial-qmk-keebart
qmk config user.keyboard=keebart/corne_choc_pro/standard
```

See [Setting `user.qmk_home`](../README.md#setting-userqmk_home) in the main README for path-expansion caveats.

## Building

Once configured, the generator works identically to mainline:

```sh
./generator.sh -src ./selenium --copy
```

For `corne_choc_pro/*`, it detects the single supported layout and proceeds silently. For `corne_mx_pro/*`, it prompts (see [Layout selection](#layout-selection-for-corne_mx_pro)).

Compile and flash:

```sh
qmk compile -kb keebart/corne_choc_pro/standard -km selenium
qmk flash   -kb keebart/corne_choc_pro/standard -km selenium
```

## Build target gotcha: variant suffix is mandatory

`keebart/corne_choc_pro` and `keebart/corne_mx_pro` (without the `/mini` or `/standard` suffix) are **not buildable targets** — they are parent folders. Always include the variant suffix from the [Boards covered](#boards-covered) table.

## Layout selection for `corne_mx_pro`

`corne_mx_pro/mini` and `corne_mx_pro/standard` each expose four layouts. The generator prompts you to pick the one matching your physical board:

| Layout                   | Physical board                                       |
| ------------------------ | ---------------------------------------------------- |
| `LAYOUT_split_3x5_3`     | 3×5 grid, 3 thumbs, no inner column                  |
| `LAYOUT_split_3x5_3_ex2` | 3×5 grid + inner-column daughterboard (4 extra keys) |
| `LAYOUT_split_3x6_3`     | 3×6 grid, 3 thumbs, no inner column                  |
| `LAYOUT_split_3x6_3_ex2` | 3×6 grid + inner-column daughterboard (4 extra keys) |

For `_ex2` layouts, the 4 extra keys default to `KC_NO`. Bind them via `ODK_EXT_LT/LB/RT/RB` in your keymap's `options.h` — see [Inner-column extensions](supported_keyboards.md#inner-column-extensions-_ex2-layouts) in `supported_keyboards.md`.

## Switching back to mainline

To return to a mainline QMK keyboard (e.g. `beekeeb/piantor`):

```sh
qmk config user.qmk_home=/path/to/qmk_firmware
qmk config user.keyboard=beekeeb/piantor
```

The generator works identically on mainline.
