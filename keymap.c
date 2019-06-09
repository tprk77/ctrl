/* Copyright (c) 2019 Tim Perkins */

#include QMK_KEYBOARD_H

#include "ctrl_transpose.h"
#include "rgb_modes.h"
#include "messages.h"

#define KEYMAP LAYOUT_transpose

#define XXXXXXX KC_NO
#define _______ KC_TRNS

enum tprk77_layers {
  LR_QWTY = 0,  /* QWERTY (BASE) */
  LR_NAV  = 1,  /* NAV */
};

enum tprk77_fns {
  MO_NAV  = KC_FN0,
};

enum tprk77_mod_keys {
  MD_HYPR = ALL_T(KC_NO),
  MD_MEH  = MEH_T(KC_NO),
  MD_CADL = LCTL(LALT(KC_DEL)),
};

#define SR SAFE_RANGE

enum tprk77_keys {
  MC_PI = SR,   /* 3.14159265358979323846 (As defined in "math.h") */
  MC_MESG,      /* Print a message */
  MC_MODI,      /* Next RGB mode */
  MC_MODD,      /* Previous RGB mode */
  MC_MREC,      /* Macro Record */
  MC_MPLY,      /* Macro Stop and Play */
  DYNAMIC_MACRO_RANGE,  /* Dynamic Macros (Must be at the end) */
};

#undef SR

/*
 * NOTE This must be included AFTER defining DYNAMIC_MACRO_RANGE!
 */
#include "dynamic_macro.h"

/* State for messages, etc */
bool is_randomized = false;
int current_message = 0;

/* State for the current RGB mode */
int current_mode = 0;

/* State for dynamic macro stop/play toggle */
int dyn_macro_stopped = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /*
   * QWERTY (BASE) LAYER:
   *
   * ,----------------------------------.   ,------.
   * | Ctrl |      | Nav  | Tab  |  `   |   | Esc  |
   * |------| Shft |------+------+------|   |------|
   * | GUI  |      |  A   |  Q   |  1   |   | F1   |
   * |------+------+------+------+------|   |------|
   * | Alt  |  Z   |  S   |  W   |  2   |   | F2   |
   * |------+------+------+------+------|   |------|
   * |      |  X   |  D   |  E   |  3   |   | F3   |
   * |      |------+------+------+------|   |------|
   * |      |  C   |  F   |  R   |  4   |   | F4   |
   * |      |------+------+------+------|   |------|
   * |      |  V   |  G   |  T   |  5   |   | F5   |
   * |      |------+------+------+------|   |------|
   * | Spc  |  B   |  H   |  Y   |  6   |   | F6   |
   * |      |------+------+------+------|   |------|
   * |      |  N   |  J   |  U   |  7   |   | F7   |
   * |      |------+------+------+------|   |------|
   * |      |  M   |  K   |  I   |  8   |   | F8   |
   * |      |------+------+------+------|   |------|
   * |      |  ,   |  L   |  O   |  9   |   | F9   |
   * |------+------+------+------+------|   |------|
   * | Alt  |  .   |  ;   |  P   |  0   |   | F10  |
   * |------+------+------+------+------|   |------|
   * | Pls  |  /   |  '   |  [   |  _   |   | F11  |
   * |------+------+------+------+------|   |------|
   * | App  |      |      |  ]   |  =   |   | F12  |
   * |------| Shft | Entr |------+------|   `------'
   * | Ctrl |      |      |  \   | Bksp |
   * `----------------------------------'
   *
   * ,------.             ,-------------.   ,------.
   * | Left |             | Del  | Ins  |   | PSrc |
   * |------+------.      |------+------|   |------|
   * | Down |  Up  |      | End  | Home |   | XXX  |
   * |------+------'      |------+------|   |------|
   * | Rght |             | PgDn | PgUp |   | XXX  |
   * `------'             `-------------'   `------'
   */
  [LR_QWTY] = LAYOUT_transpose(
      KC_LCTL, KC_LSFT, MO_NAV,  KC_TAB,  KC_GRV,  KC_ESC,
      KC_LGUI,          KC_A,    KC_Q,    KC_1,    KC_F1,
      KC_LALT, KC_Z,    KC_S,    KC_W,    KC_2,    KC_F2,
               KC_X,    KC_D,    KC_E,    KC_3,    KC_F3,
               KC_C,    KC_F,    KC_R,    KC_4,    KC_F4,
               KC_V,    KC_G,    KC_T,    KC_5,    KC_F5,
      KC_SPC,  KC_B,    KC_H,    KC_Y,    KC_6,    KC_F6,
               KC_N,    KC_J,    KC_U,    KC_7,    KC_F7,
               KC_M,    KC_K,    KC_I,    KC_8,    KC_F8,
               KC_COMM, KC_L,    KC_O,    KC_9,    KC_F9,
      KC_RALT, KC_DOT,  KC_SCLN, KC_P,    KC_0,    KC_F10,
      KC_RGUI, KC_SLSH, KC_QUOT, KC_LBRC, KC_MINS, KC_F11,
      KC_APP,  KC_RSFT, KC_ENT,  KC_RBRC, KC_EQL,  KC_F12,
      KC_RCTL,                   KC_BSLS, KC_BSPC,

      KC_LEFT,                   KC_DEL,  KC_INS,  KC_PSCR,
      KC_DOWN, KC_UP,            KC_END,  KC_HOME, MC_MREC,
      KC_RGHT,                   KC_PGDN, KC_PGUP, MC_MPLY
  ),

  /*
   * NAV LAYER:
   *
   * ,----------------------------------.   ,------.
   * | Ctrl |      | Nav  | XXXX | XXXX |   | XXXX |
   * |------| Shft |------+------+------|   |------|
   * | GUI  |      | XXXX | XXXX | XXXX |   | XXXX |
   * |------+------+------+------+------|   |------|
   * | Alt  | XXXX | XXXX | XXXX | XXXX |   | XXXX |
   * |------+------+------+------+------|   |------|
   * |      | XXXX | XXXX | XXXX |  Pi  |   | XXXX |
   * |      |------+------+------+------|   |------|
   * |      | XXXX | XXXX | XXXX | XXXX |   | XXXX |
   * |      |------+------+------+------|   |------|
   * |      | XXXX | XXXX | XXXX | XXXX |   | XXXX |
   * |      |------+------+------+------|   |------|
   * | Ctrl | XXXX | PgDn | PgUp | XXXX |   | XXXX |
   * |      |------+------+------+------|   |------|
   * |      | XXXX | Left | Home | XXXX |   | XXXX |
   * |      |------+------+------+------|   |------|
   * |      | XXXX | Down | Up   | XXXX |   | XXXX |
   * |      |------+------+------+------|   |------|
   * |      | XXXX | Rght | End  | XXXX |   | XXXX |
   * |------+------+------+------+------|   |------|
   * | Alt  | XXXX | XXXX | XXXX | XXXX |   | XXXX |
   * |------+------+------+------+------|   |------|
   * | GUI  | XXXX | XXXX | XXXX | XXXX |   | XXXX |
   * |------+------+------+------+------|   |------|
   * | XXXX |      |      | XXXX | XXXX |   | XXXX |
   * |------| Shft | Mesg |------+------|   `------'
   * | Ctrl |      |      | XXXX |C-A-Dl|
   * `----------------------------------'
   *
   * ,------.             ,-------------.   ,------.
   * | LED- |             | Mute | Play |   | XXXX |
   * |------+------.      |------+------|   |------|
   * | Hue- | Hue+ |      | Next | Prev |   | XXXX |
   * |------+------'      |------+------|   |------|
   * | LED+ |             | Vol- | Vol+ |   | XXXX |
   * `------'             `-------------'   `------'
   */
  [LR_NAV] = LAYOUT_transpose(
      KC_LCTL, KC_LSFT, MO_NAV,  XXXXXXX, XXXXXXX, XXXXXXX,
      KC_LGUI,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      KC_LALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
               XXXXXXX, XXXXXXX, XXXXXXX, MC_PI,   XXXXXXX,
               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      KC_LCTL, XXXXXXX, KC_PGDN, KC_PGUP, XXXXXXX, XXXXXXX,
               XXXXXXX, KC_LEFT, KC_HOME, XXXXXXX, XXXXXXX,
               XXXXXXX, KC_DOWN, KC_UP,   XXXXXXX, XXXXXXX,
               XXXXXXX, KC_RGHT, KC_END,  XXXXXXX, XXXXXXX,
      KC_RALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      KC_RGUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, KC_RSFT, MC_MESG, XXXXXXX, XXXXXXX, XXXXXXX,
      KC_RCTL,                   XXXXXXX, MD_CADL,

      MC_MODD,                   KC_MUTE, KC_MPLY, XXXXXXX,
      RGB_HUD, RGB_HUI,          KC_MNXT, KC_MPRV, XXXXXXX,
      MC_MODI,                   KC_VOLD, KC_VOLU, XXXXXXX
  ),
};

const uint16_t PROGMEM fn_actions[] = {
  /* MO_NAV  */ [0] = ACTION_LAYER_MOMENTARY(LR_NAV),
};

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
  /* Process dynamic macro keys */
  if (!process_record_dynamic_macro(keycode, record)) {
    return false;
  }
  /* Process macro keys */
  if (record->event.pressed) {
    switch (keycode) {
      case MC_PI:
        SEND_STRING("3.14159265358979323846");
        return false;
      case MC_MESG:
        send_string(&tprk77_message_data[tprk77_message_offsets[current_message]]);
        send_string(" ");  /* Trailing space as a separator */
        if (!is_randomized) {
          /* Seed with the time of the first message */
          srand(timer_read32());
          is_randomized = true;
        }
        current_message = rand() % tprk77_num_messages;
        return false;
      case MC_MODI:
        if (++current_mode >= tprk77_num_rgb_modes) {
          current_mode = 0;
        }
        rgblight_mode(tprk77_rgb_modes[current_mode]);
        return false;
      case MC_MODD:
        if (--current_mode < 0) {
          current_mode = tprk77_num_rgb_modes - 1;
        }
        rgblight_mode(tprk77_rgb_modes[current_mode]);
        return false;
      case MC_MREC:
        {
          keyrecord_t fake_record = {0};
          fake_record.event.pressed = 0;  /* Record needs to be released */
          process_record_dynamic_macro(DYN_REC_START1, &fake_record);
          dyn_macro_stopped = 0;
        }
        return false;
      case MC_MPLY:
        {
          keyrecord_t fake_record = {0};
          if (!dyn_macro_stopped) {
            dyn_macro_stopped = 1;
            fake_record.event.pressed = 1;  /* Stop needs to be pressed */
            process_record_dynamic_macro(DYN_REC_STOP, &fake_record);
          } else {
            fake_record.event.pressed = 0;  /* Play needs to be released */
            process_record_dynamic_macro(DYN_MACRO_PLAY1, &fake_record);
          }
        }
        return false;
    }
  }
  return true;
}

void matrix_init_user(void)
{
  /*
   * NOTE The Ctrl doesn't currently have EEPROM functionality.
   * This means that the RGB setting will reset every time.
   *
   * See also: https://github.com/qmk/qmk_firmware/issues/4625
   */
  rgblight_mode(tprk77_rgb_modes[current_mode]);
  /* Decypher the scrambled messages */
  tprk77_decypher_messages();
}

  /*
   * LAYER TEMPLATE:
   *
   * ,----------------------------------.   ,------.
   * |      |      |      |      |      |   |      |
   * |------|      |------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |------+------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |------+------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |      |------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |      |------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |      |------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |      |------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |      |------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |      |------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |------+------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |------+------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |------+------+------+------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |------|      |      |------+------|   `------'
   * |      |      |      |      |      |
   * `----------------------------------'
   *
   * ,------.             ,-------------.   ,------.
   * |      |             |      |      |   |      |
   * |------+------.      |------+------|   |------|
   * |      |      |      |      |      |   |      |
   * |------+------'      |------+------|   |------|
   * |      |             |      |      |   |      |
   * `------'             `-------------'   `------'
   */

/* (setq tab-stop-list '(6 15 24 33 42 51 60 69)) */
