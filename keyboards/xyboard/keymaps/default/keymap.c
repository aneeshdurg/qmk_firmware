/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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
#include "quantum.h"
#include "analog.h"
#include "transactions.h"



enum layers {
    _QWERTY = 0,
    _DVORAK,
    _COLEMAK_DH,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
};

enum custom_keys {
    DEBUG_JS = SAFE_RANGE,
};

// Aliases for readability
#define QWERTY DF(_QWERTY)
#define COLEMAK DF(_COLEMAK_DH)
#define DVORAK DF(_DVORAK)

#define SYM MO(_SYM)
#define NAV MO(_NAV)
#define FKEYS MO(_FUNCTION)
#define ADJUST MO(_ADJUST)

#define CTL_ESC MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT MT(MOD_LALT, KC_ENT)

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | [ {  |CapsLk|  |F-keys|  ] } |   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
     KC_TAB  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,                                        KC_Y,   KC_U ,  KC_I ,   KC_O ,  KC_P , KC_BSPC,
     CTL_ESC , KC_A ,  KC_S   ,  KC_D  ,   KC_F ,   KC_G ,                                        KC_H,   KC_J ,  KC_K ,   KC_L ,KC_SCLN,CTL_QUOT,
     KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , KC_LBRC, DEBUG_JS ,     FKEYS  , KC_RBRC, KC_N,   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                ADJUST , KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
    ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

typedef struct _rpc_req { } rpc_req;

typedef struct _rpc_resp {
    int slave_x;
    int slave_y;
    bool slave_sw;
} rpc_resp;


void keyboard_pre_init_user(void) {
    // Select button on joystick
    setPinInputHigh(C7);
}

void get_joystick_state_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    // const rpc_req *req = (const rpc_req*)in_data;
    rpc_resp *resp = (rpc_resp*)out_data;

    int x = analogReadPin(F0);
    int y = analogReadPin(F1);
    // int new_sw_state = readPin(C7);

    resp->slave_x = x;
    resp->slave_y = y;
    resp->slave_sw = readPin(C7);
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(RPC_GET_JOYSTICK_STATE, get_joystick_state_handler);
}


typedef struct {
    int x;
    int y;
    bool sw;
} joystick_state;

joystick_state state_left;
joystick_state state_right;
keyrecord_t record;
uint16_t js_keycode;
bool had_js_event = false;
bool enable_mouse = true;

const int left_center_x = 514;
const int left_center_y = 508;
const int right_center_x = 511;
const int right_center_y = 504;

void matrix_scan_user(void) {
    if (!is_keyboard_master()) {
        return;
    }

    joystick_state new_left = {analogReadPin(F0), analogReadPin(F1), !!readPin(C7)};
    if (state_left.x != new_left.x || state_left.y != new_left.y) {
        state_left.x = new_left.x;
        state_left.y = new_left.y;
        state_left.sw = new_left.sw;
        if (state_left.x == 0 && state_left.y == 0) {
            record.event = MAKE_EVENT(0, 0, true, KEY_EVENT);
            js_keycode = DEBUG_JS;
            process_record_user(js_keycode, &record);
            had_js_event = true;
        } else if (state_left.x == 507 && state_left.y == 514) {
            if (had_js_event) {
                record.event.pressed = false;
                process_record_user(js_keycode, &record);
            }
            had_js_event = false;
        }
    }


    joystick_state new_right = {0, 0, false};
    if (transaction_rpc_exec(RPC_GET_JOYSTICK_STATE, sizeof(joystick_state), &state_left, sizeof(joystick_state), &new_right)) {
        if (!enable_mouse) {
        }
        if (state_right.sw != new_right.sw) {
            if (!new_right.sw) {
                enable_mouse = !enable_mouse;
            }
        }

        state_right.x = new_right.x;
        state_right.y = new_right.y;
        state_right.sw = new_right.sw;
    }
}


int8_t last_report_x = 0;
int8_t last_report_y = 0;
void pointing_device_driver_init(void) {}

int8_t clamp(int v) {
    if (v > 127) {
        return 127;
    }

    if (v < -127) {
        return -127;
    }

    return (int8_t)v;
}

#define RESOLUTION 50
uint8_t tick;
report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    if (enable_mouse) {
        int xval = state_right.x - 507;
        xval *= -1;
        xval /= 32;

        int yval = (state_right.y - 514) / 32;

        int8_t new_xreport = clamp(xval);
        int8_t new_yreport = clamp(yval);

        mouse_report.h = 0;
        mouse_report.v = 0;
        mouse_report.x = new_xreport;
        mouse_report.y = new_yreport;

    }

    // TODO use timers/timer_read instead
    tick += 1;
    tick %= RESOLUTION;
    if (tick != 0) {
        mouse_report.x = 0;
        mouse_report.y = 0;
    } else {
        if (mouse_report.x != 0) {
            if (mouse_report.x > last_report_x) {
                mouse_report.x = last_report_x + 1;
            } else if (mouse_report.x < last_report_x) {
                mouse_report.x = last_report_x - 1;
            }
        }

        if (mouse_report.y != 0) {
            if (mouse_report.y > last_report_y) {
                mouse_report.y = last_report_y + 1;
            } else if (mouse_report.y < last_report_y) {
                mouse_report.y = last_report_y - 1;
            }
        }

        last_report_x = mouse_report.x;
        last_report_y = mouse_report.y;
    }

    return mouse_report;
}
uint16_t       pointing_device_driver_get_cpi(void) { return 100; }
void           pointing_device_driver_set_cpi(uint16_t cpi) {}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    char string[100];

    switch (keycode) {
    case DEBUG_JS:
        if (record->event.pressed) {
            if (sprintf(
                string,
                "x: %d y: %d s: %d r_x: %d r_y: %d r_s: %d\n",
                state_left.x,
                state_left.y,
                state_left.sw,
                state_right.x,
                state_right.y,
                state_right.sw
            ) > 0)
                send_string(string);
        } else {
            // when keycode DEBUG_JS is released
        }
        break;
    }
    return true;
};
