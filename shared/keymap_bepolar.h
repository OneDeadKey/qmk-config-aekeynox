#pragma once
#include "keycodes.h"

// Bepolar host layout — derived from zmk-config-aekeynox keymaps/aliases/bepolar.h

// Letters used in shortcuts via AS()
#define BL_A KC_A
#define BL_C KC_H
#define BL_S KC_K
#define BL_V KC_U
#define BL_W KC_T
#define BL_X KC_C
#define BL_Y KC_Q
#define BL_Z KC_Y

// Numbers (same physical positions as QWERTY)
#define BL_0 KC_0
#define BL_1 KC_1
#define BL_2 KC_2
#define BL_3 KC_3
#define BL_4 KC_4
#define BL_5 KC_5
#define BL_6 KC_6
#define BL_7 KC_7
#define BL_8 KC_8
#define BL_9 KC_9

// Symbols — first row: ^<>$% @&*'`
#define BL_CIRC ALGR(KC_Q)
#define BL_LABK ALGR(KC_W)
#define BL_RABK ALGR(KC_E)
#define BL_DLR  KC_BSLS
#define BL_PERC KC_EQL
#define BL_AT   S(KC_6)
#define BL_AMPR ALGR(KC_U)
#define BL_ASTR S(KC_0)
#define BL_QUOT KC_QUOT
#define BL_GRV  ALGR(KC_P)

// Symbols — second row: {()}= \+-/"
#define BL_LCBR S(KC_LBRC)
#define BL_LPRN S(KC_4)
#define BL_RPRN S(KC_5)
#define BL_RCBR S(KC_RBRC)
#define BL_EQL  KC_MINS
#define BL_BSLS S(KC_NUBS)
#define BL_PLUS S(KC_7)
#define BL_MINS KC_Z
#define BL_SLSH KC_NUBS
#define BL_DQUO S(KC_QUOT)

// Symbols — third row: ~[]_# |!;:?
#define BL_TILD ALGR(KC_Z)
#define BL_LBRC KC_LBRC
#define BL_RBRC KC_RBRC
#define BL_UNDS ALGR(KC_V)
#define BL_HASH S(KC_1)
#define BL_PIPE ALGR(KC_N)
#define BL_EXLM S(KC_Z)
#define BL_SCLN S(KC_G)
#define BL_COLN S(KC_V)
#define BL_QUES S(KC_N)

// Extra
#define BL_COMM KC_G
#define BL_DOT  KC_V
