#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;
uint32_t check;
uint32_t desired = 13;

void matrix_init_user() {
    rgblight_mode(desired);
}

#define _BASE 0		// Base layer
#define _NUM 1		// Num Pad
#define _FUN 2		// Game directory and general functions
#define _GP0 3		// Default Game Pad
#define _GP1 4		// Vermintide 2
#define _GP2 5		// Unused
#define _ADJUST 16

// Keys
#define QM_NT LCTL(KC_T)  // new tab
#define QM_PT LCTL(LSFT(KC_T))  // restore deleted tab
#define COPY LCTL(KC_C)
#define PASTE LCTL(KC_V)
#define QM_NEW LCTL(KC_N)
#define QM_STAB LSFT(KC_TAB)  // shift+tab for Steam overlay

// Macros
enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  QM_00,
  ALT_TAB,
  QM_DN,  // dismiss notifications
  QM_ATCL  // autoclick
};

// Tap Dance
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[_BASE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TG(_NUM),          RAISE,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_LGUI, KC_ENT,  KC_HOME,                   KC_END,  KC_SPC,   KC_LALT
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_NUM] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                            KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
    TG(_GP1), _______, KC_UP,   _______, _______, KC_LCBR,                            KC_RBRC, KC_P7,   KC_P8,   KC_P9,   KC_PLUS, KC_BSLS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_DEL,  KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,                            _______, KC_P4,   KC_P5,   KC_P6,   KC_MINS, KC_RSFT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     BL_STEP, _______, _______, _______, _______, _______, _______,          _______, _______, KC_P1,   KC_P2,   KC_P3,   _______, KC_ENT,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, KC_ENT,  KC_PGUP,                   KC_SPC,  KC_P0,   KC_PDOT
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_FUN] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, KC_VOLU,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, KC_VOLD,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, KC_MSTP, _______, _______, KC_MUTE,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_GP0] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    TO(0),
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    QM_ATCL,          RAISE,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_SPC,  KC_SPC,  KC_HOME,                   KC_END,  KC_ENT,   KC_LALT
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_GP1] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_ESC,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    TO(0),
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    QM_ATCL,          QM_ATCL,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_T,    KC_SPC,  KC_HOME,                   KC_END,  KC_ENT,   KC_LALT
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_ADJUST] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     RGB_M_P, RGB_M_R, RGB_M_SW, RGB_M_SN, RGB_M_K, RGB_M_X,                            _______, _______, _______, _______, _______, RESET,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RGB_TOG, RGB_MOD, RGB_M_B, RGB_M_G, RGB_M_T, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RGB_VAD, RGB_VAI, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     BL_TOGG, BL_STEP, _______, _______, _______, RGB_SAI, RGB_HUI,          _______, _______, _______, _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, RGB_SAD, RGB_HUD,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  )
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
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_BASE);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_NUM);
        update_tri_layer(_NUM, _FUN, _ADJUST);
      } else {
        layer_off(_NUM);
        update_tri_layer(_NUM, _FUN, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_FUN);
        update_tri_layer(_NUM, _FUN, _ADJUST);
      } else {
        layer_off(_FUN);
        update_tri_layer(_NUM, _FUN, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
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

uint8_t prev = _BASE; // the previous layer to base at the start

uint32_t layer_state_set_user(uint32_t state) {
  uint8_t layer = biton32(state);
  if (prev!=_ADJUST) {
      switch (layer) {
        case _BASE:
          rgblight_mode(desired);
          break;
        case _NUM:
          rgblight_mode_noeeprom(RGBLIGHT_MODE_SNAKE + 2);
          rgblight_sethsv_noeeprom(HSV_ORANGE);
          break;
        case _FUN:
          rgblight_mode_noeeprom(RGBLIGHT_MODE_CHRISTMAS);
          break;
        case _ADJUST:
          break;
        case _GP1:
          rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING + 3);
          rgblight_sethsv_noeeprom(HSV_RED);
          rgblight_set();
      }
  } else {
      desired = rgblight_get_mode();
  }
  prev = layer;
  return state;
}

void led_set_user(uint8_t usb_led) {

}
