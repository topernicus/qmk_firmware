/* Copyright 2017 Colin T.A. Gray
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "xd75.h"
#include "rgblight.h"
#include <util/delay.h>
#include "keymap.h"

enum my_keycodes {
    // sticky keys
    STK_CTL = SAFE_RANGE,
    STK_ALT,
    STK_SHFT,
    STK_GUI,

    // media tap/hold buttons
    TH_M0,
    TH_M1,
    TH_M2,

    // number/fn tap/hold buttons
    TH_1,
    TH_2,
    TH_3,
    TH_4,
    TH_5,
    TH_6,
    TH_7,
    TH_8,
    TH_9,
    TH_0,
    TH_LAST,

    // normal macros
    MM_1,
    MM_2,
    MM_3,
    MM_4,
    DM_CLR,
    DYNAMIC_MACRO_RANGE,
};

#include "dynamic_macro.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* COLEMAK
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | `      | 1      | 2      | 3      | 4      | 5      | DM_1   | DM_2   | KC_DEL | 6      | 7      | 8      | 9      | 0      | -      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | TAB    | Q      | W      | F      | P      | G      | MM_3   | MM_4   | BKSP   | J      | L      | U      | Y      | ;      | =      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | SHIFT  | A      | R      | S      | T      | D      | [      | GOTO_FN| ENTER  | H      | N      | E      | I      | O      | '      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | CTRL   | Z      | X      | C      | V      | B      | ]      | PG UP  | /      | K      | M      | ,      | .      | UP     | \      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------+--------|
 * | VOL-   | PLAY   | VOL+   | ALT    | GUI    | SPACE  | HOME   | PG DN  | END    | SPACE  | ESC    | MENU   | LEFT   | DOWN   | RIGHT  |
 * '--/RRND----/MUTE----/FFWD-------------------------------------------------------------------------------------------------------------'
 */

  [LAYER_COLEMAK] = KEYMAP(
     KC_GRV , TH_1   , TH_2   , TH_3   , TH_4   , TH_5   , DM_1   , DM_2   , KC_DEL , TH_6   , TH_7   , TH_8   , TH_9   , TH_0   , KC_MINS,
     KC_TAB , KC_Q   , KC_W   , KC_F   , KC_P   , KC_G   , MM_3   , MM_4   , KC_BSPC, KC_J   , KC_L   , KC_U   , KC_Y   , KC_SCLN, KC_EQL ,
     STK_SHFT,KC_A   , KC_R   , KC_S   , KC_T   , KC_D   , KC_LBRC, GOTO_FN, KC_ENT , KC_H   , KC_N   , KC_E   , KC_I   , KC_O   , KC_QUOT,
     STK_CTL, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_RBRC, KC_PGUP, KC_SLSH, KC_K   , KC_M   , KC_COMM, KC_DOT , KC_UP  , KC_BSLS,
     TH_M0  , TH_M1  , TH_M2  , STK_ALT, STK_GUI, KC_SPC,  KC_HOME, KC_PGDN, KC_END , KC_SPC , KC_ESC , MMENU  , KC_LEFT, KC_DOWN, KC_RGHT
  ),

/* DYN_REC LAYER - recording tap/hold keys is possible, but they will always "tap" (macros don't record holding duration)
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        |        |        |        |        |        |  STOP  |  STOP  |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * |        |        |        |        |        |        |  STOP  |  STOP  |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * |        |        |        |        |        |        |        |  STOP  |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [LAYER_RECORD_CM] = KEYMAP(
     ______ , ______ , ______ , ______ , ______ , ______ ,  STOP  ,  STOP  , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , ______ , ______ , ______ ,  STOP  ,  STOP  , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , ______ , ______ , ______ , ______ ,  STOP  , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______
  ),

/* QWERTY - Falls through to colemak for most keys, but passes through the colemak recording layer, so that's why those keys are re-defined.
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        |        |        |        |        |        | DM_1   | DM_2   |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * |        | Q      | W      | E      | R      | T      | MM_3   | MM_4   |        | Y      | U      | I      | O      | P      |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | SHIFT  | A      | S      | D      | F      | G      |        | GOTO_FN|        | H      | J      | K      | L      | ;      |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | CTRL   | Z      | X      | C      | V      | B      |        |        |        | N      | M      |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        | ALT    | GUI    |        |        |        |        |        |        |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [LAYER_QWERTY] = KEYMAP(
     ______ , ______ , ______ , ______ , ______ , ______ , DM_1   , DM_2   , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , MM_3   , MM_4   , ______ , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , ______ ,
     MLSFT  , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , ______ , GOTO_FN, ______ , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, ______ ,
     MLCTL  , KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , ______ , ______ , ______ , KC_N   , KC_M   , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , MLALT  , MLGUI  , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______
  ),

  [LAYER_RECORD_QW] = KEYMAP(
     ______ , ______ , ______ , ______ , ______ , ______ ,  STOP  ,  STOP  , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , ______ , ______ , ______ ,  STOP  ,  STOP  , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , ______ , ______ , ______ , ______ ,  STOP  , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ ,
     ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______ , ______
  ),

/* FN LAYER - change layouts and start recording a macro
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | KC_SLEP|        |        |        |        |        | REC 1  | REC 2  |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * |        |        |        |        |        |        |COLEMAK | QWERTY |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * |        |        |        |        |        |        |        | GOTO_FN|        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |DM_CLR  |        |        |        |        |        | RESET  |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [LAYER_FN] = KEYMAP(
     KC_SLEP, KC_NO  , KC_NO  , KC_NO  , KC_NO  ,  KC_NO , REC1   , REC2   , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO ,
     KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,  KC_NO , GOTO_CM, GOTO_QW, KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
     KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,  KC_NO , KC_NO  , GOTO_FN, KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
     KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,  KC_NO , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
     KC_NO  , KC_NO  , KC_NO  , KC_NO  , DM_CLR ,  KC_NO , KC_NO  , KC_NO  , KC_NO  , KC_NO  , RESET  , KC_NO  , KC_NO  , KC_NO  , KC_NO
  )
};

///
/// GLOBAL STATE VARS
///

/// previous layer refers to either QWERTY or Colemak; it is stored when going
/// to the function layer, and restored after a macro recording
static uint32_t prev_layer;

/// Tap hold events - make sure the number of events matches TH_EVENTS_COUNT, and the TH_M0 ... TH_0
/// keys must be sequential and in the same order.
static tap_hold_t th_events[] = {
    { .is_pressed = false, .timer = 0, .kc_tap = KC_VOLD, .kc_hold = KC_MRWD }, // TH_M0
    { .is_pressed = false, .timer = 0, .kc_tap = KC_MPLY, .kc_hold = KC_MUTE }, // TH_M1
    { .is_pressed = false, .timer = 0, .kc_tap = KC_VOLU, .kc_hold = KC_MFFD }, // TH_M2

    { .is_pressed = false, .timer = 0, .kc_tap = KC_1, .kc_hold = KC_F1 }, // TH_1
    { .is_pressed = false, .timer = 0, .kc_tap = KC_2, .kc_hold = KC_F2 }, // TH_2
    { .is_pressed = false, .timer = 0, .kc_tap = KC_3, .kc_hold = KC_F3 }, // TH_3
    { .is_pressed = false, .timer = 0, .kc_tap = KC_4, .kc_hold = KC_F4 }, // TH_4
    { .is_pressed = false, .timer = 0, .kc_tap = KC_5, .kc_hold = KC_F5 }, // TH_5
    { .is_pressed = false, .timer = 0, .kc_tap = KC_6, .kc_hold = KC_F6 }, // TH_6
    { .is_pressed = false, .timer = 0, .kc_tap = KC_7, .kc_hold = KC_F7 }, // TH_7
    { .is_pressed = false, .timer = 0, .kc_tap = KC_8, .kc_hold = KC_F8 }, // TH_8
    { .is_pressed = false, .timer = 0, .kc_tap = KC_9, .kc_hold = KC_F9 }, // TH_9
    { .is_pressed = false, .timer = 0, .kc_tap = KC_0, .kc_hold = KC_F10 } // TH_0
};

/// RGB KEYS
/// Store an rgblight_fadeout for each LED; these are turned on and fade out after every key press
static rgblight_fadeout lights[RGBLED_NUM];
static bool rgb_animations = true;

/// STICKY KEYS
// sticky key state - bit masks of MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) | ...
// sticky_lock - same but stays on - double tap to activate
static uint8_t sticky_state = 0;
static uint8_t sticky_lock = 0;
static uint16_t sticky_timer;
// actual "mod key pressed" state, which "overrides" the sticky state. "Sticky"
// state only lasts for one key press, but as long as the modifier key is held
// it will also continue to be active.
static uint8_t mods_down_state = 0;

/// DYNAMIC MACROS
// if the dynamic macros haven't been recorded, send default macro strings.
static bool did_record_m1 = false;
static bool did_record_m2 = false;

void matrix_init_user(void) {
    rgblight_mode(1);
    rgblight_setrgb(0, 0, 0);
    rgblight_enable();
    gp103_led_on();
    _delay_ms(250);
    gp103_led_off();

    for (uint8_t light_index = 0 ; light_index < RGBLED_NUM ; ++light_index ) {
        rgblight_fadeout *light = &lights[light_index];
        light->enabled = false;
    }
}

void taphold_tapped(uint8_t index, bool pressed) {
    if (index >= TH_EVENTS_COUNT) { return; }

    tap_hold_t *th_event = &th_events[index];

    if (pressed) {
        th_event->timer = timer_read();
        th_event->is_pressed = true;
    } else if (th_event->is_pressed) {
        register_code(th_event->kc_tap);
        unregister_code(th_event->kc_tap);
        th_event->is_pressed = false;
    }
}

void matrix_scan_user(void) {
    scan_tap_hold();
    scan_rgblight_fadeout();
}

void scan_tap_hold(void) {
    for (uint8_t index = 0 ; index < TH_EVENTS_COUNT ; ++index ) {
        tap_hold_t *th_event = &th_events[index];
        if ( th_event->is_pressed && timer_elapsed(th_event->timer) > LONGPRESS_DELAY) {
            register_code(th_event->kc_hold);
            unregister_code(th_event->kc_hold);
            th_event->is_pressed = false;
        }
    }
}

void scan_rgblight_fadeout(void) {
    if (!rgb_animations) { return; }

    bool litup = false;
    for (uint8_t light_index = 0 ; light_index < RGBLED_NUM ; ++light_index ) {
        if (lights[light_index].enabled && timer_elapsed(lights[light_index].timer) > 10) {
            rgblight_fadeout *light = &lights[light_index];
            rgblight_setrgb_at(light->r, light->g, light->b, light_index);
            litup = true;

            if (light->r || light->g || light->b) {
                light->life -= 1;
                light->r = light->r ? light->r - 1 : light->r;
                light->g = light->g ? light->g - 1 : light->g;
                light->b = light->b ? light->b - 1 : light->b;
                light->timer = timer_read();
            }
            else {
                light->enabled = false;
            }
        }
    }

    if (litup) {
        rgblight_set();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == RESET) {
        rgblight_setrgb(0xFF, 0xFF, 0);
        gp103_led_on();
        return KBD_CONTINUE;
    }

    if (KBD_HALT == process_record_user_rgb(keycode, record) ||
        KBD_HALT == process_record_user_fnlayer(keycode, record) ||
        KBD_HALT == process_record_user_sticky(keycode, record) ||
        KBD_HALT == process_record_user_taphold(keycode, record) ||
        KBD_HALT == process_record_user_dyn_macro(keycode, record) ||
        KBD_HALT == process_record_user_macro(keycode, record) ||
        false)
    {
        return KBD_HALT;
    }

    return KBD_CONTINUE;
}

bool process_record_user_rgb(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) { return KBD_CONTINUE; }

    switch (keycode) {
    case KC_A ... KC_SLASH:
    case KC_F1 ... KC_F12:
    case KC_INSERT ... KC_UP:
    case KC_KP_SLASH ... KC_KP_DOT:
    case KC_F13 ... KC_F24:
    case KC_AUDIO_MUTE ... KC_MEDIA_REWIND:
        start_rgb_light();
    }

    return KBD_CONTINUE;
}

void start_rgb_light(void) {
    uint8_t indices[RGBLED_NUM];
    uint8_t indices_count = 0;
    uint8_t min_life = 0xFF;
    uint8_t min_life_index = -1;
    for (uint8_t index = 0 ; index < RGBLED_NUM ; ++index ) {
        if (lights[index].enabled) {
            if (min_life_index == -1 ||
                lights[index].life < min_life)
            {
                min_life = lights[index].life;
                min_life_index = index;
            }
            continue;
        }

        indices[indices_count] = index;
        ++indices_count;
    }

    uint8_t light_index;
    if (!indices_count) {
        light_index = min_life_index;
    }
    else {
        light_index = indices[rand() % indices_count];
    }

    rgblight_fadeout *light = &lights[light_index];
    light->enabled = true;
    light->timer = timer_read();
    light->life = 0xFF;

    light->r = rand() % 0xFF;
    light->g = rand() % 0xFF;
    light->b = rand() % 0xFF;

    rgblight_setrgb_at(light->r, light->g, light->b, light_index);
}

bool process_record_user_fnlayer(uint16_t keycode, keyrecord_t *record) {
    if (keycode == GOTO_FN) {
        sticky_state = 0;
        sticky_lock = 0;
        gp100_led_off();

        if (layer_state_is(LAYER_COLEMAK) || layer_state_is(LAYER_QWERTY)) {
            prev_layer = layer_state;
        }
    }
    return KBD_CONTINUE;
}

bool process_record_user_sticky(uint16_t keycode, keyrecord_t *record) {
    uint8_t modkey = 0;
    switch (keycode) {
    case STK_CTL:
        modkey = KC_LCTL;
        break;
    case STK_ALT:
        modkey = KC_LALT;
        break;
    case STK_SHFT:
        modkey = KC_LSHIFT;
        break;
    case STK_GUI:
        modkey = KC_LGUI;
        break;
    default:
        sticky_state = 0;
        break;
    }

    if (!modkey) { return KBD_CONTINUE; }

    uint8_t modkey_mask = MOD_BIT(modkey);
    if (record->event.pressed) {
        register_code(modkey);

        if (sticky_lock & modkey_mask) {
            sticky_state &= ~modkey_mask;
            sticky_lock  &= ~modkey_mask;
            gp100_led_on();
        }
        else if (sticky_state & modkey_mask) {
            if (timer_elapsed(sticky_timer) < STICKY_DELAY) {
                sticky_lock |= modkey_mask;
                gp100_led_on();
            }
            else {
                sticky_state &= ~modkey_mask;
                gp100_led_on();
            }
        }
        else {
            sticky_state |= modkey_mask;
            sticky_timer = timer_read();
        }

        mods_down_state |= modkey_mask;
    }
    else {
        unregister_code(modkey);

        mods_down_state &= ~modkey_mask;

        if (!(mods_down_state | sticky_lock)) {
            gp100_led_off();
        }
    }

    set_mods(mods_down_state | sticky_state | sticky_lock);

    return KBD_HALT;
}

bool process_record_user_taphold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case TH_M0 ... TH_LAST:
        taphold_tapped(keycode - TH_M0, record->event.pressed);
        return KBD_HALT;
    }
    return KBD_CONTINUE;
}

bool process_record_user_dyn_macro(uint16_t keycode, keyrecord_t *record) {
    if (keycode == DM_CLR) {
        did_record_m1 = false;
        did_record_m2 = false;
        return KBD_CONTINUE;
    }

    if ((keycode == DM_1 && !did_record_m1) || (keycode == DM_2 && !did_record_m2)) { return KBD_CONTINUE; }
    if (process_record_dynamic_macro(keycode, record)) { return KBD_CONTINUE; }

    if (keycode == DYN_REC_START1) {
        did_record_m1 = true;
    }

    if (keycode == DYN_REC_START2) {
        did_record_m2 = true;
    }

    if (keycode == DYN_REC_START1 || keycode == DYN_REC_START2) {
        gp103_led_on();
        if (layer_state_cmp(prev_layer, LAYER_COLEMAK)) {
            layer_move(LAYER_RECORD_CM);
        }
        else if (layer_state_cmp(prev_layer, LAYER_QWERTY)) {
            layer_move(LAYER_RECORD_QW);
        }
    }
    else if (keycode == DYN_REC_STOP) {
        gp103_led_off();
        layer_move(prev_layer);
    }

    return KBD_HALT;
}

bool process_record_user_macro(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) { return KBD_CONTINUE; }

    switch (keycode) {
    case DM_1: if (did_record_m1) { return KBD_HALT; }
    case MM_1:
        SEND_STRING(SENDSTRING_MM1);
        return KBD_HALT;

    case DM_2: if (did_record_m2) { return KBD_HALT; }
    case MM_2:
        SEND_STRING(SENDSTRING_MM2);
        return KBD_HALT;

    case MM_3:
        SEND_STRING(SENDSTRING_MM3);
        return KBD_HALT;
    case MM_4:
        SEND_STRING(SENDSTRING_MM4);
        return KBD_HALT;
    }

    return KBD_CONTINUE;
}

void process_record_after(keyrecord_t *record) {
    if (!sticky_state && get_mods()) {
        set_mods(mods_down_state | sticky_lock);
    }

    if (layer_state_is(LAYER_FN)) {
        rgb_animations = false;
        rgblight_setrgb(0xFF, 0xFF, 0xFF);
        keycaps_led_on();
    }
    else if (!rgb_animations) {
        rgb_animations = true;
        rgblight_setrgb(0, 0, 0);
        keycaps_led_off();
    }

    if ((sticky_state | sticky_lock) > 0) {
        capslock_led_on();
    }
    else {
        capslock_led_off();
    }
}
