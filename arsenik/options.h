//  ────────────────────< Main Arsenik configuration >─────────────────

// Below are a bunch of options to quickly customize the Arsenik keymap. You
// can pick and choose them by (un)commenting the different `#define`
// declarations.

#define ENABLE_SYMBOLS_LAYER
/* Enables the programming symbols layer used by layouts like Ergo‑L and
 * all of the "Lafayette" family of layouts (it's their "AltGr like" layer).
 * It does not cover the full AltGr layer — the AltGr-only glyphs (math
 * signs, dead keys) are out of reach here. It is, however, more portable
 * than native AltGr, which behaves differently across OSes (on Windows
 * AltGr = Ctrl+Alt, so its symbols can't be used in keyboard shortcuts).
 * The definition of this layer depends on the keyboard layout you are using, so
 * make sure to select the correct one in the list below.
 *
 * When inactive, this layer is discarded and replaced by native AltGr, which
 * exposes the host layout's full AltGr layer (those glyphs included).
 */

// #define ENABLE_HRM
/* When active, adds a Meta, Ctrl and Alt home-row-mod on respectively s/l,
 * d/k or f/j on a Qwerty keyboard. Those home-row-mods stay on those exact
 * keys regardless of the layout being used, meaning they would be on r/i, s/e
 * and t/n on a Colemak keyboard.
 *
 * NOT enabling HRMs assumes Meta, Ctrl and Alt are assigned to available slots
 * in your keyboard layout. Edit layouts.h to map those keys to empty slots (XX)
 * in the targeted layout.
 */

// #define MAC_MODIFIERS
/* Swaps around home-row-mods from Meta, Ctrl, Alt to Alt, Meta, Ctrl, as it
 * may make more sense on a Mac, like to keep common shortcuts accessible with
 * the Ergo‑L layout, for instance
 *
 * (Requires `ENABLE_HRM`)
 */

// #define SIMPLE_THUMBS
/* When active, uses an extra simple config without any tap-holds on the thumb
 * keys. It may be a *lot* simpler to use, but overall a lot less efficient,
 * since you need to reach to less comfortable spots for utility keys such as
 * Shift, Backspace or Enter. Those are assigned to their traditional outer
 * column slots in keymap.c; this option assumes your keyboard has them.
 *
 * Base thumb config: LSFT_T(KC_BSPC)  LT(_num_nav, KC_SPC)  RALT_T(KC_ENT)
 * When it's active:     MO(_num_nav)         KC_SPC         KC_RALT
 * (KC_RALT becomes the `_symbols` layer if `ENABLE_SYMBOLS_LAYER`
 * is active)
 *
 * (Prevents using `VIM_NAVIGATION`)
 */

// #define VIM_NAVIGATION
/* For those who like to move the cursor with HJKL in all apps with any keyboard
 * layout, it is possible to enable a Vim-like Navigation layer when the spacebar
 * is held, rather than the Num Nav layer.
 *
 * It also has:
 * - super-comfortable Tab and Shift-Tab
 * - mouse emulation: previous/next and mouse scroll
 */

// #define RESTORE_SPACE
/* Having Space accessible to only one thumb may create some problems, especially
 * when trying to type Shift + Space or Lafayette / AltGr + Space (depending if
 * your space key is on your left or right hand). When active, backspace gets
 * temporarily replaced by space when the original space key is held.
 *
 * (Requires `VIM_NAVIGATION`)
 */

// Lists of layouts supported by Arsenik. Some parts of the config are dependent
// on keyboard layout used on your computer. If they don't match up some
// characters may not be correctly placed or missing entirely. If multiple
// options are toggled at the same time, the first one is chosen.
#define KB_LAYOUT_QWERTY
// #define KB_LAYOUT_AZERTY
// #define KB_LAYOUT_ERGOL
// #define KB_LAYOUT_ERGLACE
// #define KB_LAYOUT_BEPO
// #define KB_LAYOUT_DVORAK
// #define KB_LAYOUT_COLEMAK
// #define KB_LAYOUT_WORKMAN
