#include "acr60.h"

#define MODS_CTRL_MASK  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))

enum custom_keycodes
{
  SHRUG = SAFE_RANGE,
  FUCKTHISONE,
  FLPTBL,
  FIXTBL,
  NGRYTBL,
  SRSLY,
  EMOJI_LENNY,
  EMOJI_LENNYX,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
 * Default keymap with standard 60% directional layout. Added Media keys and modifier keys on second layer.
 * Extra special use of locking leds to create an underlight caps/mute indicator.
 */
	KEYMAP_DIRECTIONAL(
		KC_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_NO, KC_BSPC,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
		KC_CAPSLOCK, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
		KC_LSFT, KC_NO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_RSFT, KC_UP, KC_SLSH,
		KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_SPC, KC_SPC, KC_RALT, MO(1), KC_LEFT, KC_DOWN, KC_RGHT),

	KEYMAP_DIRECTIONAL(
		KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_DEL,
		KC_TRNS, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RESET,
		KC_TRNS, SHRUG, FLPTBL, FIXTBL, NGRYTBL, SRSLY, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, BL_DEC, BL_TOGG, BL_INC, BL_STEP, KC_MUTE, KC_VOLD, KC_VOLU, KC_TRNS, KC_PGUP, KC_INS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_END),

	KEYMAP_DIRECTIONAL(
		KC_TRNS, M(1), M(2), M(3), M(4), M(5), M(6), M(7), M(8), M(9), M(10), M(11), M(12), KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};

bool caps_is_active = false;
bool audio_is_muted = false;

void toggle_capslock_indicator(void)
{
  caps_is_active = host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK);
  if (caps_is_active)
  {
    rgblight_lock_rgb_at(255, 0, 0, 2);
    rgblight_lock_rgb_at(255, 0, 0, 19);
  }
  else
  {
    rgblight_unlock_rgb_at(2);
    rgblight_unlock_rgb_at(19);
  }
}
void toggle_mute_indicator(void)
{
  audio_is_muted = !audio_is_muted;
  if (audio_is_muted)
  {
    rgblight_lock_rgb_at(0, 255, 0, 14);
  }
  else
  {
    rgblight_unlock_rgb_at(14);
  }
}

void matrix_init_user(void)
{
  set_unicode_input_mode(UC_WIN);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
          case KC_CAPSLOCK:
            register_code(KC_CLCK);
            unregister_code(KC_CLCK);
            send_keyboard_report();
            toggle_capslock_indicator();
            return false; break;
          case KC_MUTE:
            register_code(KC_MUTE);
            unregister_code(KC_MUTE);
            send_keyboard_report();
            toggle_mute_indicator();
            return false; break;
          case SHRUG:
            process_unicode((0x00AF|QK_UNICODE), record); // hand
            SEND_STRING("\\"); // escape
            process_unicode((0x005C|QK_UNICODE), record); // arm
            SEND_STRING("\\"); // escape
            process_unicode((0x005F|QK_UNICODE), record); // shoulder
            process_unicode((0x0028|QK_UNICODE), record); // left head
            process_unicode((0x30C4|QK_UNICODE), record); // face
            process_unicode((0x0029|QK_UNICODE), record); // right head
            SEND_STRING("\\"); // escape
            process_unicode((0x005F|QK_UNICODE), record); // shoulder
            process_unicode((0x002F|QK_UNICODE), record); // arm
            process_unicode((0x00AF|QK_UNICODE), record); // hand
            //SEND_STRING(R"(¯\_(ツ)_/¯)");
            return false; break;
          case FLPTBL:
            process_unicode((0x0028|QK_UNICODE), record); // head
            process_unicode((0x256F|QK_UNICODE), record); // arm
            process_unicode((0x00B0|QK_UNICODE), record); // eye
            process_unicode((0x25A1|QK_UNICODE), record); // mouth
            process_unicode((0x00B0|QK_UNICODE), record); // eye
            process_unicode((0x0029|QK_UNICODE), record); // head
            process_unicode((0x256F|QK_UNICODE), record); // arm
            //process_unicode((0x09F2|QK_UNICODE), record); // flip
            register_code(KC_RALT);
            SEND_STRING(SS_TAP(X_KP_PLUS) SS_TAP(X_F) SS_TAP(X_E) SS_TAP(X_KP_3) SS_TAP(X_KP_5));
            unregister_code(KC_RALT);
            process_unicode((0x0020|QK_UNICODE), record); // space
            process_unicode((0x253B|QK_UNICODE), record); // table
            process_unicode((0x2501|QK_UNICODE), record); // table
            process_unicode((0x253B|QK_UNICODE), record); // table
            //SEND_STRING(R"((╯°□°）╯︵ ┻━┻)");
            return false; break;
          case FIXTBL:
            process_unicode((0x252C|QK_UNICODE), record); // table
            process_unicode((0x2500|QK_UNICODE), record); // table
            process_unicode((0x252C|QK_UNICODE), record); // table
            process_unicode((0x30CE|QK_UNICODE), record); // arm
            process_unicode((0x0028|QK_UNICODE), record); // head
            process_unicode((0x0020|QK_UNICODE), record); // space
            process_unicode((0x00BA|QK_UNICODE), record); // eye
            process_unicode((0x0020|QK_UNICODE), record); // space
            process_unicode((0x005F|QK_UNICODE), record); // mouth
            process_unicode((0x0020|QK_UNICODE), record); // space
            process_unicode((0x00BA|QK_UNICODE), record); // eye
            process_unicode((0x30CE|QK_UNICODE), record); // arm
            process_unicode((0x0029|QK_UNICODE), record); // head
            //SEND_STRING(R"(┬─┬ノ( º _ ºノ))");
            return false; break;
          case NGRYTBL:
            process_unicode((0x0028|QK_UNICODE), record); // head
            process_unicode((0x30CE|QK_UNICODE), record); // arm
            process_unicode((0x0CA0|QK_UNICODE), record); // eye
            process_unicode((0x76CA|QK_UNICODE), record); // mouth
            process_unicode((0x0CA0|QK_UNICODE), record); // eye
            process_unicode((0x0029|QK_UNICODE), record); // head
            process_unicode((0x30CE|QK_UNICODE), record); // arm
            process_unicode((0x5F61|QK_UNICODE), record); // whoosh
            process_unicode((0x253B|QK_UNICODE), record); // table
            process_unicode((0x2501|QK_UNICODE), record); // table
            process_unicode((0x253B|QK_UNICODE), record); // table
            //SEND_STRING(R"((ノಠ益ಠ)ノ彡┻━┻)");
            return false; break;
          case SRSLY:
            process_unicode((0x0CA0|QK_UNICODE), record); // eye
            process_unicode((0x005F|QK_UNICODE), record); // mouth
            process_unicode((0x0CA0|QK_UNICODE), record); // eye
            return false; break;
          // case EMOJI_LENNY:
          //   SEND_STRING(R"(( ͡° ͜ʖ ͡°))");
          //   return false; break;
          // case EMOJI_LENNYX:
          //   SEND_STRING(R"(( ͡⚆ ͜ʖ ͡⚆))");
          //   return false; break;
        }
    }
    return true;
}



// const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
// {
//   // MACRODOWN only works in this function
//   switch (id)
//   {
//     case 0:
//       if (record->event.pressed)
//       {
//         register_code(KC_CLCK);
//         unregister_code(KC_CLCK);
//         caps_is_active = !caps_is_active;
//         capslock_led_init();
//         if (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK))//(caps_is_active)
//         {
//           rgblight_lock_rgb_at(255, 0, 0, 2);
//           rgblight_lock_rgb_at(255, 0, 0, 19);
//         }
//         else
//         {
//           rgblight_unlock_rgb_at(2);
//           rgblight_unlock_rgb_at(19);
//         }
//       }
//       break;
//     case 1:
//       if (record->event.pressed)
//       {
//         register_code(KC_MUTE);
//         unregister_code(KC_MUTE);
//         audio_is_muted = !audio_is_muted;
//         if (audio_is_muted)
//         {
//           rgblight_lock_rgb_at(0, 255, 0, 14);
//         }
//         else
//         {
//           rgblight_unlock_rgb_at(14);
//         }
//       }
//       break;
//   }
//   return MACRO_NONE;
// }



