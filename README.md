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

> **Boards with extra keys** (Lily58, Sofle, Corne v4, cornifi, Atreus) leave the extras inert by default. See [docs/supported_keyboards.md](docs/supported_keyboards.md) for the `ODK_EXT_*` override macros that bind them.

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

Any QMK keyboard with a compatible physical layout works:

- 32-key splits (3×5, 2 thumbs)
- 36-key splits (3×5, 3 thumbs) — Ferris, Corne (3×5)
- 40-key splits, inner column — cornifi → see [docs](docs/supported_keyboards.md#inner-column-extensions-_ex2-layouts)
- 42-key splits (3×6, 3 thumbs) — Corne (crkbd), Piantor
- 46-key splits, inner column — Corne v4 → see [docs](docs/supported_keyboards.md#inner-column-extensions-_ex2-layouts)
- 40-key ortho (4×10)
- 48-key ortho (4×12) — Planck (incl. grid), Preonic-likes
- 50-key ortho (5×10)
- 60-key ortho (5×12)
- Iris (keebio)
- Atreus (keyboardio) → see [docs](docs/supported_keyboards.md#atreus)
- Lily58 (all revs) → see [docs](docs/supported_keyboards.md#lily58)
- Sofle (all revs incl. `keebart/sofle_choc_pro`) → see [docs](docs/supported_keyboards.md#sofle)

If your keyboard is not listed, you can add its layout to `shared/layouts.h` or [open an issue](https://github.com/OneDeadKey/qmk-config-aekeynox/issues) for help.

#### Boards not in mainline QMK

Keyboards missing from mainline QMK may still be supported by a fork. The setup is fork-specific; the Keebart fork below is a worked example.

[`vial-kb/vial-qmk`](https://github.com/vial-kb/vial-qmk) is another fork worth checking — it adds ~150 boards. Layout-compatible candidates include `bastardkb/charybdis` (3×5/3×6 variants), `beekeeb/piantor` controller variants, and Lily58-shaped boards. None tested from this repo.

##### Keebart fork

[`Keebart/vial-qmk-keebart`](https://github.com/Keebart/vial-qmk-keebart) carries Keebart's own product line:

- Corne Choc Pro
- Corne MX Pro

See [`docs/keebart_keyboards.md`](docs/keebart_keyboards.md) for the setup recipe and per-variant build targets.

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
