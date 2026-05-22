# qmk-config-aekeynox

[QMK](https://qmk.fm/) keymap implementations for [OneDeadKey](https://github.com/OneDeadKey) keyboard layouts.

## Keymaps

- **arsenik/** — [Arsenik](https://github.com/OneDeadKey/arsenik) QMK keymap
- **selenium/** — [Selenium](https://github.com/OneDeadKey/selenium) QMK keymap (configurable Arsenik variant for split keyboards)

## Getting started

### 1. Install QMK and jq

Install QMK and its CLI by following the [QMK getting started guide](https://docs.qmk.fm/newbs_getting_started) for your OS.
You also need [jq](https://jqlang.org/) (used by the generator for layout detection).

Then check that `qmk config user.qmk_home` resolves to an absolute path on your system:

```sh
qmk config user.qmk_home
```

If it's empty, point it at your QMK firmware clone:

```bash
qmk config user.qmk_home="~/path/to/qmk_firmware"
```

> **Note:** If `qmk config` shows a literal `$HOME/...` instead of an expanded path, see [Setting `user.qmk_home`](#setting-userqmk_home) in the Reference section.

### 2. Configure QMK for your keyboard

Tell QMK which keyboard you're using. To find your keyboard's QMK name:

```bash
# example for piantor
qmk list-keyboards | grep piantor
```

Then configure it (replace `beekeeb/piantor` with your model):

```bash
# set your keyboard
qmk config user.keyboard=beekeeb/piantor

# check your config
qmk config
```

### 3. Clone this repository

```bash
git clone https://github.com/OneDeadKey/qmk-config-aekeynox.git
cd qmk-config-aekeynox
```

### 4. Customize (optional)

Each keymap has an `options.h` file where you can configure host layout, hold-tap behavior, and other features.
Each option is documented with inline comments. Edit this file **before** generating your keymap.

The defaults work out of the box — you can skip this step and come back later.

### 5. Generate the keymap and copy it to QMK

Run the generator to build your keymap. The `--copy` flag installs it into
QMK so it can be compiled and flashed from there; without `--copy`, the
keymap is only written to `./output/` (useful for previewing before
installing).

If a QMK keyboard reference declares several physical variants, the
generator will ask you to pick the one matching your actual board — refer
to your keyboard's specs (key count, inner column, thumb cluster shape) to
choose. Pass `-layout <LAYOUT_NAME>` to skip the prompt.

```bash
./generator.sh -src ./selenium --copy
```

Other useful invocations:

```bash
# Generate only — preview without installing into QMK
./generator.sh -src ./selenium

# Pick a keyboard explicitly (ignores user.keyboard)
./generator.sh -src ./selenium -kb beekeeb/piantor --copy

# Force a specific physical layout (skip the variant prompt)
./generator.sh -src ./selenium -layout LAYOUT_split_3x6_3_ex2 --copy

# Full flag list
./generator.sh --help
```

### 6. Compile and flash

```bash
# Compile the firmware (replace `selenium` with `arsenik` if you generated Arsenik)
qmk compile -kb beekeeb/piantor -km selenium

# Connect your keyboard, then flash
qmk flash -kb beekeeb/piantor -km selenium
```

The keyboard enters flash mode differently depending on the board — check your keyboard's documentation (usually a reset button or a key combination).

## Reference

### Setting `user.qmk_home`

`qmk config` stores the value verbatim, then later expands a leading `~` but **not** `$HOME`.
Whether a form works therefore depends on what your shell did with the value before `qmk` saw it.

Works:

```bash
qmk config user.qmk_home=$HOME/qmk_firmware       # shell expands $HOME → absolute path stored
qmk config user.qmk_home="$HOME/qmk_firmware"     # same: double quotes still expand $HOME
qmk config user.qmk_home="~/qmk_firmware"         # literal ~ stored; qmk expands it at read-time
qmk config user.qmk_home=/home/you/qmk_firmware   # plain absolute path, no expansion needed
```

Breaks:

```bash
qmk config user.qmk_home='$HOME/qmk_firmware'     # single quotes: literal $HOME stored, nothing expands it
```

After setting, run `qmk config` and verify the value resolves to an absolute path or starts with `~` — never a literal `$HOME`.

### Supported keyboards

The following physical layouts are supported:

- `LAYOUT_split_3x5_2`
- `LAYOUT_split_3x5_3`
- `LAYOUT_split_3x5_3_ex2` (4 extra inner-column keys — see below)
- `LAYOUT_split_3x6_3`
- `LAYOUT_split_3x6_3_ex2` (4 extra inner-column keys — see below)
- `LAYOUT_ortho_4x10`
- `LAYOUT_ortho_4x12`
- `LAYOUT_ortho_5x10`
- `LAYOUT_ortho_5x12`
- `LAYOUT_planck_grid`
- `LAYOUT_keebio_iris`
- `LAYOUT` on Lily58 (all variants: `rev1`, `light`, `lite_rev3`, `glow_enc`, `r2g` — see below)
- `LAYOUT` on Sofle (all variants: `sofle/rev1`, `sofle/keyhive`, `sofle_choc`, `sofle_pico`, `splitkb/aurora/sofle_v2`, `mechboards/sofle/pro`, `keebart/sofle_choc_pro` — see below)

If your keyboard is not listed, you can add its layout to `shared/layouts.h` or [open an issue](https://github.com/OneDeadKey/qmk-config-aekeynox/issues) for help.

#### Lily58 (58 keys, 5 rows + 4 thumbs per side)

Lily58 has **4 keys beyond** the 42-key Selenium/Arsenik spec: 1 inner-corner key per side (below the home row) and 1 extra outermost thumb per side.
They default to `KC_NO` (inert). Override in your keymap's `options.h`:

```c
#define ODK_EXT_INNER_L     KC_LBRC  // left  inner corner
#define ODK_EXT_INNER_R     KC_RBRC  // right inner corner
#define ODK_EXT_THUMB_OUT_L KC_LCTL  // left  outermost thumb
#define ODK_EXT_THUMB_OUT_R KC_RCTL  // right outermost thumb
```

**Selenium caveat:** Selenium is a 42-key spec, so logical row 1 (the top row) is transparent on every layer.
On Lily58 with Selenium, the physical number row emits nothing until you edit `selenium/keymap.c` row 1 locally.
**Arsenik is unaffected** — its base layer uses the top row for numbers natively.

#### Sofle (60 keys, Lily58 + 1 extra outermost thumb per side)

Sofle adds **2 more keys** to the Lily58 layout — an extra outermost thumb per side (matrix `[4,0]` left, `[9,0]` right).
On most Sofle builds, that position is a rotary encoder press.

Everything from the Lily58 section applies, plus two more override slots:

```c
#define ODK_EXT_THUMB_FAR_L KC_MUTE  // left  outermost thumb (encoder press)
#define ODK_EXT_THUMB_FAR_R KC_MPLY  // right outermost thumb (encoder press)
```

The same Selenium top-row caveat applies.

**Note for `keebart/sofle_choc_pro` users:** that board exposes `LAYOUT_split_4x6_5` (a community layout name) rather than a Sofle-specific one.
Auto-detection would produce `ONEDEADKEY_LAYOUT_split_4x6_5`, which is intentionally not matched (the community name has a different meaning on other keyboards).
Generate with `-layout LAYOUT_sofle` to force the Sofle layout.

#### Inner-column extensions (`_ex2` layouts)

Boards like the Corne v4 expose an `_ex2` variant with **4 extra physical keys** — 2 per side, stacked as a short inner column on the top and middle rows.
These keys are outside the Selenium/Arsenik spec, so they default to `KC_NO` (inert).

To bind them, add to your keymap's `options.h` before generating:

```c
#define ODK_EXT_LT KC_TAB   // left  inner-top
#define ODK_EXT_LB KC_LCTL  // left  inner-middle
#define ODK_EXT_RT KC_BSPC  // right inner-top
#define ODK_EXT_RB KC_RSFT  // right inner-middle
```

The bound keycode is the same across all layers.

### Generator options

```bash
./generator.sh -src <keymap_folder> [-kb <keyboard_model>] [-layout <LAYOUT>] [-name <target_name>] [--copy]
```

| Flag                   | Description                                                                     |
| ---------------------- | ------------------------------------------------------------------------------- |
| `-src <path>`          | Path to the keymap source folder (required)                                     |
| `-kb <keyboard_model>` | Keyboard model (default: from `qmk config user.keyboard`)                       |
| `-layout <LAYOUT>`     | Explicit LAYOUT macro override (e.g. `LAYOUT_split_3x6_3_ex2`); skips detection |
| `-name <target_name>`  | Name for the generated keymap (default: source folder name)                     |
| `--copy`, `-cp`        | Copy output to `$QMK_HOME` after generating                                     |

The generator infers the physical layout from `qmk info -kb <keyboard>` by intersecting the board's `layouts` field with the layouts supported by `shared/layouts.h`.
When the intersection contains a single layout, it is used silently. When several qualify (e.g. `crkbd/rev1` exposes both `LAYOUT_split_3x5_3` and `LAYOUT_split_3x6_3`),
the generator prompts you to choose; the default (Enter) is the largest by key count.
In non-interactive contexts (piped invocations, scripts) the prompt is unavailable, so the generator refuses to guess and requires `-layout` explicitly.
Pass `-layout` to disambiguate without the prompt. When none qualify, the generator fails with the list of layouts the board exposes so a matching one can be added to `shared/layouts.h`.

### Formatting

Keymap files can be formatted with [qmk-layout-fmt](https://github.com/OneDeadKey/qmk-layout-fmt):

```bash
qmk-layout-fmt arsenik/keymap.c
qmk-layout-fmt selenium/keymap.c
```

## Development

### Structure

```
arsenik/          keymap source files
selenium/         keymap source files
shared/           common QMK headers (keycodes, layouts, host layout definitions)
tests/            compile tests and matrix files
generator.sh      keymap assembler
Dockerfile        self-contained test environment
```

### Tests

Compile tests verify that every valid config option combination builds successfully.

```bash
# generator.sh -layout flag plumbing (fast, no compile)
bash tests/test_layout_flag.sh

# Per-option tests (~2 min)
bash tests/test_per_option.sh

# Exhaustive tests (~10 min)
bash tests/test_exhaustive.sh

# Filter by target
bash tests/test_per_option.sh -target selenium
```

#### Docker

No QMK setup needed — the Docker image includes everything.

```bash
docker build -t qmk-1dk-test .
docker run --rm qmk-1dk-test tests/run_all.sh
```
