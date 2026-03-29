#pragma once

#include "../shared/keycodes.h"
#include "../shared/layouts.h"

// Symbols layer access
#ifdef ENABLE_SYMBOLS_LAYER
#    define SYMBOLS            MO(_symbols)
#    define SYMBOLS_T(keycode) LT(_symbols, keycode)
#else
#    define SYMBOLS            KC_RALT
#    define SYMBOLS_T(keycode) RALT_T(keycode)
#endif

// TODO: find better names for those macros ?
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

#ifdef ENABLE_HRM
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

// Extra simple config for beginners with bigger keyboards
#if defined SIMPLE_THUMBS
#    define AS_TL_TUCK  _ALT
#    define AS_TL_HOME  _CTL
#    define AS_TL_REACH _GUI
#    define AS_TR_REACH MO(_num_nav)
#    define AS_TR_HOME  KC_SPC
#    define AS_TR_TUCK  SYMBOLS
#elif defined VIM_NAVIGATION
#    define AS_TL_REACH XX
#    define AS_TR_REACH XX
#    define AS_TL_TUCK  LSFT_T(KC_ESC)
#    define AS_TR_TUCK  SYMBOLS_T(KC_ENT)
#    if defined LEFT_HAND_SPACE
#        define AS_TL_HOME LT(_vim_nav, KC_SPC)
#        define AS_TR_HOME LT(_num_row, KC_BSPC)
#    else
#        define AS_TL_HOME LT(_vim_nav, KC_BSPC)
#        define AS_TR_HOME LT(_num_row, KC_SPC)
#    endif
#else
#    define AS_TL_TUCK  LSFT_T(KC_BSPC)
#    define AS_TL_HOME  LT(_num_nav, KC_BSPC)
#    define AS_TL_REACH XX
#    define AS_TR_REACH XX
#    define AS_TR_HOME  AS_TL_HOME
#    define AS_TR_TUCK  SYMBOLS_T(KC_ENT)
#endif
