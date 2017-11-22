/* Copyright 2016 Jack Humbert
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

/* Author: Wojciech Siewierski < wojciech dot siewierski at onet dot pl > */
#ifndef DYNAMIC_MACROS_H
#define DYNAMIC_MACROS_H

#include "action_layer.h"

#ifndef DYNAMIC_MACRO_SIZE
/* May be overridden with a custom value. Be aware that the effective
 * macro length is half of this value: each keypress is recorded twice
 * because of the down-event and up-event. This is not a bug, it's the
 * intended behavior.
 *
 * Usually it should be fine to set the macro size to at least 256 but
 * there have been reports of it being too much in some users' cases,
 * so 128 is considered a safe default.
 */
#define DYNAMIC_MACRO_SIZE 128
#endif

/* DYNAMIC_MACRO_RANGE must be set as the last element of user's
 * "planck_keycodes" enum prior to including this header. This allows
 * us to 'extend' it.
 */
enum dynamic_macro_keycodes {
    DYN_REC_START1 = DYNAMIC_MACRO_RANGE,
    DYN_REC_START2,
    DYN_REC_STOP,
    DYN_MACRO_PLAY1,
    DYN_MACRO_PLAY2,
};

/* Blink the LEDs to notify the user about some event. */
void dynamic_macro_led_blink(void)
{
#ifdef BACKLIGHT_ENABLE
    backlight_toggle();
    wait_ms(100);
    backlight_toggle();
#endif
}

/* Convenience macros used for retrieving the debug info. All of them
 * need a `direction` variable accessible at the call site.
 */
#define DYNAMIC_MACRO_CURRENT_SLOT() (direction > 0 ? 1 : 2)
#define DYNAMIC_MACRO_CURRENT_LENGTH(BEGIN, POINTER) \
    ((int)(direction * ((POINTER) - (BEGIN))))
#define DYNAMIC_MACRO_CURRENT_CAPACITY(BEGIN, END2) \
    ((int)(direction * ((END2) - (BEGIN)) + 1))

/**
 * Start recording of the dynamic macro.
 *
 * @param[out] macro_pointer The new macro buffer iterator.
 * @param[in]  macro_buffer  The macro buffer used to initialize macro_pointer.
 */
void dynamic_macro_record_start(
    keyrecord_t **macro_pointer, keyrecord_t *macro_buffer)
{
    dprintln("dynamic macro recording: started");

    dynamic_macro_led_blink();

    clear_keyboard();
    layer_clear();
    *macro_pointer = macro_buffer;
}

/**
 * Play the dynamic macro.
 *
 * @param macro_buffer[in] The beginning of the macro buffer being played.
 * @param macro_end[in]    The element after the last macro buffer element.
 * @param direction[in]    Either +1 or -1, which way to iterate the buffer.
 */
void dynamic_macro_play(
    keyrecord_t *macro_buffer, keyrecord_t *macro_end, int8_t direction)
{
    dprintf("dynamic macro: slot %d playback\n", DYNAMIC_MACRO_CURRENT_SLOT());

    uint32_t saved_layer_state = layer_state;

    clear_keyboard();
    layer_clear();

    while (macro_buffer != macro_end) {
        process_record(macro_buffer);
        macro_buffer += direction;
    }

    clear_keyboard();

    layer_state = saved_layer_state;
}

/**
 * Record a single key in a dynamic macro.
 *
 * @param macro_buffer[in] The start of the used macro buffer.
 * @param macro_pointer[in,out] The current buffer position.
 * @param macro2_end[in] The end of the other macro.
 * @param direction[in]  Either +1 or -1, which way to iterate the buffer.
 * @param record[in]     The current keypress.
 */
void dynamic_macro_record_key(
    keyrecord_t *macro_buffer,
    keyrecord_t **macro_pointer,
    keyrecord_t *macro2_end,
    int8_t direction,
    keyrecord_t *record)
{
    /* If we've just started recording, ignore all the key releases. */
    if (!record->event.pressed && *macro_pointer == macro_buffer) {
        dprintln("dynamic macro: ignoring a leading key-up event");
        return;
    }

    /* The other end of the other macro is the last buffer element it
     * is safe to use before overwriting the other macro.
     */
    if (*macro_pointer - direction != macro2_end) {
        **macro_pointer = *record;
        *macro_pointer += direction;
    } else {
        dynamic_macro_led_blink();
    }

    dprintf(
        "dynamic macro: slot %d length: %d/%d\n",
        DYNAMIC_MACRO_CURRENT_SLOT(),
        DYNAMIC_MACRO_CURRENT_LENGTH(macro_buffer, *macro_pointer),
        DYNAMIC_MACRO_CURRENT_CAPACITY(macro_buffer, macro2_end));
}

/**
 * End recording of the dynamic macro. Essentially just update the
 * pointer to the end of the macro.
 */
void dynamic_macro_record_end(
    keyrecord_t *macro_buffer,
    keyrecord_t *macro_pointer,
    int8_t direction,
    keyrecord_t **macro_end)
{
    dynamic_macro_led_blink();

    /* Do not save the keys being held when stopping the recording,
     * i.e. the keys used to access the layer DYN_REC_STOP is on.
     */
    while (macro_pointer != macro_buffer &&
           (macro_pointer - direction)->event.pressed) {
        dprintln("dynamic macro: trimming a trailing key-down event");
        macro_pointer -= direction;
    }

    dprintf(
        "dynamic macro: slot %d saved, length: %d\n",
        DYNAMIC_MACRO_CURRENT_SLOT(),
        DYNAMIC_MACRO_CURRENT_LENGTH(macro_buffer, macro_pointer));

    *macro_end = macro_pointer;
}

/* READY   - no macro is being recorded right now
 * RECORD_L, RECORD_R - either macro 1 or 2 is being recorded */
enum {
  MACRO_STATE_READY = 0,
  MACRO_STATE_RECORD_L,
  MACRO_STATE_RECORD_R
};

enum {
    MACRO_DIRECTION_LEFT = 1,
    MACRO_DIRECTION_RIGHT = -1,
};

typedef struct {
  uint16_t play_l;
  uint16_t play_r;
  uint16_t record_l;
  uint16_t record_r;
  keyrecord_t *l_buffer;
  keyrecord_t *l_end;
  keyrecord_t *r_buffer;
  keyrecord_t *r_end;
  uint8_t state;
} dynamic_macro_t;

/* Creates a new dynamic macro storage struct, initialized with the default size.
 * When you call process_record_dynamic_macro(keycode, record), a default macro storage will be
 * used, if you need more than 2 dynamic macros you can create another struct and pass it to
 * process_record_dynamic_macro_helper().
 *
 * If you have a custom keycodes of "PLAY_M1, PLAY_M2, RECORD_M1, RECORD_M2", the process_record_user() code
 * would look like this:
 *
 *     // the keycodes for play and record are paired because two macros are
 *     // store in one buffer (see below).  The default macro uses DYN_REC_START1 and DYN_MACRO_PLAY1.
 *     static dynamic_macro_t *my_dynamic_macro = NULL;
 *     if (!my_dynamic_macro) {
 *         create_dynamic_macro(
 *             PLAY_M1, PLAY_M2,
 *             RECORD_M1, RECORD_M2
 *             );
 *     }
 *
 *     if (!process_record_dynamic_macro_helper(keycode, record, my_dynamic_macro)) {
 *         return false;
 *     }
 *
 * If you ever need to reclaim the memory of a dynamic macro, use free() on the macro struct.
 *
 *     free(my_dynamic_macro);
 */
dynamic_macro_t *create_dynamic_macro(
    uint16_t play_l,
    uint16_t play_r,
    uint16_t record_l,
    uint16_t record_r
    ) {
    /* Both macros use the same buffer but read/write on different
     * ends of it.
     *
     * Macro1 is written left-to-right starting from the beginning of
     * the buffer.
     *
     * Macro2 is written right-to-left starting from the end of the
     * buffer.
     *
     * &macro_buffer   macro_end
     *  v                   v
     * +------------------------------------------------------------+
     * |>>>>>> MACRO1 >>>>>>      <<<<<<<<<<<<< MACRO2 <<<<<<<<<<<<<|
     * +------------------------------------------------------------+
     *                           ^                                 ^
     *                         r_macro_end                  r_macro_buffer
     *
     * During the recording when one macro encounters the end of the
     * other macro, the recording is stopped. Apart from this, there
     * are no arbitrary limits for the macros' length in relation to
     * each other: for example one can either have two medium sized
     * macros or one long macro and one short macro. Or even one empty
     * and one using the whole buffer.
     */
    keyrecord_t macro_buffer[DYNAMIC_MACRO_SIZE];

    /* Pointer to the first buffer element after the first macro.
     * Initially points to the very beginning of the buffer since the
     * macro is empty. */
    keyrecord_t *macro_end = macro_buffer;

    /* The other end of the macro buffer. Serves as the beginning of
     * the second macro. */
    keyrecord_t *const r_macro_buffer = macro_buffer + DYNAMIC_MACRO_SIZE - 1;

    /* Like macro_end but for the second macro. */
    keyrecord_t *r_macro_end = r_macro_buffer;

    dynamic_macro_t *new_macro = (dynamic_macro_t *)malloc(sizeof(dynamic_macro_t));

    new_macro->play_l = play_l;
    new_macro->play_r = play_r;
    new_macro->record_l = record_l;
    new_macro->record_r = record_r;
    new_macro->l_buffer = macro_buffer;
    new_macro->l_end = macro_end;
    new_macro->r_buffer = r_macro_buffer;
    new_macro->r_end = r_macro_end;
    new_macro->state = MACRO_STATE_READY;

    return new_macro;
}

bool process_record_dynamic_macro_helper(uint16_t keycode, keyrecord_t *record, dynamic_macro_t *macro);

/* Handle the key events related to the dynamic macros. Should be
 * called from process_record_user() like this:
 *
 *   bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 *       if (!process_record_dynamic_macro(keycode, record)) {
 *           return false;
 *       }
 *       <...THE REST OF THE FUNCTION...>
 *   }
 */
bool process_record_dynamic_macro(uint16_t keycode, keyrecord_t *record) {
    static dynamic_macro_t *default_macro = NULL;
    if (!default_macro) {
        default_macro = create_dynamic_macro(
            DYN_MACRO_PLAY1, DYN_MACRO_PLAY2,
            DYN_REC_START1, DYN_REC_START2
        );
    }

    return process_record_dynamic_macro_helper(keycode, record, default_macro);
}

bool process_record_dynamic_macro_helper(uint16_t keycode, keyrecord_t *record, dynamic_macro_t *macro) {
    /* A persistent pointer to the current macro position (iterator)
     * used during the recording. */
    static keyrecord_t *macro_pointer = NULL;

    if (macro->state == MACRO_STATE_READY) {
        if (!record->event.pressed) {
            /* No macro recording in progress and macro or recored key is released. */
            if (keycode == macro->record_l) {
                dynamic_macro_record_start(&macro_pointer, macro->l_buffer);
                macro->state = MACRO_STATE_RECORD_L;
                return false;
            }
            else if (keycode == macro->record_r) {
                dynamic_macro_record_start(&macro_pointer, macro->r_buffer);
                macro->state = MACRO_STATE_RECORD_R;
                return false;
            }
            else if (keycode == macro->play_l) {
                dynamic_macro_play(macro->l_buffer, macro->l_end, MACRO_DIRECTION_LEFT);
                return false;
            }
            else if (keycode == macro->play_r) {
                dynamic_macro_play(macro->r_buffer, macro->r_end, MACRO_DIRECTION_RIGHT);
                return false;
            }
        }
    } else if (keycode == DYN_REC_STOP){
        /* Stop the macro recording. */
        if (record->event.pressed) { /* Ignore the initial release
                                      * just after the recoding
                                      * starts. */
            switch (macro->state) {
            case MACRO_STATE_RECORD_L:
                dynamic_macro_record_end(macro->l_buffer, macro_pointer, MACRO_DIRECTION_LEFT, &macro->l_end);
                break;
            case MACRO_STATE_RECORD_R:
                dynamic_macro_record_end(macro->r_buffer, macro_pointer, MACRO_DIRECTION_RIGHT, &macro->r_end);
                break;
            }
            macro->state = MACRO_STATE_READY;
            macro_pointer = NULL;
        }
        return false;
    } else if (keycode == macro->play_l || keycode == macro-> play_r) {
        dprintln("dynamic macro: ignoring macro play key while recording");
        return false;
    } else {
        /* A macro is being recorded right now. */
        /* Store the key in the macro buffer and process it normally. */
        if (macro->state == macro->record_l) {
            dynamic_macro_record_key(macro->l_buffer, &macro_pointer, macro->r_end, MACRO_DIRECTION_LEFT, record);
        }
        else {
            dynamic_macro_record_key(macro->r_buffer, &macro_pointer, macro->l_end, MACRO_DIRECTION_RIGHT, record);
        }
    }

    return true;
}

#undef DYNAMIC_MACRO_CURRENT_SLOT
#undef DYNAMIC_MACRO_CURRENT_LENGTH
#undef DYNAMIC_MACRO_CURRENT_CAPACITY

#endif
