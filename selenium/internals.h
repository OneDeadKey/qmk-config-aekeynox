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
#elif defined KB_LAYOUT_BEPOLAR
#    define AS_MONEY S(KC_BSLS) // € on Bepolar
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

// OS-aware shortcut prefix: Cmd on Mac, Ctrl elsewhere.
// AS() further adapts the keycode to the active host keyboard layout.
#ifdef MAC_MODIFIERS
#    define _SC(kc) G(AS(kc))
#else
#    define _SC(kc) C(AS(kc))
#endif

#define SC_UNDO  _SC(Z)
#define SC_CUT   _SC(X)
#define SC_COPY  _SC(C)
#define SC_PASTE _SC(V)
#define SC_REDO  _SC(Y)
#define SC_CTL_W _SC(W)
#define SC_SAVE  _SC(S)
#define SC_ALL   _SC(A)

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
#    define LTHUMB_TUCK KC_LALT
#    ifdef MAC_MODIFIERS
#        define LTHUMB_HOME  KC_LGUI
#        define LTHUMB_REACH KC_LCTL
#    else
#        define LTHUMB_HOME  KC_LCTL
#        define LTHUMB_REACH KC_LGUI
#    endif
#    define RTHUMB_REACH MO(_SE_NAV)
#    define RTHUMB_HOME  KC_SPC
#    define RTHUMB_TUCK  MO(_symbols)

#elif defined HT_THUMB_TAPS
#    define LTHUMB_TUCK OSM(MOD_LSFT)
#    ifdef MAC_MODIFIERS
#        define LTHUMB_REACH LCTL_T(_SE_REACH)
#        define RTHUMB_REACH LGUI_T(KC_ENT)
#    else
#        define LTHUMB_REACH LGUI_T(_SE_REACH)
#        define RTHUMB_REACH LALT_T(KC_ENT)
#    endif
#    define RTHUMB_TUCK OSL(_symbols)
#    ifdef LEFT_HAND_SPACE
#        define LTHUMB_HOME _CTL_T(KC_SPC)
#        define RTHUMB_HOME LT(_SE_NAV, KC_BSPC)
#    else
#        define LTHUMB_HOME _CTL_T(KC_BSPC)
#        define RTHUMB_HOME LT(_SE_NAV, KC_SPC)
#    endif

#elif defined HT_HOME_ROW_MODS
#    define LTHUMB_TUCK  OSM(MOD_LSFT)
#    define LTHUMB_REACH LT(_SE_EXTRA, _SE_REACH)
#    define RTHUMB_REACH LT(_SE_EXTRA, KC_ENT)
#    define RTHUMB_TUCK  OSL(_symbols)
#    ifdef LEFT_HAND_SPACE
#        define LTHUMB_HOME LT(_SE_NAV, KC_SPC)
#        define RTHUMB_HOME LT(_SE_NAV, KC_BSPC)
#    else
#        define LTHUMB_HOME LT(_SE_NAV, KC_BSPC)
#        define RTHUMB_HOME LT(_SE_NAV, KC_SPC)
#    endif

#elif defined HT_TWO_THUMB_KEYS
#    define LTHUMB_TUCK  LSFT_T(_SE_REACH)
#    define LTHUMB_REACH LTHUMB_TUCK
#    define RTHUMB_REACH LT(_symbols, KC_ENT)
#    define RTHUMB_TUCK  RTHUMB_REACH
#    ifdef LEFT_HAND_SPACE
#        define LTHUMB_HOME LT(_SE_NAV, KC_SPC)
#        define RTHUMB_HOME LT(_SE_EXTRA, KC_BSPC)
#    else
#        define LTHUMB_HOME LT(_SE_NAV, KC_BSPC)
#        define RTHUMB_HOME LT(_SE_S34, KC_SPC)
#    endif

#endif

// _nav_num layer thumb cluster
#ifdef HT_TWO_THUMB_KEYS
#    define NAV_LTHUMB_TUCK  S(KC_TAB)
#    define NAV_LTHUMB_REACH __
#else
#    define NAV_LTHUMB_TUCK  KC_CAPS
#    define NAV_LTHUMB_REACH S(KC_TAB)
#endif

// ╭─────────────────────────────────────────────────────────╮
// │       Outer column keys (42-key boards only)            │
// ╰─────────────────────────────────────────────────────────╯
// Defaults match the Selenium specification. Override in options.h.

#ifndef OC_TL
#    define OC_TL KC_TAB
#endif
#ifndef OC_TR
#    define OC_TR KC_BSPC
#endif
#ifndef OC_ML
#    define OC_ML KC_ESC
#endif
#ifndef OC_MR
#    define OC_MR KC_ENT
#endif
#ifndef OC_BL
#    define OC_BL KC_LSFT
#endif
#ifndef OC_BR
#    define OC_BR KC_RSFT
#endif
