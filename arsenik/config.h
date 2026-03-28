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

#define ARSENIK_HRM_TAPPING_TERM 300
/* This is the delay used by Arsenik for sensitive tap-holds, which include
 * home-row-mods and mod-taps using the spacebar
 */

#undef PERMISSIVE_HOLD
/* QMK's `PERMISSIVE_HOLD` can be some extra comfort for experienced users, but
 * can also be an absolute pain for beginners, especially if you are trying to
 * learn home-row-mods, which is why we chose to deactivate this option by
 * default.
 */

#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
/* QMK considers tap-hold actions as "tap by default" when another key is
 * pressed during the "quantum period", but some tap-holds should ideally be
 * "hold by default" to avoid this delay or accidentally pressing keys like
 * Enter or Escape.
 *
 * This option allows us to have a fine-grain controll over this behaviour.
 *
 * Arsenik provides a good default implementation of the required function
 * `get_hold_on_other_key_press` in the `keymap.c` file.
 */

#define TAPPING_TERM_PER_KEY
/* Tap-hold actions (especially home-row-mods) may be hard to use as a
 * beginner, as it is common to hold those keys for too long and accidentally
 * use the `hold-action`.
 *
 * This option allows us to have a fine-grain controll over the `TAPPING_TERM`
 * for each tap-hold key, allowing for a longer delay on those sensitive keys
 * without slowing down the safer ones.
 *
 * Arsenik provides a good default implementation of the required function
 * `get_tapping_term` in the `keymap.c` file.
 */
