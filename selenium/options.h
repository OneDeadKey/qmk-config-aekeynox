// Selenium keymap for QMK
// https://github.com/OneDeadKey/selenium

/******************************************************************************
 * Layout-specific actions and symbols.
 ******************************************************************************/

// Uncomment one of the following to match the layout installed on your OS:

// #define KB_LAYOUT_QWERTY
// #define KB_LAYOUT_QWERTY_LAFAYETTE
// #define KB_LAYOUT_AZERTY
#define KB_LAYOUT_ERGOL
// #define KB_LAYOUT_ERGLACE
// #define KB_LAYOUT_BEPO
// #define KB_LAYOUT_BEPOLAR
// #define KB_LAYOUT_DVORAK

// Uncomment the following line if using a Mac:

// #define MACOS

/******************************************************************************
 * Hold-Taps
 ******************************************************************************/

// Uncomment one of the following lines to pick your preferred hold-tap config:

// #define HT_NONE
// #define HT_THUMB_TAPS
// #define HT_HOME_ROW_MODS  // (default behavior)
// #define HT_TWO_THUMB_KEYS

// Uncomment the following line if your keyboard has exactly 4 thumb keys
// (2 per hand, e.g. Ferris/Sweep). This forces HT_TWO_THUMB_KEYS mode.

// #define FOUR_THUMB_KEYS

// Timing is key! Keep the default value if unsure.
// This defines how long (in ms) a hold-tap key with the "tap-preferred" flavor
// must be held to be considered as a modifier or layer shift. These hold-taps
// are the ones that may produce text, including the space-bar and home-row-mods.
// Keep this value high if you struggle with home-row-mods.

// #define HRM_TAPPING_TERM 300

// This defines how quickly (in ms) you need to press and release a hold-tap
// with the "hold-preferred" flavor for it to be considered a tap. These
// hold-taps include the ones that don't produce text when tapped.
// Keep this value low if you press Enter or Escape by mistake too often.

// #define SHORT_TAPPING_TERM 150

// When tapping then holding the same tap-hold, if the delay between the two
// key presses is lower than `QUICK_TAP`, the tap-hold will hold the tap action
// instead of the hold action. Again, keep the default value if unsure.

// #define QUICK_TAP 200

// For more information on how hold-taps operate, feel free to read ZMK's docs:
// https://zmk.dev/docs/keymaps/behaviors/hold-tap

/******************************************************************************
 * Other Options
 ******************************************************************************/

// Uncomment the following line to split the num-nav layer in two:
//  - one layer for vim-style navigation (right) and GUI shortcuts (left)
//  - one layer for a full number row + easy access to Shift+Number
//  + Escape under the left thumb (direct access)
// Highly recommended for Vim users, obviously. :-)

// #define VIM_NAVIGATION

// Uncomment the following line to enable shift as a pinky HRM.
// Useful for combined shortcuts, but NOT MEANT to type text!

// #define HRM_SHIFT

// [Experimental]
// Uncomment the following line to enable the "mod-hold behavior" on the left
// thumb navigation layer key. When enabled, if LAlt is held while entering the
// navigation layer, LAlt is kept held until the navigation layer is released.
// This enables one-handed Alt+Tab using VIM_PREV/VIM_NEXT on the vim_nav layer,
// but may cause unwanted side effects.

// #define ENABLE_MOD_HOLD_NAVIGATION

#if defined(ENABLE_MOD_HOLD_NAVIGATION) && !defined(VIM_NAVIGATION)
#    error "ENABLE_MOD_HOLD_NAVIGATION requires VIM_NAVIGATION"
#endif

// Uncomment the following line to swap Space and Backspace.
// Beware: this increases the typing load of the left thumb.

// #define LEFT_HAND_SPACE

// PERMISSIVE_HOLD can feel like extra comfort for experienced users, but it
// will be an absolute pain if you are learning home-row-mods. It causes
// misfires during fast typing (especially on redirects and uneven finger
// rolls), and tuning it away is a dead end — the Selenium specification
// explicitly rejects this approach. Disabled by default for good reason.
// Do NOT activate this and then conclude that HRMs "don't work well".

// #define PERMISSIVE_HOLD

/******************************************************************************
 * Outer Column Keys (42-key boards only)
 ******************************************************************************/

// On 42-key boards (e.g. Quacken, Corne), these define the 6th column keys on the
// base layer. Ignored on smaller boards where the outer column doesn't exist.
// Uncomment and edit to customize. Defaults match the Selenium specification.

// #define OC_TL  KC_TAB
// #define OC_TR  KC_BSPC
// #define OC_ML  KC_ESC
// #define OC_MR  KC_ENT
// #define OC_BL  KC_LSFT
// #define OC_BR  KC_RSFT
