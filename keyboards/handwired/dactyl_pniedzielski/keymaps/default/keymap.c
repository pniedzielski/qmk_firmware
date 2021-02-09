#include QMK_KEYBOARD_H


/*****************************************************************************/
/*                                                                    LAYERS */
/*****************************************************************************/


enum {
  LYR_BASE,
  LYR_FN,
  LYR_MOUSE_NUM,
  LYR_COUNT,
};


/*****************************************************************************/
/*                                                                 TAP DANCE */
/*****************************************************************************/


// Tap dance enum
enum {
    TD_LYR,  // tap dance for layer switch
};

typedef struct {
    uint8_t is_hold;
    uint8_t count;
} tap;

// Initialize tap structure associated with example tap dance key
static tap ql_tap_state = {
    .is_hold = 1,
    .count = 0
};

static uint8_t tap_layer = LYR_BASE;

// Functions that control what our tap dance key does
void td_lyr_finished(qk_tap_dance_state_t *state, void *user_data) {
    ql_tap_state.is_hold = state->pressed;
    ql_tap_state.count   = state->count;

    if (ql_tap_state.count == 0)  return;

    uint8_t lyr = (ql_tap_state.count - 1) % LYR_COUNT;
    layer_clear();

    if (lyr != LYR_BASE)  layer_move(lyr);

    if (!ql_tap_state.is_hold)  tap_layer = lyr;
}

void td_lyr_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off
    // the layer
    if (ql_tap_state.is_hold) {
      layer_clear();
      if (tap_layer != LYR_BASE)  layer_move(tap_layer);
    }
    ql_tap_state.count = 0;
}

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_LYR] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, td_lyr_finished, td_lyr_reset, 275)
};


/*****************************************************************************/
/*                                                                    LEADER */
/*****************************************************************************/


LEADER_EXTERNS();

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_ONE_KEY(KC_GESC) {
      reset_keyboard();
    }
    SEQ_ONE_KEY(KC_C) {
      // Anything you can do in a macro.
      SEND_STRING("QMK is awesome.");
    }
    SEQ_TWO_KEYS(KC_D, KC_D) {
      SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
    }
    SEQ_THREE_KEYS(KC_D, KC_D, KC_S) {
      SEND_STRING("https://start.duckduckgo.com\n");
    }
    SEQ_TWO_KEYS(KC_A, KC_S) {
      register_code(KC_LGUI);
      register_code(KC_S);
      unregister_code(KC_S);
      unregister_code(KC_LGUI);
    }
  }
}


/*****************************************************************************/
/*                                                                   KEYMAPS */
/*****************************************************************************/


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LYR_BASE] = LAYOUT_7x5(
        KC_1,      KC_2,    KC_3,    KC_4,    KC_5,    KC_6,      KC_LEAD,         TD(TD_LYR),  KC_7,     KC_8,    KC_9,     KC_0,     KC_MINUS,  KC_BSLASH,
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      DYN_MACRO_PLAY1, KC_GRAVE,    KC_Y,     KC_U,    KC_I,     KC_O,     KC_P,      KC_LBRACKET,
        KC_DELETE, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      DYN_MACRO_PLAY2, KC_RBRACKET, KC_H,     KC_J,    KC_K,     KC_L,     KC_SCOLON, KC_QUOTE,
        KC_CAPS,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                    KC_N,     KC_M,    KC_COMMA, KC_DOT,   KC_SLASH,  KC_EQUAL,
                            KC_LEFT, KC_UP,   KC_HOME, KC_LSHIFT, KC_BSPACE,       KC_ENTER,    KC_SPACE, KC_PGUP, KC_DOWN,  KC_RIGHT,
                                              KC_END,  KC_LCTRL,  KC_LGUI,         KC_APP,      KC_LALT,  KC_PGDN
    ),
    [LYR_FN] = LAYOUT_7x5(
        KC_F1,         KC_F2,   KC_F3,   KC_F4,           KC_F5,          KC_F6,   KC_ESC,         _______,         KC_F7,   KC_F8,               KC_F9,               KC_F10,              KC_F11,  KC_F12,
        KC_MAIL,       _______, _______, KC_UP,           _______,        _______, DYN_REC_START1, KC_SYSTEM_SLEEP, _______, KC_MEDIA_EJECT,      KC_MEDIA_PLAY_PAUSE, KC_AUDIO_MUTE,       _______, KC_MEDIA_SELECT,
        KC_WWW_SEARCH, _______, KC_LEFT, KC_DOWN,         KC_RIGHT,       _______, DYN_REC_START2, RESET,           _______, KC_MEDIA_PREV_TRACK, KC_MEDIA_STOP,       KC_MEDIA_NEXT_TRACK, _______, KC_MY_COMPUTER,
        KC_F13,        KC_F14,  KC_F15,  KC_F16,          KC_F17,         KC_F18,                                   KC_F19,  KC_F20,              KC_F21,              KC_F22,              KC_F23,  KC_F24,
                                KC_PSCR, KC_AUDIO_VOL_UP, KC_WWW_HOME,    _______, _______,        _______,         _______, KC_WWW_BACK,         KC_AUDIO_VOL_DOWN,   KC_BRK,
                                                          KC_WWW_REFRESH, _______, _______,        _______,         _______, KC_WWW_FORWARD
    ),
    [LYR_MOUSE_NUM] = LAYOUT_7x5(
        _______, _______, KC_MS_ACCEL0,  KC_MS_ACCEL1,   KC_MS_ACCEL2,  _______, _______,   _______,  _______, KC_NUMLOCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS, _______,
        _______, _______, KC_MS_BTN1,    KC_MS_UP,       KC_MS_BTN2,    _______, _______,   _______,  _______, KC_KP_7,    KC_KP_8,     KC_KP_9,        KC_KP_PLUS,  _______,
        _______, _______, KC_MS_LEFT,    KC_MS_DOWN,     KC_MS_RIGHT,   _______, _______,   _______,  _______, KC_KP_4,    KC_KP_5,     KC_KP_6,        KC_KP_PLUS,  _______,
        _______, _______, KC_MS_BTN3,    KC_MS_BTN4,     KC_MS_BTN5,    _______,                      _______, KC_KP_1,    KC_KP_2,     KC_KP_3,        KC_KP_ENTER, _______,
                          KC_MS_WH_LEFT, KC_MS_WH_RIGHT, KC_MS_WH_UP,   _______, _______,   _______,  _______, _______,    KC_KP_0,     KC_KP_DOT,
                                                         KC_MS_WH_DOWN, _______, _______,   _______,  _______, _______
    ),
};
