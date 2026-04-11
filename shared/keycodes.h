// clang-format off

#pragma once

// ╭─────────────────────────────────────────────────────────╮
// │         Syntaxic sugar and generic definitions          │
// ╰─────────────────────────────────────────────────────────╯

// Generic shorthand definitions
#define AG(keycode) RALT(keycode)
#define XX KC_NO
#define __ KC_TRNS


// ╭─────────────────────────────────────────────────────────╮
// │          Keyboard layout specific declarations          │
// ╰─────────────────────────────────────────────────────────╯

#if defined KB_LAYOUT_QWERTY
#    define AS(stripped_keycode) KC_##stripped_keycode
#elif defined KB_LAYOUT_QWERTY_LAFAYETTE
#    define AS(stripped_keycode) QL_##stripped_keycode
#    include "keymap_qwerty_lafayette.h"
#elif defined KB_LAYOUT_AZERTY
#    define AS(stripped_keycode) FR_##stripped_keycode
#    define SHIFTED_NUMBERS
#    include "keymap_french.h"
#elif defined KB_LAYOUT_ERGOL
#    define AS(stripped_keycode) EL_##stripped_keycode
#    include "keymap_ergol.h"
#    define ODK1_SEQUENCE tap_code(EL_ODK); tap_code(KC_1)
#    define ODK2_SEQUENCE tap_code(EL_ODK); tap_code(KC_2)
#    define ODK3_SEQUENCE tap_code(EL_ODK); tap_code(KC_3)
#    define ODK4_SEQUENCE tap_code(EL_ODK); tap_code(KC_4)
#    define ODK5_SEQUENCE tap_code(EL_ODK); tap_code(KC_5)
#elif defined KB_LAYOUT_ERGLACE
#    define AS(stripped_keycode) EG_##stripped_keycode
#    include "keymap_erglace.h"
#    define ODK1_SEQUENCE tap_code(EG_ODK); tap_code(KC_1)
#    define ODK2_SEQUENCE tap_code(EG_ODK); tap_code(KC_2)
#    define ODK3_SEQUENCE tap_code(EG_ODK); tap_code(KC_3)
#    define ODK4_SEQUENCE tap_code(EG_ODK); tap_code(KC_4)
#    define ODK5_SEQUENCE tap_code(EG_ODK); tap_code(KC_5)
#elif defined KB_LAYOUT_BEPO
#    define AS(stripped_keycode) BP_##stripped_keycode
#    define SHIFTED_NUMBERS
#    include "keymap_bepo.h"
#    define TILDE_SEQUENCE tap_code16(BP_TILD); tap_code(KC_SPC)
#elif defined KB_LAYOUT_BEPOLAR
#    define AS(stripped_keycode) BL_##stripped_keycode
#    include "keymap_bepolar.h"
#elif defined KB_LAYOUT_DVORAK
#    define AS(stripped_keycode) DV_##stripped_keycode
#    include "keymap_dvorak.h"
#elif defined KB_LAYOUT_COLEMAK
#    define AS(stripped_keycode) CM_##stripped_keycode
#    include "keymap_colemak.h"
#elif defined KB_LAYOUT_WORKMAN
#    define AS(stripped_keycode) WK_##stripped_keycode
#    include "keymap_workman.h"
#else
#    error "No `KB_LAYOUT_*` option was found or recognised"
#endif

#ifndef ODK1_SEQUENCE
#    define ODK1_SEQUENCE tap_code(KC_NO)
#endif
#ifndef ODK2_SEQUENCE
#    define ODK2_SEQUENCE tap_code(KC_NO)
#endif
#ifndef ODK3_SEQUENCE
#    define ODK3_SEQUENCE tap_code(KC_NO)
#endif
#ifndef ODK4_SEQUENCE
#    define ODK4_SEQUENCE tap_code(KC_NO)
#endif
#ifndef ODK5_SEQUENCE
#    define ODK5_SEQUENCE tap_code(KC_NO)
#endif

#ifndef TILDE_SEQUENCE
#    define TILDE_SEQUENCE tap_code16(AS(TILD))
#endif


#ifdef SHIFTED_NUMBERS
#    define AS_S0  KC_0
#    define AS_S1  KC_1
#    define AS_S2  KC_2
#    define AS_S3  KC_3
#    define AS_S4  KC_4
#    define AS_S5  KC_5
#    define AS_S6  KC_6
#    define AS_S7  KC_7
#    define AS_S8  KC_8
#    define AS_S9  KC_9
#else
#    define AS_S0  S(KC_0)
#    define AS_S1  S(KC_1)
#    define AS_S2  S(KC_2)
#    define AS_S3  S(KC_3)
#    define AS_S4  S(KC_4)
#    define AS_S5  S(KC_5)
#    define AS_S6  S(KC_6)
#    define AS_S7  S(KC_7)
#    define AS_S8  S(KC_8)
#    define AS_S9  S(KC_9)
#endif
