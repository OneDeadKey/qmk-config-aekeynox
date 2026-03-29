# Selenium QMK Implementation

This is a QMK implementation of the [Selenium specification](https://github.com/OneDeadKey/selenium). The [ZMK implementation](https://github.com/OneDeadKey/zmk-config-selenium) was used as a practical cross-reference during development, but the spec is the authority.

QMK and ZMK don't offer the same features. This document explains the design decisions and platform differences.

### Terminology

This document refers to four distinct layers of implementation:

- **ZMK native**: features built into the ZMK firmware — `&lt`, `&mt`, `&sl`, `&sk`, the `flavor` property (`tap-preferred`, `hold-preferred`, `balanced`, `tap-unless-interrupted`), and devicetree configuration like `tapping-term-ms`.
- **Selenium ZMK**: custom behaviors defined in Selenium's ZMK config (`hold_taps.dtsi`) — `&hrm`, `&sc`, `bsl`, `bsk`, `lbsk`, and the `EZ_SL`/`EZ_SK`/`EZ_LSK` macros. These compose ZMK native primitives into Selenium-specific hold-tap behaviors.
- **QMK native**: features built into the QMK firmware — `LT()`, `*_T()`, `OSM()`, `OSL()`, `PERMISSIVE_HOLD`, `HOLD_ON_OTHER_KEY_PRESS`, `TAPPING_TERM_PER_KEY`, and their associated per-key callbacks.
- **Selenium QMK**: custom keycodes and logic in this implementation — `LSK_RALT`, `tap_keycode_is_tap_preferred()`, and the per-key callback implementations.

## Timing

### ZMK model: per-behavior timing

ZMK natively assigns timing and a `flavor` property to each hold-tap behavior definition independently. The `flavor` property controls how interrupts (other keypresses during the hold) influence the tap-or-hold decision — see [Hold-tap behavior](#hold-tap-behavior) for details.

Selenium's ZMK config defines custom behaviors (`&hrm`, `&sc`, `bsl`, `bsk`, `lbsk`) that compose ZMK native hold-tap primitives (`&lt`, `&mt`) with specific timing and `flavor` values:

| Behavior                                 | ZMK `flavor`   | `tapping-term-ms`            | `quick-tap-ms`      | Used by                                                             |
| ---------------------------------------- | -------------- | ---------------------------- | ------------------- | ------------------------------------------------------------------- |
| `&hrm` (Selenium custom)                 | tap-preferred  | 300ms (`TAPPING_TERM`)       | 200ms (`QUICK_TAP`) | HRM keys (S/D/F/J/K/L), function layer media HRMs                   |
| `&sc` (Selenium custom)                  | hold-preferred | 150ms (`SHORT_TAPPING_TERM`) | 200ms (`QUICK_TAP`) | Thumb keys that don't produce text (Enter, Escape, Del, layer taps) |
| `&lt` (ZMK native, reconfigured)         | tap-preferred  | 300ms (`TAPPING_TERM`)       | 200ms (`QUICK_TAP`) | Space thumb, NavNum/NumLock layer+space                             |
| `&mt` (ZMK native, reconfigured)         | hold-preferred | 150ms (`SHORT_TAPPING_TERM`) | 200ms (`QUICK_TAP`) | LCTL+Backspace thumb (HT_THUMB_TAPS)                                |
| `bsl` / `bsk` / `lbsk` (Selenium custom) | tap-preferred  | 300ms (`TAPPING_TERM`)       | —                   | EZ_SL, EZ_SK, EZ_LSK wrappers                                       |

### QMK model: single global + per-key callbacks

QMK natively provides one global `TAPPING_TERM` plus optional per-key callback functions (`TAPPING_TERM_PER_KEY`, `QUICK_TAP_TERM_PER_KEY`). Selenium QMK uses these callbacks to replicate the per-behavior timing from the ZMK config:

| Setting            | Value | How                                                                            |
| ------------------ | ----- | ------------------------------------------------------------------------------ |
| `TAPPING_TERM`     | 150ms | QMK native global default (set in `options.h` as `SHORT_TAPPING_TERM`)         |
| `HRM_TAPPING_TERM` | 300ms | Selenium QMK constant, returned by `get_tapping_term()` for tap-preferred keys |
| `QUICK_TAP`        | 200ms | Selenium QMK constant, returned by `get_quick_tap_term()` for all keys         |

The split is driven by `tap_keycode_is_tap_preferred()` (Selenium QMK helper), which returns `true` for keycodes that correspond to Selenium ZMK's `&hrm`/`&lt` behaviors:

- Letters, numbers, `KC_NO`, `KC_SPACE` — text-producing keys on base layer HRMs and Space thumb
- `KC_MPLY`, `KC_MUTE`, `KC_PSCR` — media/system keys used as HRM taps on the function layer (Selenium ZMK uses `&hrm` for these)

The result:

- **Tap-preferred keys** (HRM letters, Space) → `get_tapping_term()` returns 300ms, `get_hold_on_other_key_press()` returns `false` — hold triggers only on tapping term expiry
- **Hold-preferred keys** (Enter, Escape, Del, layer-taps) → `get_tapping_term()` returns 150ms, `get_hold_on_other_key_press()` returns `true` — hold triggers immediately on any keypress

### How the mapping works

- `TAPPING_TERM = 150` is set as the global default (short, for hold-preferred keys).
- `TAPPING_TERM_PER_KEY` is enabled, and `get_tapping_term()` returns `HRM_TAPPING_TERM` (300ms) for text-producing keys and `TAPPING_TERM` (150ms) for everything else.
- For `QUICK_TAP`: QMK's `action_tapping.h` unconditionally redefines `QUICK_TAP_TERM = TAPPING_TERM` unless `QUICK_TAP_TERM_PER_KEY` is defined. We define `QUICK_TAP_TERM_PER_KEY` and provide `get_quick_tap_term()` returning 200ms.

| Selenium ZMK behavior      | Selenium QMK equivalent                                       | Match                                                     |
| -------------------------- | ------------------------------------------------------------- | --------------------------------------------------------- |
| `&hrm` 300ms tap-preferred | `*_T()` on letter → 300ms, no interrupt-based hold            | See [Hold-tap behavior](#hold-tap-behavior)               |
| `&lt` 300ms tap-preferred  | `LT()` on Space → 300ms, no interrupt-based hold              | See [Hold-tap behavior](#hold-tap-behavior)               |
| `&sc` 150ms hold-preferred | `LT()` on Enter/Escape/Del → 150ms, `HOLD_ON_OTHER_KEY_PRESS` | Approximate (see [Hold-tap behavior](#hold-tap-behavior)) |
| `&mt` 150ms hold-preferred | `*_T()` on Backspace → 150ms, `HOLD_ON_OTHER_KEY_PRESS`       | Approximate (see [Hold-tap behavior](#hold-tap-behavior)) |

## Hold-tap behavior

ZMK natively attaches a `flavor` property to each hold-tap behavior definition. The `flavor` controls how keypresses during the hold influence the tap-or-hold decision. Selenium ZMK defines custom behaviors (`&hrm`, `&sc`) that each set their own `flavor` — see the [timing table](#zmk-model-per-behavior-timing) above.

QMK has no per-behavior system. Instead, QMK natively provides global flags (`PERMISSIVE_HOLD`, `HOLD_ON_OTHER_KEY_PRESS`) with optional per-key callbacks that apply uniformly to all hold-tap keys — or selectively when the `_PER_KEY` variant is enabled. These are documented in [QMK's tap-hold docs](https://docs.qmk.fm/tap_hold).

Selenium QMK uses one QMK native flag (`config.h`):

```c
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
```

### QMK native hold-tap flags

These are all standard QMK features, not Selenium-specific code.

**QMK default (no flags):** A hold-tap key resolves as "hold" only when the tapping term expires while the key is still held. Other keypresses during the hold are ignored — they do not influence the tap-or-hold decision. This is the QMK equivalent of ZMK's `tap-preferred` flavor.

**`HOLD_ON_OTHER_KEY_PRESS` (QMK native, per-key via callback):** If **any other key is pressed** (key down, no release needed) while this key is held, immediately trigger hold — regardless of the tapping term. Requires `HOLD_ON_OTHER_KEY_PRESS_PER_KEY` + a `get_hold_on_other_key_press()` callback.

### How the flag applies in Selenium QMK

`get_hold_on_other_key_press()` (Selenium's implementation of the QMK native callback) returns `true` for non-text keys and `false` for text keys:

| Key type                                      | `HOLD_ON_OTHER_KEY_PRESS` | Effective behavior                                      |
| --------------------------------------------- | ------------------------- | ------------------------------------------------------- |
| **Non-text** (Enter, Escape, Del, layer-taps) | `true` (active)           | Hold on any key down (immediate)                        |
| **Text** (HRM letters, Space)                 | `false` (inactive)        | Hold only on tapping term expiry (300ms); rolling = tap |

### Mapping ZMK native `flavor` values to QMK native flags

| ZMK native `flavor` value | Decision trigger                                              | QMK native equivalent        |
| ------------------------- | ------------------------------------------------------------- | ---------------------------- |
| `tap-preferred`           | Hold only on tapping term expiry; interrupts ignored          | QMK default (no flags)       |
| `hold-preferred`          | Hold on tapping term expiry; tap only on release before term  | QMK default (no flags)       |
| `balanced`                | Hold when another key is pressed **and released** during hold | `PERMISSIVE_HOLD` (not used) |
| `tap-unless-interrupted`  | Tap unless another key is pressed before tapping term         | Not used by Selenium         |
| — (no ZMK equivalent)     | Hold when another key is pressed (down only)                  | `HOLD_ON_OTHER_KEY_PRESS`    |

### Why this combination fits Selenium

Selenium's core insight is that homerow-mods and thumb modifiers have mutually exclusive goals:

1. **Non-text thumb keys (Enter, Escape, layer-taps)** get `HOLD_ON_OTHER_KEY_PRESS`. They almost always want hold when followed by another keypress. The immediate trigger on any key down matches the intent: you press a layer-tap then immediately press a key on that layer. With a 150ms tapping term, the practical difference with ZMK native `hold-preferred` is negligible.

2. **Text keys (homerow mods, Space)** get **no flags** — pure QMK default behavior (hold only on tapping term expiry). This matches ZMK's `tap-preferred` flavor exactly: keypresses during the hold are ignored, and only the 300ms timer can trigger hold. The long tapping term is the safety mechanism — it ensures homerow-mods are never misfired during fast typing.

### Why PERMISSIVE_HOLD is not used

`PERMISSIVE_HOLD` is the QMK equivalent of ZMK's `balanced` flavor. The Selenium specification explicitly rejects this approach:

> _"The common approach for compact keyboard keymaps is to use a single configuration (and timing) for all hold-taps, which is often a compromise between "tap-preferred" and "hold-preferred" (like QMK's "permissive hold"), and then rely on mitigating measures and fine timing adjustments to limit the number of typos. In our experience, this doesn't work reliably for most users and never will."_

The practical problem: during fast typing, finger release order is not deterministic. Redirects (same-hand direction changes like "ANES" on Ergol) and uneven finger speeds can cause a nested keypress pattern (key B pressed and released while key A is still held) even when the typist intends two separate taps. `PERMISSIVE_HOLD` would interpret this as an intentional modifier activation, causing misfires.

The trade-off of not using `PERMISSIVE_HOLD`: intentional modifier use requires holding the key for the full 300ms. This is slower, but it is the conscious design choice that Selenium makes — safety over responsiveness. The 300ms tapping term is the only mechanism that resolves hold, and no keypress pattern can trigger it earlier.

### Remaining gap: ZMK native `hold-preferred` vs QMK native `HOLD_ON_OTHER_KEY_PRESS`

ZMK native `hold-preferred` resolves hold purely on tapping term expiry. Other key presses during the hold do not influence the decision — only the timer matters.

QMK native `HOLD_ON_OTHER_KEY_PRESS` resolves hold immediately on any key down, regardless of the tapping term. It triggers hold _faster_ than ZMK native `hold-preferred`.

This means Selenium QMK's non-text keys (which use `HOLD_ON_OTHER_KEY_PRESS`) behave slightly differently from their Selenium ZMK counterparts (which use `&sc` / `&mt` with `hold-preferred` flavor).

#### Why not use QMK default (no flags) for non-text keys instead?

QMK's **default** behavior (no flags) is actually an exact match for ZMK native `hold-preferred` — hold triggers purely on tapping term expiry. However, with a 150ms tapping term on non-text keys, the practical difference is negligible: you almost always press another key while holding a layer-tap, and 150ms expires near-instantly. The edge case (holding a layer-tap alone for 50-149ms while another key happens to be pressed at the same moment) is extremely unlikely in real typing. `HOLD_ON_OTHER_KEY_PRESS` provides slightly faster response for the common case with no meaningful downside.

## Selenium ZMK behaviors without exact QMK equivalent

Some Selenium ZMK behaviors (custom or reconfigured native) have no direct QMK native counterpart. These were either approximated with Selenium QMK custom keycodes, mapped to equivalent QMK native features, or left unimplemented:

| Selenium ZMK behavior                                           | Selenium QMK approximation               | What's lost                                                                                                                   |
| --------------------------------------------------------------- | ---------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- |
| `EZ_SK(LSHIFT)` (Selenium custom: sticky key hold-tap)          | QMK native `OSM(MOD_LSFT)`               | — (equivalent: QMK native `OSM()` is one-shot on tap and continuous modifier on hold)                                         |
| `sym_shift_altgr` (Selenium custom: shift→AltGr morph)          | QMK native `OSL(_symbols)`               | Shift morph: tapping shift doesn't switch to AltGr                                                                            |
| `EZ_SL` (Selenium custom: hold=momentary, tap=one-shot layer)   | QMK native `OSL(_symbols)`               | — (equivalent: QMK native `OSL()` provides one-shot on tap and momentary on hold)                                             |
| `EZ_LSK(RALT)` (Selenium custom: sticky key on base layer)      | `LSK_RALT` (Selenium QMK custom keycode) | — (equivalent: see [EZ_LSK(RALT)](#ez_lskralt-sticky-altgr-on-base-layer) below)                                              |
| `magic_backspace` / `magic_space` (Selenium custom: mod-morphs) | Not implemented                          | See [Mod-morph decision](#mod-morph-magic_backspacemagic_space) below                                                         |
| `&lt FUNCTION LS(SPACE)` (ZMK native, with shifted tap)         | Not implemented                          | See [Insecable space](#insecable-space) below.                                                                                |
| `&sl { ignore-modifiers; }` (ZMK native, reconfigured)          | QMK native default behavior              | — (equivalent: QMK native `OSL` natively preserves modifiers when chaining OSM→OSL)                                           |
| `&sk { quick-release; }` (ZMK native, reconfigured)             | Similar QMK native default               | Minor timing difference: QMK releases OSM on next key release, ZMK `quick-release` on next key press. Negligible in practice. |

## Mod-morph (magic_backspace/magic_space)

In Selenium ZMK, `magic_space` and `magic_backspace` are custom mod-morph behaviors (using ZMK native `zmk,behavior-mod-morph`) that swap Space ↔ Backspace when Shift is held. The morph is bound to a **specific thumb key** (the secondary thumb — the one opposite the main space key), not globally.

This feature is **not implemented** in Selenium QMK. Two approaches were explored:

### QMK native Key Override (`KEY_OVERRIDE`)

QMK native `KEY_OVERRIDE` can swap keycodes when a modifier is held:

```c
const key_override_t shift_space = ko_make_basic(MOD_MASK_SHIFT, KC_SPC, KC_BSPC);
```

**Problem**: QMK native Key Override is global per keycode — it applies everywhere `KC_SPC` appears, not just on the secondary thumb. This would also morph `KC_BSPC` on the top row of the base layer and `KC_SPC`/`KC_BSPC` in other layers, which Selenium ZMK does not do.

### Custom logic in QMK native `process_record_user`

Intercepting the keycode and checking modifiers manually:

```c
if (tap_kc == KC_SPC && record->tap.count > 0 && (get_mods() & MOD_MASK_SHIFT)) {
    del_mods(MOD_MASK_SHIFT);
    tap_code(KC_BSPC);
    return false;
}
```

**Problem**: Still global per keycode — can't distinguish which physical key was pressed without hardcoding row/column positions, which breaks portability across keyboards.

### Decision

Both approaches affect all instances of the keycode, not just the secondary thumb. In ZMK, mod-morphs are bound to specific key positions via behavior bindings in the keymap — a concept that doesn't exist in QMK, where keycode processing is global. Since there is no clean way to replicate position-specific mod-morphs in QMK, this feature is intentionally left out rather than implementing a global workaround with unintended side effects.

## sym_shift_altgr (Shift→AltGr morph)

In Selenium ZMK, `sym_shift_altgr` is a custom nested mod-morph (using ZMK native `zmk,behavior-mod-morph`) used as the right tuck thumb key (HT_THUMB_TAPS and HT_HOME_ROW_MODS variants):

- Default: `EZ_SL(SYMBOLS_LAYER)` — one-shot/momentary symbols layer
- When Shift is held: `EZ_SK(RALT)` — one-shot/hold AltGr (for typing accented characters via Ergol's AltGr layer)
- When Ctrl/GUI/Alt is held: passes through to the symbols layer

The purpose: on Ergol, this lets you tap Shift (left tuck) then tap the symbol key (right tuck) to get AltGr instead of the symbol layer, giving access to accented characters.

This feature is **not implemented** in Selenium QMK. The right tuck thumb uses `OSL(_symbols)` unconditionally.

### Why not implement it

QMK has no native mod-morph behavior. The closest approach would be to intercept the QMK native `OSL(_symbols)` keypress in `process_record_user`, check if Shift is held via `get_mods()`, and send `OSM(MOD_RALT)` instead. However, `OSL()` is not easily interceptable as a custom keycode — it would require replacing it with a Selenium QMK custom keycode and reimplementing both the one-shot layer and the shift detection manually.

### Decision

The added complexity is not justified. Ergol users can still access AltGr via the dedicated `KC_RALT` key available on several layers. The shift→AltGr shortcut is a convenience, not a necessity.

## Insecable space

In Selenium ZMK, the right thumb home key on the NumLock and NumNav layers uses ZMK native `&lt LAYER LS(SPACE)` — hold activates a layer, tap sends Shift+Space. On Ergol, Shift+Space produces a non-breaking space (espace insécable).

This feature is **not implemented** in Selenium QMK.

QMK native `LT()` only accepts basic keycodes, so `LT(layer, S(KC_SPC))` is not possible. An initial approach intercepted taps in `process_record_user` by checking if the keycode matched `LT(_num_nav, KC_SPC)` or `LT(_function, KC_SPC)`. However, the base layer space key also uses `LT(_num_nav, KC_SPC)` — making it impossible to distinguish a base layer space tap from a NumLock/NumNav layer space tap. This caused every space on the base layer to send Shift+Space (non-breaking space), breaking normal typing.

## OSL nesting bug and SYM_NUM_LAYER

In Selenium ZMK, `EZ_SL(SYMBOLS_LAYER)` expands to `bsl` (a custom hold-tap: hold=momentary, tap=one-shot). QMK native `OSL()` provides the same behavior natively.

However, QMK's `OSL()` state machine breaks when two `OSL()` keys are nested: pressing `SYM_NUM_LAYER` = `OSL(_num_nav)` while `OSL(_symbols)` is active corrupts the first OSL's state, leaving `_symbols` permanently active.

A custom keycode `BSL_SYM` was initially implemented to avoid this nesting bug, using manual `layer_on()`/`layer_off()` for the hold path and manual one-shot tracking for the tap path. However, the manual one-shot implementation could not reliably activate the layer for the next keypress — `layer_on()` called from `process_record_user` during a release event was not seen by the next key's matrix scan.

### Decision

Use QMK native `OSL(_symbols)` for the symbols layer (it works correctly for both tap and hold). Avoid the nesting bug by using `MO()` instead of `OSL()` for `SYM_NUM_LAYER` (the num key on the symbols layer). This means the num key requires holding instead of tapping — a minor deviation from the spec, which says this key should be sticky for flavors with three thumb keys.

## EZ_LSK(RALT) (sticky AltGr on base layer)

In Selenium ZMK, `EZ_LSK(RALT)` is a Selenium macro used on the VimNav and NavNum layers (RT tuck). It expands to `lbsk RALT RALT` (a Selenium custom hold-tap behavior) that:

- **Tap** (Selenium custom `losm`): goes to base layer via ZMK native `&to BASE_LAYER`, then sends one-shot RALT
- **Hold** (Selenium custom `lkp`): goes to base layer via ZMK native `&to BASE_LAYER`, then holds RALT until released

The purpose is to exit the navigation layer and apply AltGr on the base layer, where letter keys are available for typing accented characters (e.g. on Ergol).

In Selenium QMK, this is implemented via a custom keycode `LSK_RALT` with manual tap/hold detection in `process_record_user`:

- On press: `layer_move(_base)` + `register_mods(MOD_BIT(KC_RALT))`
- On release: `unregister_mods(MOD_BIT(KC_RALT))`, and if no other key was pressed during the hold, `set_oneshot_mods(MOD_BIT(KC_RALT))` for one-shot behavior

A static flag (`lsk_ralt_used`) tracks whether another key was pressed while LSK_RALT was held, distinguishing tap from hold.

## Function layer: C_AL_LOCK (screen lock)

In Selenium ZMK, position (2,9) on the function layer uses Selenium custom `&hrm RGUI C_AL_LOCK` — a home-row mod with RGUI on hold and ZMK native `C_AL_LOCK` (HID consumer usage 0x19E, screen lock/screensaver) on tap.

This tap action is **not implemented** in Selenium QMK. QMK defines the HID constant `AL_LOCK = 0x19E` internally but does not map it to any keycode. Implementing it would require a Selenium QMK custom keycode with `host_consumer_send(0x19E)`, which cannot be combined with QMK native `RGUI_T()` (only accepts basic keycodes). The purpose of a dedicated screen lock key is not meaningful enough to justify the complexity — most users lock their screen via OS shortcuts. The position uses plain `KC_RGUI`.

## SYM_NUM_LAYER (number access from symbols layer)

In Selenium ZMK, `SYM_NUM_LAYER` uses Selenium macro `EZ_SL(NUM_LAYER)` — a hold-tap where tap activates the number layer as one-shot (type one number, return to symbols) and hold keeps it active momentarily.

In Selenium QMK, we use `MO(_SE_NUM)` (momentary, hold-only) instead of the spec's sticky behavior. This is a workaround for the [OSL nesting bug](#osl-nesting-bug-and-sym_num_layer): using `OSL()` here would nest with `OSL(_symbols)` and corrupt QMK's one-shot state. `_SE_NUM` resolves to `_num_row` when `VIM_NAVIGATION` is enabled, `_num_nav` otherwise — matching the Selenium specification's layer routing.

**Exception**: for `HT_TWO_THUMB_KEYS`, Selenium ZMK uses Selenium custom `&sc NUM_NAV_LAYER CAPSLOCK` — a `hold-preferred` hold-tap with CapsLock on tap. In Selenium QMK, we use QMK native `LT(_SE_NUM, KC_CAPS)`. Since `KC_CAPS` is not a text-producing key, `get_hold_on_other_key_press()` returns `true`, making it effectively hold-preferred — matching the ZMK behavior.

## Configurable options

All options from the Selenium specification are available in `options.h`:

- **Hold-tap configs**: `HT_NONE`, `HT_THUMB_TAPS`, `HT_HOME_ROW_MODS` (default), `HT_TWO_THUMB_KEYS`
- **VIM_NAVIGATION**: splits num-nav into vim-style navigation + number row layers
- **HRM_SHIFT**: adds shift as a pinky home-row mod
- **LEFT_HAND_SPACE**: swaps space and backspace on thumbs
- **Timing overrides**: `HRM_TAPPING_TERM`, `SHORT_TAPPING_TERM`, `QUICK_TAP`
