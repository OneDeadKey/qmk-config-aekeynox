# QMK Compile Tests

Compile tests verify that every valid config option combination builds successfully for each keymap target (arsenik, selenium).

## Requirements

- QMK CLI installed and configured (`qmk config user.keyboard`, `user.keymap`, `user.qmk_home`)
- QMK firmware cloned (`qmk setup`)
- Must run from the `qmk/` directory

## Running tests

```sh
cd qmk

# Run everything (per-option + exhaustive, both targets)
bash tests/run_all.sh

# Per-option only (one option toggled at a time, ~2 min)
bash tests/test_per_option.sh

# Exhaustive only (all valid combinations from matrix files, ~10 min)
bash tests/test_exhaustive.sh

# One layout branch (defaults compile for the given board's layout)
bash tests/test_layouts.sh -kb planck/rev5 -layout ortho_4x12

# Filter by target
bash tests/test_per_option.sh -target arsenik
bash tests/test_exhaustive.sh -target selenium

# Override keyboard and parallel jobs
bash tests/test_per_option.sh -kb beekeeb/piantor -j 4

# Multi-layout boards require -layout (the generator refuses to guess in
# non-interactive contexts)
bash tests/test_per_option.sh -kb crkbd/rev1 -layout LAYOUT_split_3x6_3
```

## Running tests in Docker

No QMK setup needed — the Docker image includes everything.

```sh
# From the repo root
docker build -f qmk/Dockerfile -t arsenik-qmk-test .

# Run per-option tests
docker run --rm --entrypoint "" arsenik-qmk-test bash qmk/tests/test_per_option.sh

# Run exhaustive tests
docker run --rm --entrypoint "" arsenik-qmk-test bash qmk/tests/test_exhaustive.sh

# Run everything
docker run --rm --entrypoint "" arsenik-qmk-test bash qmk/tests/run_all.sh
```

The QMK version is pinned in `qmk/.env` and used by both the Dockerfile and CI.

## Test structure

| File                  | Purpose                                                          |
| --------------------- | ---------------------------------------------------------------- |
| `common.sh`           | Shared functions: generate, patch config, compile, matrix runner |
| `test_per_option.sh`  | Toggles one option at a time per target                          |
| `test_exhaustive.sh`  | Reads matrix files, tests all valid combinations                 |
| `test_layouts.sh`     | Compiles one `ONEDEADKEY_LAYOUT_*` branch on a board exposing it  |
| `matrix_arsenik.txt`  | Arsenik combination matrix (human-readable)                      |
| `matrix_selenium.txt` | Selenium combination matrix (human-readable)                     |
| `run_all.sh`          | Runs per-option then exhaustive                                  |

## Matrix file format

Each line defines a test case. Use `+DEFINE` to enable and `-DEFINE` to disable a `#define`:

```
# Comments start with #
test name here         | +ENABLE_THIS -DISABLE_THAT +ANOTHER_FLAG
defaults               |
```

To add a new combination, add a line to the relevant matrix file. No code changes needed.

## CI

GitHub Actions runs automatically:

- **On PRs**: per-option compile tests + layout-branch compile tests
- **On pushes to `main`**: exhaustive compile tests
