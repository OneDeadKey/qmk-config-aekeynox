#include QMK_KEYBOARD_H
#include "internals.h"

enum arsenik_layers {
    _base,
    _num_lock,
    _symbols,
    _vim_nav,
    _nav_num,
    _num_row,
    _fn_media,
};

enum custom_keycodes {
    TILDE = SAFE_RANGE, // ~
    ODK_1,              // „
    ODK_2,              // "
    ODK_3,              // "
    ODK_4,              // ¢
    ODK_5,              // ‰
    LSK_RALT,           // EZ_LSK(RALT): go to base layer, then sticky RALT
    VIM_PREV,           // Alt+Left; morphs to Shift+Grave when LAlt/LGUI is held
    VIM_NEXT,           // Alt+Right; morphs to Grave when LAlt/LGUI is held
};

// QMK implementation of the Selenium specification.
// Thumb keys are configurable via HT_*, VIM_NAVIGATION, and LEFT_HAND_SPACE
// options in options.h. See internals.h for the thumb key definitions.
// Selenium is a 42-key spec: physical row 1 (if present on the board) is
// transparent on every layer — injected by the SELENIUM_LAYOUT facade.
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // 0. Base layer
    [_base] = SELENIUM_LAYOUT(
        OC_TL,    KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,        KC_Y,  KC_U,   KC_I,     KC_O,    KC_P,     OC_TR,
        OC_ML,    KC_AA,  KC_SS,  KC_DD,  KC_FF,  KC_G,        KC_H,  KC_JJ,  KC_KK,    KC_LL,   KC_SCSC,  OC_MR,
        OC_BL,    KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,        KC_N,  KC_M,   KC_COMM,  KC_DOT,  KC_SLSH,  OC_BR,

              LTHUMB_TUCK,  LTHUMB_HOME,  LTHUMB_REACH,        RTHUMB_REACH,  RTHUMB_HOME,  RTHUMB_TUCK
    ),

    // 1. NumLock layer -- sticky NavNum that stays on until deactivated
    [_num_lock] = SELENIUM_LAYOUT(
        __,  KC_ESC,    KC_HOME,   KC_UP,     KC_END,    KC_PGUP,         TO(_base),  AS(7),  AS(8),  AS(9),  AS(SLSH),  __,
        __,  AS(EQL),   KC_LEFT,   KC_DOWN,   KC_RGHT,   KC_PGDN,         AS(MINS),   AS(4),  AS(5),  AS(6),  AS(0),     __,
        __,  AS_MONEY,  AS(COLN),  AS(ASTR),  AS(PLUS),  AS(PERC),        AS(COMM),   AS(1),  AS(2),  AS(3),  AS(DOT),   __,

                              __,  LT(_nav_num, KC_BSPC),  KC_TAB,        __,  LT(_nav_num, KC_SPC),  OSL(_symbols)
    ),

    // 2. Symbols layer -- programming symbols (AltGr layer for Ergol)
    [_symbols] = SELENIUM_LAYOUT(
        __,  AS(CIRC),  AS(LABK),  AS(RABK),  AS(DLR),   AS(PERC),        AS(AT),    AS(AMPR),  AS(ASTR),  AS(QUOT),  AS(GRV),   __,
        __,  AS(LCBR),  AS(LPRN),  AS(RPRN),  AS(RCBR),  AS(EQL),         AS(BSLS),  AS(PLUS),  AS(MINS),  AS(SLSH),  AS(DQUO),  __,
        __,  TILDE,     AS(LBRC),  AS(RBRC),  AS(UNDS),  AS(HASH),        AS(PIPE),  AS(EXLM),  AS(SCLN),  AS(COLN),  AS(QUES),  __,

                                  SYM_NUM_LAYER,  KC_SPC,  KC_ENT,        __,  __,  __
    ),

    // 3. VimNav layer -- HJKL arrow cluster + GUI shortcuts (not accessible by default)
    [_vim_nav] = SELENIUM_LAYOUT(
        __,  XX,       SC_CTL_W,  VIM_PREV,    VIM_NEXT,    XX,             KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   KC_DEL,  __,
        __,  SC_ALL,   SC_SAVE,   S(KC_TAB),   KC_TAB,      XX,             KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  XX,      __,
        __,  SC_UNDO,  SC_CUT,    SC_COPY,     SC_PASTE,    SC_REDO,        XX,       XX,       XX,       XX,       XX,      __,

                     KC_CAPS,  LT(_fn_media, KC_DEL),  MO(_num_row),        __,  MO(_fn_media),  LSK_RALT
    ),

    // 4. NavNum layer -- inverted T navigation + numpad
    [_nav_num] = SELENIUM_LAYOUT(
        __,  KC_ESC,   KC_HOME,  KC_UP,    KC_END,    KC_PGUP,        TO(_num_lock),  AS(7),  AS(8),  AS(9),  AS(SLSH),  __,
        __,  SC_ALL,   KC_LEFT,  KC_DOWN,  KC_RGHT,   KC_PGDN,        AS(MINS),       AS(4),  AS(5),  AS(6),  AS(0),     __,
        __,  SC_UNDO,  SC_CUT,   SC_COPY,  SC_PASTE,  SC_REDO,        AS(COMM),       AS(1),  AS(2),  AS(3),  AS(DOT),   __,

    NAV_LTHUMB_TUCK,  LT(_fn_media, KC_DEL),  NAV_LTHUMB_REACH,       KC_ESC,  LT(_fn_media, KC_SPC),  LSK_RALT
    ),

    // 5. NumRow layer -- numbers on homerow (not accessible by default)
    [_num_row] = SELENIUM_LAYOUT(
        __,  AS_S1,  AS_S2,  AS_S3,  AS_S4,  AS_S5,        AS_S6,     AS_S7,     AS_S8,    AS_S9,     AS_S0,     __,
        __,  AS(1),  AS(2),  AS(3),  AS(4),  AS(5),        AS(6),     AS(7),     AS(8),    AS(9),     AS(0),     __,
        __,  XX,     XX,     XX,     XX,     XX,           AS(MINS),  AS(COMM),  AS(DOT),  AS(COLN),  AS(SLSH),  __,

                               __,  S(KC_SPC),  __,        __,  S(KC_SPC),  KC_RALT
    ),

    // 6. FnMedia layer -- F1..12 + media controls + modifiers on right homerow
    [_fn_media] = SELENIUM_LAYOUT(
        __,  KC_F1,  KC_F2,   KC_F3,   KC_F4,   XX,        XX,  KC_MNXT,          KC_VOLU,          KC_BRIU,  KC_SCRL,          __,
        __,  KC_F5,  KC_F6,   KC_F7,   KC_F8,   XX,        XX,  _ALT_T(KC_MPLY),  _CTL_T(KC_MUTE),  _GUI_T(KC_NO),  LSFT_T(KC_PSCR),  __,
        __,  KC_F9,  KC_F10,  KC_F11,  KC_F12,  XX,        XX,  KC_MPRV,          KC_VOLD,          KC_BRID,  KC_INS,           __,

                                 __,  QK_BOOT,  __,        __,  QK_RBT,  __
    ),

};
// clang-format on

// EZ_LSK(RALT) state: go to base layer, then sticky RALT.
// Hold = continuous RALT on base; tap = one-shot RALT on base.
static bool lsk_ralt_held = false;
static bool lsk_ralt_used = false;

// VIM_PREV / VIM_NEXT: mod-morph one-handed window cycling.
// With LAlt or LGUI held: send Shift+Grave / Grave (keeping the held mod).
// Otherwise: send Alt+Left / Alt+Right (or Cmd+[ / Cmd+] on Mac).
static void vim_prev_action(void) {
    const uint8_t mods = get_mods();
    if (mods & (MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI))) {
        register_code(KC_LSFT);
        tap_code(KC_GRAVE);
        unregister_code(KC_LSFT);
    } else {
#ifdef MAC_MODIFIERS
        register_code(KC_LGUI);
        tap_code(KC_LBRC);
        unregister_code(KC_LGUI);
#else
        register_code(KC_LALT);
        tap_code(KC_LEFT);
        unregister_code(KC_LALT);
#endif
    }
}

static void vim_next_action(void) {
    const uint8_t mods = get_mods();
    if (mods & (MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI))) {
        tap_code(KC_GRAVE);
    } else {
#ifdef MAC_MODIFIERS
        register_code(KC_LGUI);
        tap_code(KC_RBRC);
        unregister_code(KC_LGUI);
#else
        register_code(KC_LALT);
        tap_code(KC_RGHT);
        unregister_code(KC_LALT);
#endif
    }
}

#ifdef ENABLE_MOD_HOLD_NAVIGATION
// Mods pinned for the lifetime of the nav-layer thumb hold (0 = inactive).
// These match VIM_PREV / VIM_NEXT's mod-morph trigger, which fires on LAlt or
// LGUI: pin LGUI on Mac, LAlt elsewhere, so pinning actually engages the morph.
#    ifdef MAC_MODIFIERS
#        define MHN_MODS_TO_PIN (MOD_BIT(KC_LGUI))
#    else
#        define MHN_MODS_TO_PIN (MOD_BIT(KC_LALT))
#    endif

static uint8_t mhn_pinned_mods = 0;
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Track whether another key was pressed while LSK_RALT is held
        if (lsk_ralt_held && keycode != LSK_RALT) { lsk_ralt_used = true; }
    }

#ifdef ENABLE_MOD_HOLD_NAVIGATION
    if (keycode == LTHUMB_HOME) {
        if (record->event.pressed) {
            // Pin whichever target mods are held at the instant the thumb goes down.
            mhn_pinned_mods = get_mods() & MHN_MODS_TO_PIN;
        } else if (mhn_pinned_mods) {
            unregister_mods(mhn_pinned_mods);
            mhn_pinned_mods = 0;
        }
    }
#endif

    // NOTE: Insecable space (Shift+Space for Ergol) is NOT implemented.
    // The base layer space uses LT(_nav_num, KC_SPC), which shares the same
    // keycode as the NumLock/NavNum space — making it impossible to distinguish
    // which layer the tap originated from. See IMPLEMENTATION.md for details.

    if (record->event.pressed) {
        switch (keycode) {
            case TILDE: TILDE_SEQUENCE; return false;
            case ODK_1: ODK1_SEQUENCE; return false;
            case ODK_2: ODK2_SEQUENCE; return false;
            case ODK_3: ODK3_SEQUENCE; return false;
            case ODK_4: ODK4_SEQUENCE; return false;
            case ODK_5: ODK5_SEQUENCE; return false;
            case VIM_PREV: vim_prev_action(); return false;
            case VIM_NEXT: vim_next_action(); return false;
            case LSK_RALT:
                layer_move(_base);
                register_mods(MOD_BIT(KC_RALT));
                lsk_ralt_held = true;
                lsk_ralt_used = false;
                return false;
        }
    } else {
        switch (keycode) {
            case LSK_RALT:
                unregister_mods(MOD_BIT(KC_RALT));
                if (!lsk_ralt_used) { set_oneshot_mods(MOD_BIT(KC_RALT)); }
                lsk_ralt_held = false;
                return false;
        }
    }

    return true;
}

#ifdef ENABLE_MOD_HOLD_NAVIGATION
// Runs after QMK's default processing: re-assert the pinned mods so any
// HRM/mod-tap release during the thumb hold can't clear them before the next
// HID report. Decoupling the pin from its source key means it survives no
// matter which key supplied the mod (KC_FF, KC_JJ, ...) or when it is released.
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (mhn_pinned_mods) { register_mods(mhn_pinned_mods); }
}
#endif

// Returns whether a hold-tap keycode should resolve as tap-preferred:
//  - true  → long tapping term (HRM_TAPPING_TERM), no hold-on-other-key-press
//  - false → short tapping term (TAPPING_TERM), hold-on-other-key-press
//
// The aekeynox/selenium spec splits hold-taps into two groups:
//   - HRMs and the Space layer-tap fire constantly inside normal
//     typing flow; rolling onto the next key must never be misread as a
//     hold. They favor tap.
//   - Layer-taps on non-text thumb keys (Enter, Escape, Backspace, Tab)
//     are pressed in chord with the next key. They favor hold.
//
// The enumerated keycodes below are the tap-targets of every key in the
// first group.
static inline bool tap_keycode_is_tap_preferred(uint16_t keycode) {
    // Custom keycodes (LSK_RALT, …) carry their own hold/tap logic.
    if (keycode >= SAFE_RANGE) return false;

    // Strip the modifier/layer bits to recover the underlying tap keycode.
    const uint16_t tap_keycode = keycode & 0xff;

    switch (tap_keycode) {
        // Space thumb LT
        case KC_SPACE:

        // base-layer HRMs
        case KC_S:
        case KC_D:
        case KC_F:
        case KC_J:
        case KC_K:
        case KC_L:

        // placeholder used by HRMs whose tap is a complex action
        case KC_NO:

        // function-layer media HRMs
        case KC_MPLY:
        case KC_MUTE:
        case KC_PSCR:

#ifdef HRM_SHIFT
        // pinky HRMs (HRM_SHIFT)
        case KC_A:
        case KC_SCLN:
#endif

            // tap-preferred
            return true;
    }

    // hold-preferred
    return false;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return tap_keycode_is_tap_preferred(keycode) ? HRM_TAPPING_TERM : TAPPING_TERM;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    return !tap_keycode_is_tap_preferred(keycode);
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    return QUICK_TAP;
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A:
        case KC_B:
        case KC_D ... KC_M:
        case KC_P ... KC_Z:
        case KC_SCLN:
        case KC_COMM:
        case KC_SLSH:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_C:
        case KC_O:
        case KC_BSPC:
        case KC_DEL:
        case KC_RIGHT:
        case KC_LEFT:
        case KC_UNDS: return true;

        default: return false; // Deactivate Caps Word.
    }
}
