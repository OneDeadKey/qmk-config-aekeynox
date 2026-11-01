#pragma once
#include "keycodes_aekeynox.h"

// Colemak-DH host layout, ANSI or ortho variant (Linux xkb: us(colemak_dh) or
// us(colemak_dh_ortho)). Only keys used via AS() in keymaps are defined here.

// Letters used in shortcuts via AS()
#define CD_A KC_A
#define CD_S KC_D
#define CD_W KC_W
#define CD_Y KC_O
#ifdef KB_LAYOUT_COLEMAK_DH_ORTHO
// Bottom row: z x c d v
#    define CD_Z KC_Z
#    define CD_X KC_X
#    define CD_C KC_C
#    define CD_V KC_B
#else
// Bottom row: x c d v z
#    define CD_Z KC_B
#    define CD_X KC_Z
#    define CD_C KC_X
#    define CD_V KC_V
#endif

// Numbers (same as Qwerty)
#define CD_0 KC_0
#define CD_1 KC_1
#define CD_2 KC_2
#define CD_3 KC_3
#define CD_4 KC_4
#define CD_5 KC_5
#define CD_6 KC_6
#define CD_7 KC_7
#define CD_8 KC_8
#define CD_9 KC_9

// Symbols (same as Qwerty unless noted)
#define CD_CIRC KC_CIRC // ^
#define CD_DLR  KC_DLR  // $
#define CD_PERC KC_PERC // %
#define CD_AT   KC_AT   // @
#define CD_AMPR KC_AMPR // &
#define CD_ASTR KC_ASTR // *
#define CD_QUOT KC_QUOT // '
#define CD_GRV  KC_GRV  // `
#define CD_LCBR KC_LCBR // {
#define CD_LPRN KC_LPRN // (
#define CD_RPRN KC_RPRN // )
#define CD_RCBR KC_RCBR // }
#define CD_EQL  KC_EQL  // =
#define CD_BSLS KC_BSLS // backslash
#define CD_PLUS KC_PLUS // +
#define CD_MINS KC_MINS // -
#define CD_SLSH KC_SLSH // /
#define CD_DQUO KC_DQUO // "
#define CD_TILD KC_TILD // ~
#define CD_LBRC KC_LBRC // [
#define CD_RBRC KC_RBRC // ]
#define CD_UNDS KC_UNDS // _
#define CD_HASH KC_HASH // #
#define CD_PIPE KC_PIPE // |
#define CD_EXLM KC_EXLM // !
#define CD_QUES KC_QUES // ?
#define CD_COMM KC_COMM // ,
#define CD_DOT  KC_DOT  // .
#define CD_LABK KC_LABK // <
#define CD_RABK KC_RABK // >
#define CD_SCLN KC_P    // ; is on the Qwerty P key
#define CD_COLN S(KC_P) // :
