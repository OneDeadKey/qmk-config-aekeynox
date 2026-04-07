# qmk-config-aekeynox

[QMK](https://qmk.fm/) keymap implementations for [OneDeadKey](https://github.com/OneDeadKey) keyboard layouts.

## Keymaps

- **arsenik/** — [Arsenik](https://github.com/OneDeadKey/arsenik) QMK keymap
- **selenium/** — [Selenium](https://github.com/OneDeadKey/selenium) QMK keymap (configurable Arsenik variant for split keyboards)

## Getting started

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
./generator.sh -src <keymap_folder> [-kb <keyboard>] [-km <ref_keymap>] [-name <name>] [--copy]
```

| Flag             | Description                                                        |
| ---------------- | ------------------------------------------------------------------ |
| `-src <path>`    | Path to the keymap source folder (required)                        |
| `-kb <keyboard>` | Keyboard model (default: from `qmk config`)                        |
| `-km <keymap>`   | Reference keymap for layout detection (default: from `qmk config`) |
| `-name <name>`   | Name for the generated keymap (default: source folder name)        |
| `--copy`, `-cp`  | Copy output to `$QMK_HOME` after generating                        |

## Formatting

Keymap files can be formatted with [qmk-layout-fmt](https://github.com/OneDeadKey/qmk-layout-fmt):

```bash
qmk-layout-fmt arsenik/keymap.c
qmk-layout-fmt selenium/keymap.c
```

## Tests

Compile tests verify that every valid config option combination builds successfully.

```bash
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
- `LAYOUT_split_3x6_3`
- `LAYOUT_ortho_4x10`
- `LAYOUT_ortho_4x12`
- `LAYOUT_ortho_5x10`
- `LAYOUT_ortho_5x12`
- `LAYOUT_planck_grid`
- `LAYOUT_keebio_iris`

If your keyboard is not listed, you can add its layout to `shared/layouts.h` or [open an issue](https://github.com/OneDeadKey/qmk-config-aekeynox/issues) for help.

## Structure

```
arsenik/          keymap source files
selenium/         keymap source files
shared/           common QMK headers (keycodes, layouts, host layout definitions)
tests/            compile tests and matrix files
generator.sh      keymap assembler
Dockerfile        self-contained test environment
```
