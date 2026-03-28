// QMK requires this file to be named config.h.
// User options are in options.h — edit that file to customize your keymap.

#pragma once
#include "options.h"

/******************************************************************************
 * QMK-specific settings (usually no need to change below)
 ******************************************************************************/

#define ONEDEADKEY_PLACEHOLDER_LAYOUT
/* NOTE: This line gets automatically filled in by the generator, but the
 * underlying layout may not yet exist, and keyboard specific layout may have
 * other variants you might want to check out.
 */

// Host layout mapping
#if defined KB_LAYOUT_ERGOL
#    define ARSENIK_HOST_LAYOUT_ERGOL
#elif defined KB_LAYOUT_AZERTY
#    define ARSENIK_HOST_LAYOUT_AZERTY
#elif defined KB_LAYOUT_QWERTY_LAFAYETTE
#    define ARSENIK_HOST_LAYOUT_QWERTY
#else
#    define ARSENIK_HOST_LAYOUT_QWERTY
#endif

// Mac modifier swap
#ifdef MACOS
#    define ARSENIK_MAC_MODIFIERS
#endif

// Timing defaults and QMK mapping
#ifndef HRM_TAPPING_TERM
#    define HRM_TAPPING_TERM 300
#endif

#ifndef SHORT_TAPPING_TERM
#    define SHORT_TAPPING_TERM 150
#endif
#define TAPPING_TERM SHORT_TAPPING_TERM

#ifndef QUICK_TAP
#    define QUICK_TAP 200
#endif
// QMK's action_tapping.h unconditionally sets QUICK_TAP_TERM = TAPPING_TERM
// unless QUICK_TAP_TERM_PER_KEY is defined. We use that to set our own value.
#define QUICK_TAP_TERM_PER_KEY

// QMK per-key hold-tap settings
#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define TAPPING_TERM_PER_KEY
