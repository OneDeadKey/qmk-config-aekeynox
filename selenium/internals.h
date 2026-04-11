#pragma once

#include "../shared/keycodes.h"
#include "../shared/layouts.h"

// Symbols layer access
#define SYMBOLS            MO(_symbols)
#define SYMBOLS_T(keycode) LT(_symbols, keycode)

// Currency symbol (S_MONEY equivalent from ZMK aliases — locale-dependent)
#if defined KB_LAYOUT_ERGOL || defined KB_LAYOUT_ERGLACE
#    define AS_MONEY S(KC_1) // € on Ergol/Erglace
#elif defined KB_LAYOUT_AZERTY
#    define AS_MONEY ALGR(KC_E) // € on AZERTY
#elif defined KB_LAYOUT_BEPO
#    define AS_MONEY BP_EURO // € on Bepo
#else
#    define AS_MONEY AS(DLR) // $ on QWERTY/Lafayette
#endif

// ╭─────────────────────────────────────────────────────────╮
// │               Hold-Tap configuration                    │
// ╰─────────────────────────────────────────────────────────╯

// FOUR_THUMB_KEYS forces HT_TWO_THUMB_KEYS regardless of other HT options
#ifdef FOUR_THUMB_KEYS
#    undef HT_NONE
#    undef HT_THUMB_TAPS
#    undef HT_HOME_ROW_MODS
#    define HT_TWO_THUMB_KEYS
#endif

// Default to HT_HOME_ROW_MODS if no hold-tap config is selected
#if !defined HT_NONE && !defined HT_THUMB_TAPS && !defined HT_HOME_ROW_MODS && !defined HT_TWO_THUMB_KEYS
#    define HT_HOME_ROW_MODS
#endif

// Modifier aliases (swapped for Mac)
#ifdef MAC_MODIFIERS
#    define _GUI_T LALT_T
#    define _CTL_T LGUI_T
#    define _ALT_T LCTL_T
#    define _GUI   KC_LALT
#    define _CTL   KC_LGUI
#    define _ALT   KC_LCTL
#else
#    define _GUI_T LGUI_T
#    define _CTL_T LCTL_T
#    define _ALT_T LALT_T
#    define _GUI   KC_LGUI
#    define _CTL   KC_LCTL
#    define _ALT   KC_LALT
#endif

// Home-row mods (enabled for HT_HOME_ROW_MODS and HT_TWO_THUMB_KEYS)
#if defined HT_HOME_ROW_MODS || defined HT_TWO_THUMB_KEYS
#    define KC_SS _GUI_T(KC_S)
#    define KC_DD _CTL_T(KC_D)
#    define KC_FF _ALT_T(KC_F)
#    define KC_JJ _ALT_T(KC_J)
#    define KC_KK _CTL_T(KC_K)
#    define KC_LL _GUI_T(KC_L)
#else
#    define KC_SS KC_S
#    define KC_DD KC_D
#    define KC_FF KC_F
#    define KC_JJ KC_J
#    define KC_KK KC_K
#    define KC_LL KC_L
#endif

// Shift as pinky HRM
#ifdef HRM_SHIFT
#    define KC_AA   LSFT_T(KC_A)
#    define KC_SCSC RSFT_T(KC_SCLN)
#else
#    define KC_AA   KC_A
#    define KC_SCSC KC_SCLN
#endif

// Layer and keycode aliases based on VIM_NAVIGATION
#ifdef VIM_NAVIGATION
#    define _SE_NAV   _vim_nav
#    define _SE_EXTRA _num_row
#    define _SE_S34   _num_row
#    define _SE_REACH KC_ESC
#else
#    define _SE_NAV   _nav_num
#    define _SE_EXTRA _fn_media
#    define _SE_S34   _nav_num
#    define _SE_REACH KC_TAB
#endif

// Symbols layer thumb for accessing numbers
// ZMK uses NUM_LAYER which is NUM_ROW_LAYER with VIM_NAVIGATION, NAV_NUM_LAYER otherwise
#ifdef VIM_NAVIGATION
#    define _SE_NUM _num_row
#else
#    define _SE_NUM _nav_num
#endif

// MO() instead of OSL() to avoid QMK's OSL nesting bug with OSL(_symbols).
#if defined HT_TWO_THUMB_KEYS && !defined VIM_NAVIGATION
#    define SYM_NUM_LAYER LT(_SE_NUM, KC_CAPS)
#else
#    define SYM_NUM_LAYER MO(_SE_NUM)
#endif

// Thumb key definitions
#if defined HT_NONE
#    define AS_TL_TUCK KC_LALT
#    ifdef MAC_MODIFIERS
#        define AS_TL_HOME  KC_LGUI
#        define AS_TL_REACH KC_LCTL
#    else
#        define AS_TL_HOME  KC_LCTL
#        define AS_TL_REACH KC_LGUI
#    endif
#    define AS_TR_REACH MO(_SE_NAV)
#    define AS_TR_HOME  KC_SPC
#    define AS_TR_TUCK  MO(_symbols)

#elif defined HT_THUMB_TAPS
#    define AS_TL_TUCK OSM(MOD_LSFT)
#    ifdef MAC_MODIFIERS
#        define AS_TL_REACH LCTL_T(_SE_REACH)
#        define AS_TR_REACH LGUI_T(KC_ENT)
#    else
#        define AS_TL_REACH LGUI_T(_SE_REACH)
#        define AS_TR_REACH LALT_T(KC_ENT)
#    endif
#    define AS_TR_TUCK OSL(_symbols)
#    ifdef LEFT_HAND_SPACE
#        define AS_TL_HOME _CTL_T(KC_SPC)
#        define AS_TR_HOME LT(_SE_NAV, KC_BSPC)
#    else
#        define AS_TL_HOME _CTL_T(KC_BSPC)
#        define AS_TR_HOME LT(_SE_NAV, KC_SPC)
#    endif

#elif defined HT_HOME_ROW_MODS
#    define AS_TL_TUCK  OSM(MOD_LSFT)
#    define AS_TL_REACH LT(_SE_EXTRA, _SE_REACH)
#    define AS_TR_REACH LT(_SE_EXTRA, KC_ENT)
#    define AS_TR_TUCK  OSL(_symbols)
#    ifdef LEFT_HAND_SPACE
#        define AS_TL_HOME LT(_SE_NAV, KC_SPC)
#        define AS_TR_HOME LT(_SE_NAV, KC_BSPC)
#    else
#        define AS_TL_HOME LT(_SE_NAV, KC_BSPC)
#        define AS_TR_HOME LT(_SE_NAV, KC_SPC)
#    endif

#elif defined HT_TWO_THUMB_KEYS
#    define AS_TL_TUCK  LSFT_T(_SE_REACH)
#    define AS_TL_REACH AS_TL_TUCK
#    define AS_TR_REACH LT(_symbols, KC_ENT)
#    define AS_TR_TUCK  AS_TR_REACH
#    ifdef LEFT_HAND_SPACE
#        define AS_TL_HOME LT(_SE_NAV, KC_SPC)
#        define AS_TR_HOME LT(_SE_EXTRA, KC_BSPC)
#    else
#        define AS_TL_HOME LT(_SE_NAV, KC_BSPC)
#        define AS_TR_HOME LT(_SE_S34, KC_SPC)
#    endif

#endif
