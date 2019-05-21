/* Copyright 2018 Jumail Mundekkat
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
#include QMK_KEYBOARD_H

void matrix_init_user(void) {

}


// Friendly Layers
#define _BASE 0		// Base layer, mouse keys
#define _NUM 1		// Num Pad
#define _EX 2			// Game directory and general functions
#define _GP0 3		// Vermintide 2
#define _GP1 4		// Unused
#define _GP2 5		// Unused

// Muh keys
#define QM_NT LCTL(KC_T)  // new tab
#define QM_PT LCTL(LSFT(KC_T))  // restore deleted tab
#define COPY LCTL(KC_C)
#define PASTE LCTL(KC_V)
#define QM_NEW LCTL(KC_N)
#define QM_STAB LSFT(KC_TAB)  // shift+tab for Steam overlay

// Macro Keys
enum custom_keycodes {
  QMKBEST = SAFE_RANGE,
  QMKURL,
  QM_00,
  ALT_TAB,
  QM_DN,  // dismiss notifications
	QM_ATCL  // autoclick
};

// Tap Dance Keys
enum td_keycodes {
	// simple
  _IH, // 1t = i, 2t = h --VT2 inventory/hero menus
	_OM, // 1t = o, 2t = m --VT2 challenge/map menus
	_ZX, // 1h = z, 2h = x --VT2 inspect character/weapon
	_CW, // 1h = ctrl, 2h = alt --VT2 crouch/walk
	// advanced
	_E0 // 1t = esc, 3h = return to base layer
};

// define a type containing as many tapdance states as you need
typedef enum {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_TAP,
  DOUBLE_HOLD,
  DOUBLE_SINGLE_TAP,
  TRIPLE_TAP,
  TRIPLE_HOLD
} td_state_t;

// 1u 0, 2u + and enter
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_BASE] = LAYOUT_ortho_5x6(
		KC_ESC,  KC_BSPC, TG(_NUM), KC_BTN4, KC_BTN5, KC_BTN3,
		KC_LOCK, ALT_TAB, KC_WH_L,  KC_MS_U, KC_WH_U, KC_BTN1,
		COPY,    PASTE,   KC_MS_L,  KC_MS_D, KC_MS_R, XXXXXXX,
		KC_F5,   QM_NT,   KC_WH_R,  KC_UP,   KC_WH_D, KC_BTN2,
		QM_DN,   KC_ACL0, KC_LEFT,  KC_DOWN, KC_RGHT, XXXXXXX
    ),

  [_NUM] = LAYOUT_ortho_5x6(
  	KC_ESC,  KC_BSPC, _______, KC_PSLS, KC_PAST, KC_PMNS,
  	MO(_EX), _______, KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
  	_______, _______, KC_P4,   KC_P5,   KC_P6,   XXXXXXX,
  	KC_CALC, _______, KC_P1,   KC_P2,   KC_P3,   KC_PENT,
  	_______, _______, KC_P0,   QM_00,   KC_PDOT, XXXXXXX
    ),

  [_EX] = LAYOUT_ortho_5x6(
    BL_TOGG, BL_STEP, _______,  _______,  _______, _______,
    BL_BRTG, _______, _______,  _______,  _______, _______,
    _______, _______, _______,  _______,  _______, XXXXXXX,
    _______, _______, TG(_GP1), _______,  _______, KC_VOLU,
    _______, _______, TG(_GP0), _______,  KC_MUTE, XXXXXXX
    ),
	[_GP0] = LAYOUT_ortho_5x6(
	 	TD(_E0),   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
	 	KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_ENT,
	 	KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    XXXXXXX,
	 	KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_SPC,
	 	KC_LCTL,   KC_LALT, TO(0),   KC_P,    KC_H,    XXXXXXX
	   ),

	[_GP1] = LAYOUT_ortho_5x6(
		TD(_E0),   QM_STAB,   KC_1,      KC_2,      KC_3,    KC_4,
		QM_ATCL,   KC_TAB,    KC_Q,      KC_W,      KC_E,    KC_T,
		KC_F5,     KC_C,      KC_A,      KC_S,      KC_D,    XXXXXXX,
		KC_F6,     TD(_CW),   KC_R,      KC_5,      KC_F,    KC_SPC,
		KC_ENT,    TD(_IH),   TD(_OM),   TD(_ZX),   KC_LSFT, XXXXXXX
    ) // Vermintide 2

};

// Timers
bool alt_tab_enabled;
uint32_t alt_tab_timer;

bool auto_click;
uint16_t ac_timer = false;
uint16_t ac_interval = 100; // (1000ms == 1s)

// tapdance functions:
// create a global instance of the tapdance state type
static td_state_t td_state;
// function to determine the current tapdance state
int cur_dance (qk_tap_dance_state_t *state);
// determine the tapdance state to return
int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; // any number higher than the maximum state value you return above
}

void tde0_each (qk_tap_dance_state_t *state, void *user_data) {
  tap_code16(KC_ESC);
}

void tde0_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_state = cur_dance(state);
  if ((td_state = TRIPLE_HOLD)) {
		layer_move(_BASE);
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [_IH] = ACTION_TAP_DANCE_DOUBLE(KC_I, KC_H),
	[_OM] = ACTION_TAP_DANCE_DOUBLE(KC_O, KC_M),
	[_ZX] = ACTION_TAP_DANCE_DOUBLE(KC_Z, KC_X),
	[_CW] = ACTION_TAP_DANCE_DOUBLE(KC_LCTL, KC_LALT),
	[_E0] = ACTION_TAP_DANCE_FN_ADVANCED(tde0_each, tde0_finished, NULL)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QMKBEST:
      if (record->event.pressed) {
        // when keycode QMKBEST is pressed
        SEND_STRING("QMK is the best thing ever!");
      } else {
        // when keycode QMKBEST is released
      }
      break;
    case QMKURL:
      if (record->event.pressed) {
        // when keycode QMKURL is pressed
        SEND_STRING("https://qmk.fm/" SS_TAP(X_ENTER));
      } else {
        // when keycode QMKURL is released
      }
      break;
    case QM_00:
      if (record->event.pressed) {
        SEND_STRING("00");
      } else {
        // when released
      }
      break;
    case ALT_TAB:
    alt_tab_enabled = record->event.pressed;
      if (alt_tab_enabled) {
        alt_tab_timer = timer_read();
        register_code(KC_LALT);
        tap_code(KC_TAB);
      } else {
        unregister_code(KC_LALT);
      }
      break;
    case QM_DN:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("a"));
        _delay_ms(100);
        SEND_STRING(SS_LGUI("a"));
      } else {
        // when released
      }
      break;
		case QM_ATCL:
			if (record->event.pressed) {
				auto_click ^= 1;
				ac_timer = timer_read();
			}
			return false;
  }
  return true;
}

void matrix_scan_user(void) {
  if ( alt_tab_enabled && (timer_elapsed(alt_tab_timer) > 250) ) {
    tap_code(KC_TAB);
    alt_tab_timer = timer_read();
	};
	if (auto_click && timer_elapsed(ac_timer) >= ac_interval) {
		ac_timer = timer_read();
		tap_code16(KC_BTN1);
	}
}

void led_set_user(uint8_t usb_led) {

}
