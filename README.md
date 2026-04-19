# qmk-config-aekeynox

[QMK](https://qmk.fm/) keymap implementations for [OneDeadKey](https://github.com/OneDeadKey) keyboard layouts.

## Keymaps

- **arsenik/** — [Arsenik](https://github.com/OneDeadKey/arsenik) QMK keymap
- **selenium/** — [Selenium](https://github.com/OneDeadKey/selenium) QMK keymap (configurable Arsenik variant for split keyboards)

## Getting started

### Prerequisites

- `rsync` — required by `generator.sh` to assemble the keymap output

### 1. Install QMK

Follow the [QMK getting started guide](https://docs.qmk.fm/newbs_getting_started) for your OS, then:

```bash
# Install the QMK CLI (pick one)
uv tool install qmk
pipx install qmk
python3 -m pip install qmk

# Setup - it clones the QMK firmware repository (~1.5 GB)
qmk setup
```

### 2. Configure QMK for your keyboard

Tell QMK which keyboard and keymap to use. To find your keyboard's QMK name:

```bash
qmk list-keyboards | grep <your keyboard>
qmk info -kb <keyboard_model>
```

Then configure it (replace `beekeeb/piantor` with your model):

```bash
# set your config values
qmk config user.keyboard=beekeeb/piantor
qmk config user.keymap=default
qmk config user.qmk_home=$HOME/qmk_firmware

# check your config
qmk config
```

> **Warning:** for `user.qmk_home`, always use `$HOME` (not `~`). QMK stores the value literally and does not expand `~`, so `qmk config user.qmk_home=~/qmk_firmware` will break subsequent commands.

### 3. Clone this repository

```bash
git clone https://github.com/OneDeadKey/qmk-config-aekeynox.git
cd qmk-config-aekeynox
```

### 4. Customize (optional)

Each keymap has an `options.h` file where you can configure host layout, hold-tap behavior, and other features. Each option is documented with inline comments. Edit this file **before** generating your keymap.

The defaults work out of the box — you can skip this step and come back later.

### 5. Generate the keymap

The generator assembles a keymap from a source folder + shared headers, adapts it for your keyboard layout, and outputs the result in `./output/`.

```bash
./generator.sh -src ./selenium
```

You can also specify the keyboard explicitly:

```bash
./generator.sh -src ./selenium -kb beekeeb/piantor
```

### 6. Copy to QMK

Copy the generated keymap into your QMK firmware tree:

```bash
./generator.sh -src ./selenium --copy
```

Or do both in one step:

```bash
./generator.sh -src ./selenium --copy
```

### 7. Compile and flash

```bash
# Compile the firmware
qmk compile -km selenium

# Connect your keyboard, then flash
qmk flash -km selenium
```

The keyboard enters flash mode differently depending on the board — check your keyboard's documentation (usually a reset button or a key combination).

## Generator options

```bash
./generator.sh -src <keymap_folder> [-kb <keyboard>] [-km <ref_keymap>] [-layout <LAYOUT>] [-name <name>] [--copy]
```

| Flag               | Description                                                                          |
| ------------------ | ------------------------------------------------------------------------------------ |
| `-src <path>`      | Path to the keymap source folder (required)                                          |
| `-kb <keyboard>`   | Keyboard model (default: from `qmk config`)                                          |
| `-km <keymap>`     | Reference keymap for layout auto-detection (default: from `qmk config`)              |
| `-layout <LAYOUT>` | Explicit LAYOUT macro (e.g. `LAYOUT_split_3x6_3_ex2`). Skips detection; `-km` unused |
| `-name <name>`     | Name for the generated keymap (default: source folder name)                          |
| `--copy`, `-cp`    | Copy output to `$QMK_HOME` after generating                                          |

Use `-layout` when the keyboard exposes multiple physical variants (e.g. `crkbd/rev4_0` has both `LAYOUT_split_3x6_3` and `LAYOUT_split_3x6_3_ex2`) and the reference keymap doesn't use the one you want.

## Formatting

Keymap files can be formatted with [qmk-layout-fmt](https://github.com/OneDeadKey/qmk-layout-fmt):

```bash
qmk-layout-fmt arsenik/keymap.c
qmk-layout-fmt selenium/keymap.c
```

## Tests

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

### Docker

No QMK setup needed — the Docker image includes everything.

```bash
docker build -t qmk-1dk-test .
docker run --rm qmk-1dk-test tests/run_all.sh
```

## Supported keyboards

The keymaps use a `ONEDEADKEY_LAYOUT` macro that adapts automatically to different keyboard sizes and shapes. The following physical layouts are supported:

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

### Lily58 (58 keys, 5 rows + 4 thumbs per side)

Lily58 has **4 keys beyond** the 42-key Selenium/Arsenik spec: 1 inner-corner key per side (below the home row) and 1 extra outermost thumb per side. They default to `KC_NO` (inert). Override in your keymap's `options.h`:

```c
#define ODK_EXT_INNER_L     KC_LBRC  // left  inner corner
#define ODK_EXT_INNER_R     KC_RBRC  // right inner corner
#define ODK_EXT_THUMB_OUT_L KC_LCTL  // left  outermost thumb
#define ODK_EXT_THUMB_OUT_R KC_RCTL  // right outermost thumb
```

**Selenium caveat:** Selenium is a 42-key spec, so logical row 1 (the top row) is transparent on every layer. On Lily58 with Selenium, the physical number row emits nothing until you edit `selenium/keymap.c` row 1 locally. **Arsenik is unaffected** — its base layer uses the top row for numbers natively.

### Sofle (60 keys, Lily58 + 1 extra outermost thumb per side)

Sofle adds **2 more keys** to the Lily58 layout — an extra outermost thumb per side (matrix `[4,0]` left, `[9,0]` right). On most Sofle builds, that position is a rotary encoder press.

Everything from the Lily58 section applies, plus two more override slots:

```c
#define ODK_EXT_THUMB_FAR_L KC_MUTE  // left  outermost thumb (encoder press)
#define ODK_EXT_THUMB_FAR_R KC_MPLY  // right outermost thumb (encoder press)
```

The same Selenium top-row caveat applies.

**Note for `keebart/sofle_choc_pro` users:** that board exposes `LAYOUT_split_4x6_5` (a community layout name) rather than a Sofle-specific one. Auto-detection would produce `ONEDEADKEY_LAYOUT_split_4x6_5`, which is intentionally not matched (the community name has a different meaning on other keyboards). Generate with `-layout LAYOUT_sofle` to force the Sofle branch.

### Inner-column extensions (`_ex2` layouts)

Boards like the Corne v4 expose an `_ex2` variant with **4 extra physical keys** — 2 per side, stacked as a short inner column on the top and middle rows. These keys are outside the Selenium/Arsenik spec, so they default to `KC_NO` (inert).

To bind them, add to your keymap's `options.h` before generating:

```c
#define ODK_EXT_LT KC_TAB   // left  inner-top
#define ODK_EXT_LB KC_LCTL  // left  inner-middle
#define ODK_EXT_RT KC_BSPC  // right inner-top
#define ODK_EXT_RB KC_RSFT  // right inner-middle
```

The bound keycode is the same across all layers.

## Structure

```
arsenik/          keymap source files
selenium/         keymap source files
shared/           common QMK headers (keycodes, layouts, host layout definitions)
tests/            compile tests and matrix files
generator.sh      keymap assembler
Dockerfile        self-contained test environment
```
