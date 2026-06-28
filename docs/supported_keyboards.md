# Supported keyboards

The `LAYOUT_*` names below are QMK layout macros — pass one to `generator.sh -layout <LAYOUT>` to skip auto-detection when the board exposes multiple matching layouts.

The following physical layouts are supported:

- `LAYOUT_split_3x5_2`
- `LAYOUT_split_3x5_3`
- `LAYOUT_split_3x5_3_ex2` (4 extra inner-column keys — see [Inner-column extensions](#inner-column-extensions-_ex2-layouts))
- `LAYOUT_split_3x6_3`
- `LAYOUT_split_3x6_3_ex2` (4 extra inner-column keys — see [Inner-column extensions](#inner-column-extensions-_ex2-layouts))
- `LAYOUT_ortho_4x10`
- `LAYOUT_ortho_4x12`
- `LAYOUT_ortho_5x10`
- `LAYOUT_ortho_5x12`
- `LAYOUT_planck_mit`
- `LAYOUT_planck_grid`
- `LAYOUT_keebio_iris`
- `LAYOUT_keyboardio_atreus` (8 keys beyond the spec — see [Atreus](#atreus))
- `LAYOUT` on Lily58 (all variants: `rev1`, `light`, `lite_rev3`, `glow_enc`, `r2g` — see [Lily58](#lily58))
- `LAYOUT` on Sofle (all variants: `sofle/rev1`, `sofle/keyhive`, `sofle_choc`, `sofle_pico`, `splitkb/aurora/sofle_v2`, `mechboards/sofle/pro`, `keebart/sofle_choc_pro` — see [Sofle](#sofle))

If your keyboard is not listed, you can add its layout to `shared/layouts.h` or [open an issue](https://github.com/OneDeadKey/qmk-config-aekeynox/issues) for help.

## Lily58

58 keys, 5 rows + 4 thumbs per side.

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

## Sofle

60 keys, Lily58 + 1 extra outermost thumb per side.

Sofle adds **2 more keys** to the Lily58 layout — an extra outermost thumb per side (matrix `[4,0]` left, `[9,0]` right).
On most Sofle builds, that position is a rotary encoder press.

Everything from the [Lily58](#lily58) section applies, plus two more override slots:

```c
#define ODK_EXT_THUMB_FAR_L KC_MUTE  // left  outermost thumb (encoder press)
#define ODK_EXT_THUMB_FAR_R KC_MPLY  // right outermost thumb (encoder press)
```

The same Selenium top-row caveat applies.

**`keebart/sofle_choc_pro` needs a manual override.** Unlike other Sofle variants, this board reports its layout to QMK as `LAYOUT_split_4x6_5` — a generic community-shared name that means a different physical layout on non-Sofle boards. The generator refuses to auto-bind it to avoid producing wrong keymaps elsewhere. Pass `-layout LAYOUT_sofle` to force the Sofle layout:

```bash
./generator.sh -src ./arsenik -kb keebart/sofle_choc_pro -layout LAYOUT_sofle --copy
```

## Inner-column extensions (`_ex2` layouts)

Boards that expose an `_ex2` variant have **4 extra physical keys** — 2 per side, stacked as a short inner column on the top and middle rows.
These keys are outside the Selenium/Arsenik spec, so they default to `KC_NO` (inert).

Known boards:

- [Corne v4](https://github.com/foostan/crkbd) — `LAYOUT_split_3x6_3_ex2`
- [cornifi](https://github.com/v3lmx/cornifi) — `LAYOUT_split_3x5_3_ex2`

To bind the extra keys, add to your keymap's `options.h` before generating:

```c
#define ODK_EXT_LT KC_TAB   // left  inner-top
#define ODK_EXT_LB KC_LCTL  // left  inner-middle
#define ODK_EXT_RT KC_BSPC  // right inner-top
#define ODK_EXT_RB KC_RSFT  // right inner-middle
```

The bound keycode is the same across all layers.

## Atreus

44 keys (Keyboard.io Atreus), exposed by QMK as `LAYOUT_keyboardio_atreus`.

Atreus has **8 keys beyond** the 42-key Selenium/Arsenik spec: 1 inner-corner key per side (between the home block and the thumb cluster) and 3 extra thumb keys per side.
They default to `KC_NO` (inert). Override in your keymap's `options.h`:

```c
#define ODK_EXT_INNER_L         KC_LBRC  // left  inner corner
#define ODK_EXT_INNER_R         KC_RBRC  // right inner corner
#define ODK_EXT_THUMB_OUT_L     KC_LCTL  // left  outer thumb
#define ODK_EXT_THUMB_OUT_R     KC_RCTL  // right outer thumb
#define ODK_EXT_THUMB_FAR_L     KC_LALT  // left  far thumb
#define ODK_EXT_THUMB_FAR_R     KC_RALT  // right far thumb
#define ODK_EXT_THUMB_FAR_EXT_L KC_LGUI  // left  outermost thumb
#define ODK_EXT_THUMB_FAR_EXT_R KC_RGUI  // right outermost thumb
```

`ODK_EXT_INNER_*`, `ODK_EXT_THUMB_OUT_*` and `ODK_EXT_THUMB_FAR_*` are shared with [Lily58](#lily58) / [Sofle](#sofle).
`ODK_EXT_THUMB_FAR_EXT_L` and `ODK_EXT_THUMB_FAR_EXT_R` are Atreus-only.

The same Selenium top-row caveat applies.
