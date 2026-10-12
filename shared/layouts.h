// clang-format off

#pragma once

// ╭─────────────────────────────────────────────────────────╮
// │          SELENIUM_LAYOUT — one per board                │
// ╰─────────────────────────────────────────────────────────╯
//
// Each board maps SELENIUM_LAYOUT (3×12 + 6 thumbs: LT1 LT2 LT3, RT3 RT2 RT1)
// onto its QMK LAYOUT macro.
//
// Board-specific cases:
//   - 5th Upper Row → UR_* keys (selenium/internals.h).
//   - Central spacebar: defines THREE_THUMB_KEYS (Arsenik thumb keys).

//  ──────────────────────────< Generic layouts >──────────────────────────

#if defined ONEDEADKEY_LAYOUT_split_3x5_2
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_split_3x5_2(\
             L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25,\
             L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35,\
             L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45,\
                            LT1, LT2,     RT2, RT1\
    )

#elif defined ONEDEADKEY_LAYOUT_split_3x5_3
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_split_3x5_3(\
             L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25,\
             L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35,\
             L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45,\
                       LT1, LT2, LT3,     RT3, RT2, RT1\
    )

#elif defined ONEDEADKEY_LAYOUT_split_3x6_3
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_split_3x6_3(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )

//  ──< Corne v4 ex2: 3x6_3 + 2 inner-column keys per side (top + middle) >──
//  The 4 extra keys are off-spec. Defaults to KC_NO. Override in options.h:
//    #define ODK_EXT_LT KC_TAB   // left  inner-top
//    #define ODK_EXT_LB KC_LCTL  // left  inner-middle
//    #define ODK_EXT_RT KC_BSPC  // right inner-top
//    #define ODK_EXT_RB KC_RSFT  // right inner-middle
#elif defined ONEDEADKEY_LAYOUT_split_3x6_3_ex2
#ifndef ODK_EXT_LT
#    define ODK_EXT_LT KC_NO
#endif
#ifndef ODK_EXT_LB
#    define ODK_EXT_LB KC_NO
#endif
#ifndef ODK_EXT_RT
#    define ODK_EXT_RT KC_NO
#endif
#ifndef ODK_EXT_RB
#    define ODK_EXT_RB KC_NO
#endif
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_split_3x6_3_ex2(\
        LOUT1, L21, L22, L23, L24, L25, ODK_EXT_LT,     ODK_EXT_RT, R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35, ODK_EXT_LB,     ODK_EXT_RB, R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,                             R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,                             RT3, RT2, RT1\
    )

//  ──< Corne v4 ex2: 3x5_3 + 2 inner-column keys per side (top + middle) >──
//  Same ODK_EXT_* overrides as split_3x6_3_ex2 apply.
#elif defined ONEDEADKEY_LAYOUT_split_3x5_3_ex2
#ifndef ODK_EXT_LT
#    define ODK_EXT_LT KC_NO
#endif
#ifndef ODK_EXT_LB
#    define ODK_EXT_LB KC_NO
#endif
#ifndef ODK_EXT_RT
#    define ODK_EXT_RT KC_NO
#endif
#ifndef ODK_EXT_RB
#    define ODK_EXT_RB KC_NO
#endif
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_split_3x5_3_ex2(\
             L21, L22, L23, L24, L25, ODK_EXT_LT,     ODK_EXT_RT, R21, R22, R23, R24, R25,\
             L31, L32, L33, L34, L35, ODK_EXT_LB,     ODK_EXT_RB, R31, R32, R33, R34, R35,\
             L41, L42, L43, L44, L45,                             R41, R42, R43, R44, R45,\
                       LT1, LT2, LT3,                             RT3, RT2, RT1\
    )

#elif defined ONEDEADKEY_LAYOUT_ortho_4x10
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_ortho_4x10(\
             L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25,\
             L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35,\
             L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45,\
             XX,  XX,  LT1, LT2, LT3,     RT3, RT2, RT1, XX,  XX\
    )

#elif defined ONEDEADKEY_LAYOUT_ortho_4x12
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_ortho_4x12(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
        XX,    XX,  XX,  LT1, LT2, LT3,     RT3, RT2, RT1, XX,  XX,  XX\
    )

#elif defined ONEDEADKEY_LAYOUT_ortho_5x10
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_ortho_5x10(\
             UR_1, UR_2, UR_3, UR_4, UR_5,  UR_6, UR_7, UR_8, UR_9, UR_0,\
             L21,  L22,  L23,  L24,  L25,   R21,  R22,  R23,  R24,  R25,\
             L31,  L32,  L33,  L34,  L35,   R31,  R32,  R33,  R34,  R35,\
             L41,  L42,  L43,  L44,  L45,   R41,  R42,  R43,  R44,  R45,\
             XX,   XX,   LT1,  LT2,  LT3,   RT3,  RT2,  RT1,  XX,   XX\
    )

#elif defined ONEDEADKEY_LAYOUT_ortho_5x12
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_ortho_5x12(\
        UR_OL, UR_1, UR_2, UR_3, UR_4, UR_5,    UR_6, UR_7, UR_8, UR_9, UR_0, UR_OR,\
        LOUT1, L21,  L22,  L23,  L24,  L25,     R21,  R22,  R23,  R24,  R25,  ROUT1,\
        LOUT2, L31,  L32,  L33,  L34,  L35,     R31,  R32,  R33,  R34,  R35,  ROUT2,\
        LOUT3, L41,  L42,  L43,  L44,  L45,     R41,  R42,  R43,  R44,  R45,  ROUT3,\
        XX,    XX,   XX,   LT1,  LT2,  LT3,     RT3,  RT2,  RT1,  XX,   XX,   XX\
    )

//  ─────────────────< Custom keyboard-specific layouts >──────────────

//  ──< Planck MIT: 2u spacebar >──
//  Arsenik thumbs: LT1 left of the spacebar, RT2 on it, RT1 right of it.
#elif defined ONEDEADKEY_LAYOUT_planck_mit
#define THREE_THUMB_KEYS
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_planck_mit(\
        LOUT1, L21, L22, L23, L24, L25,        R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,        R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,        R41, R42, R43, R44, R45, ROUT3,\
        XX,  XX,  XX,  XX,  LT1,         RT2,        RT1, XX,  XX,  XX,  XX\
    )

#elif defined ONEDEADKEY_LAYOUT_planck_grid
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    )\
    LAYOUT_planck_grid(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
        XX,    XX,  XX,  LT1, LT2, LT3,     RT3, RT2, RT1, XX,  XX,  XX\
    )

#elif defined ONEDEADKEY_LAYOUT_keebio_iris
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    ) LAYOUT(\
        UR_OL, UR_1, UR_2, UR_3, UR_4, UR_5,        UR_6, UR_7, UR_8, UR_9, UR_0, UR_OR,\
        LOUT1, L21,  L22,  L23,  L24,  L25,         R21,  R22,  R23,  R24,  R25,  ROUT1,\
        LOUT2, L31,  L32,  L33,  L34,  L35,         R31,  R32,  R33,  R34,  R35,  ROUT2,\
        LOUT3, L41,  L42,  L43,  L44,  L45, XX, XX, R41,  R42,  R43,  R44,  R45,  ROUT3,\
                             LT1, LT2, LT3,         RT3,  RT2,  RT1\
    )

//  ──< Lily58: 5 rows × 6 + 2 inner-corner + 4 thumbs per side (58 keys) >──
//  Lily58 variants in QMK expose LAYOUT (unsuffixed); the generator builds
//  distinct ONEDEADKEY_LAYOUT_lily58* names per sub-variant, so we match
//  all known ones here.
//
//  Four keys lie outside the Selenium grid — inner corner below the
//  home row (2, one per side) and an outermost thumb (2, one per side).
//  They default to KC_NO. Override in options.h:
//    #define ODK_EXT_INNER_L     KC_LBRC
//    #define ODK_EXT_INNER_R     KC_RBRC
//    #define ODK_EXT_THUMB_OUT_L KC_LCTL
//    #define ODK_EXT_THUMB_OUT_R KC_RCTL
//
//  Upper row: carries the UR_* and defaults to a num row.
#elif defined(ONEDEADKEY_LAYOUT_lily58) \
   || defined(ONEDEADKEY_LAYOUT_lily58_light) \
   || defined(ONEDEADKEY_LAYOUT_lily58_lite) \
   || defined(ONEDEADKEY_LAYOUT_lily58_glow_enc) \
   || defined(ONEDEADKEY_LAYOUT_lily58_r2g)
#ifndef ODK_EXT_INNER_L
#    define ODK_EXT_INNER_L KC_NO
#endif
#ifndef ODK_EXT_INNER_R
#    define ODK_EXT_INNER_R KC_NO
#endif
#ifndef ODK_EXT_THUMB_OUT_L
#    define ODK_EXT_THUMB_OUT_L KC_NO
#endif
#ifndef ODK_EXT_THUMB_OUT_R
#    define ODK_EXT_THUMB_OUT_R KC_NO
#endif
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    ) LAYOUT(\
        UR_OL, UR_1, UR_2, UR_3, UR_4, UR_5,                                  UR_6, UR_7, UR_8, UR_9, UR_0, UR_OR,\
        LOUT1, L21,  L22,  L23,  L24,  L25,                                   R21,  R22,  R23,  R24,  R25,  ROUT1,\
        LOUT2, L31,  L32,  L33,  L34,  L35,                                   R31,  R32,  R33,  R34,  R35,  ROUT2,\
        LOUT3, L41,  L42,  L43,  L44,  L45, ODK_EXT_INNER_L, ODK_EXT_INNER_R, R41,  R42,  R43,  R44,  R45,  ROUT3,\
        ODK_EXT_THUMB_OUT_L, LT1, LT2, LT3,                                   RT3, RT2, RT1, ODK_EXT_THUMB_OUT_R\
    )

//  ──< Sofle: Lily58 + 1 extra outermost thumb per side (60 keys) >──
//  Sofle's extra thumb (matrix [4,0] left, [9,0] right) sits at the
//  outermost position and is typically wired to a rotary encoder press.
//  Exposes ODK_EXT_THUMB_FAR_L/R on top of the Lily58 ODK_EXT_* set.
//  Defaults to KC_NO. Override in options.h:
//    #define ODK_EXT_INNER_L     KC_LBRC
//    #define ODK_EXT_INNER_R     KC_RBRC
//    #define ODK_EXT_THUMB_OUT_L KC_LCTL
//    #define ODK_EXT_THUMB_OUT_R KC_RCTL
//    #define ODK_EXT_THUMB_FAR_L KC_MUTE  // typically the encoder press
//    #define ODK_EXT_THUMB_FAR_R KC_MPLY
//
//  Upper row: carries the UR_* and defaults to a num row.
//
//  Note: keebart/sofle_choc_pro exposes LAYOUT_split_4x6_5 (not plain
//  LAYOUT), so its auto-detected name is ONEDEADKEY_LAYOUT_split_4x6_5,
//  which is intentionally NOT in the guard below (that community name
//  means 58 keys elsewhere). Use `-layout LAYOUT_sofle` explicitly.
#elif defined(ONEDEADKEY_LAYOUT_sofle) \
   || defined(ONEDEADKEY_LAYOUT_sofle_keyhive) \
   || defined(ONEDEADKEY_LAYOUT_sofle_choc) \
   || defined(ONEDEADKEY_LAYOUT_sofle_pico) \
   || defined(ONEDEADKEY_LAYOUT_splitkb_aurora_sofle_v2) \
   || defined(ONEDEADKEY_LAYOUT_mechboards_sofle_pro) \
   || defined(ONEDEADKEY_LAYOUT_keebart_sofle_choc_pro)
#ifndef ODK_EXT_INNER_L
#    define ODK_EXT_INNER_L KC_NO
#endif
#ifndef ODK_EXT_INNER_R
#    define ODK_EXT_INNER_R KC_NO
#endif
#ifndef ODK_EXT_THUMB_OUT_L
#    define ODK_EXT_THUMB_OUT_L KC_NO
#endif
#ifndef ODK_EXT_THUMB_OUT_R
#    define ODK_EXT_THUMB_OUT_R KC_NO
#endif
#ifndef ODK_EXT_THUMB_FAR_L
#    define ODK_EXT_THUMB_FAR_L KC_NO
#endif
#ifndef ODK_EXT_THUMB_FAR_R
#    define ODK_EXT_THUMB_FAR_R KC_NO
#endif
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    ) LAYOUT(\
        UR_OL, UR_1, UR_2, UR_3, UR_4, UR_5,                                  UR_6, UR_7, UR_8, UR_9, UR_0, UR_OR,\
        LOUT1, L21,  L22,  L23,  L24,  L25,                                   R21,  R22,  R23,  R24,  R25,  ROUT1,\
        LOUT2, L31,  L32,  L33,  L34,  L35,                                   R31,  R32,  R33,  R34,  R35,  ROUT2,\
        LOUT3, L41,  L42,  L43,  L44,  L45, ODK_EXT_INNER_L, ODK_EXT_INNER_R, R41,  R42,  R43,  R44,  R45,  ROUT3,\
        ODK_EXT_THUMB_FAR_L, ODK_EXT_THUMB_OUT_L,\
                             LT1, LT2, LT3,                                   RT3, RT2, RT1,\
        ODK_EXT_THUMB_OUT_R, ODK_EXT_THUMB_FAR_R\
    )

#elif defined(ONEDEADKEY_LAYOUT_keyboardio_atreus)
#ifndef ODK_EXT_INNER_L
#    define ODK_EXT_INNER_L KC_NO
#endif
#ifndef ODK_EXT_INNER_R
#    define ODK_EXT_INNER_R KC_NO
#endif
#ifndef ODK_EXT_THUMB_OUT_L
#    define ODK_EXT_THUMB_OUT_L KC_NO
#endif
#ifndef ODK_EXT_THUMB_OUT_R
#    define ODK_EXT_THUMB_OUT_R KC_NO
#endif
#ifndef ODK_EXT_THUMB_FAR_L
#    define ODK_EXT_THUMB_FAR_L KC_NO
#endif
#ifndef ODK_EXT_THUMB_FAR_R
#    define ODK_EXT_THUMB_FAR_R KC_NO
#endif
#ifndef ODK_EXT_THUMB_FAR_EXT_L
#    define ODK_EXT_THUMB_FAR_EXT_L KC_NO
#endif
#ifndef ODK_EXT_THUMB_FAR_EXT_R
#    define ODK_EXT_THUMB_FAR_EXT_R KC_NO
#endif
#define SELENIUM_LAYOUT(\
        LOUT1, L21, L22, L23, L24, L25,     R21, R22, R23, R24, R25, ROUT1,\
        LOUT2, L31, L32, L33, L34, L35,     R31, R32, R33, R34, R35, ROUT2,\
        LOUT3, L41, L42, L43, L44, L45,     R41, R42, R43, R44, R45, ROUT3,\
                         LT1, LT2, LT3,     RT3, RT2, RT1\
    ) LAYOUT(\
        L21, L22, L23, L24, L25,                                   R21, R22, R23, R24, R25,\
        L31, L32, L33, L34, L35,                                   R31, R32, R33, R34, R35,\
        L41, L42, L43, L44, L45, ODK_EXT_INNER_L, ODK_EXT_INNER_R, R41, R42, R43, R44, R45,\
        ODK_EXT_THUMB_FAR_EXT_L, ODK_EXT_THUMB_FAR_L, ODK_EXT_THUMB_OUT_L,\
                  LT1, LT2, LT3,                                   RT3, RT2, RT1,\
        ODK_EXT_THUMB_OUT_R, ODK_EXT_THUMB_FAR_R, ODK_EXT_THUMB_FAR_EXT_R\
    )

#else
#    error "Your keyboard appears unsupported — open an issue on GitHub to propose an implementation or ask the team for support."
#endif
