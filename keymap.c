/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "frames.h"

static int num_keypresses = 0;
static int current_frame = 0;

// Layers
enum {
	_BASE,
	_SYM,
	_NUM,
	_GREEK,
	_MATH,
	_GAME,
	_WPN
};

enum combos {
	FLWR_RESET,  // KC_SPC + KC_ENT
    FLWR_GROW,   // KC_SPC + KC_K
};

enum unicode_names {
	// GREEK
	ALPHA_LO,
	BETA_LO,
	GAMMA_UP, GAMMA_LO,
	DELTA_UP, DELTA_LO,
	EPS_LO,
	ZETA_LO,
	ETA_LO,
	THETA_UP, THETA_LO,
	IOTA_LO,
	KAPPA_LO,
	LAMBDA_UP, LAMBDA_LO,
	MU_LO,
	NU_LO,
	XI_UP, XI_LO,
	PI_UP, PI_LO,
	RHO_LO,
	SIG_UP, SIG_LO,
	TAU_LO,
	UPS_LO,
	PHI_UP, PHI_LO,
	CHI_LO,
	PSI_UP, PSI_LO,
	OMEG_UP, OMEG_LO,
	// MATH
	PLSMIN,
	GE,
	LE,
	COMPLEX,
	INTS,
	NATURAL,
	NUL,
	RAT,
	REAL,
	IN,
	NIN,
	DEL,
	NABLA,
	INTEG,
};

const uint32_t PROGMEM unicode_map[] = {
	// GREEK
	[ALPHA_LO] = 0x03B1,
	[BETA_LO] = 0x03B2,
	[GAMMA_UP] = 0x0393, [GAMMA_LO] = 0x03B3,
	[DELTA_UP] = 0x0394, [DELTA_LO] = 0x03B4,
	[EPS_LO] = 0x03B5,
	[ZETA_LO] = 0x03B6,
	[ETA_LO] = 0x03B7,
	[THETA_UP] = 0x0398, [THETA_LO] = 0x03B8,
	[IOTA_LO] = 0x03B9,
	[KAPPA_LO] = 0x03BA,
	[LAMBDA_UP] = 0x039B, [LAMBDA_LO] = 0x03BB,
	[MU_LO] = 0x03BC,
	[NU_LO] = 0x03BD,
	[XI_UP] = 0x039E, [XI_LO] = 0x03BE,
	[PI_UP] = 0x03A0, [PI_LO] = 0x03C0,
	[RHO_LO] = 0x03C1,
	[SIG_UP] = 0x03A3, [SIG_LO] = 0x03C3,
	[TAU_LO] = 0x03C4,
	[UPS_LO] = 0x03C5,
	[PHI_UP] = 0x03A6, [PHI_LO] = 0x03C6,
	[CHI_LO] = 0x03C7,
	[PSI_UP] = 0x03A8, [PSI_LO] = 0x03C8,
	[OMEG_UP] = 0x03A9, [OMEG_LO] = 0x03A9,
	// MATH
	[PLSMIN] = 0x00B1,
	[GE] = 0x2265,
	[LE] = 0x2264,
	[COMPLEX] = 0x2102,
	[INTS] = 0x2124,
	[NATURAL] = 0x2115,
	[NUL] = 0x2205,
	[RAT] = 0x211A,
	[REAL] = 0x211D,
	[IN] = 0x2208,
	[NIN] = 0x2209,
	[DEL] = 0x2202,
	[NABLA] = 0x2207,
	[INTEG] = 0x222B,
};

const uint16_t PROGMEM flwr_combo[] = {KC_SPC, KC_ENT, COMBO_END};
const uint16_t PROGMEM flwr_grow[] = {KC_SPC, KC_K, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
	[FLWR_RESET] = COMBO_ACTION(flwr_combo),
	[FLWR_GROW] = COMBO_ACTION(flwr_grow),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /*
  // BASE, workman layout
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC,   KC_Q,    KC_D,   KC_R,    KC_W,     KC_B,                         KC_J,    KC_F,   KC_U,    KC_P,   KC_QUOT, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,   KC_A,    KC_S,   KC_H,    KC_T,     KC_G,                         KC_Y,    KC_N,   KC_E,    KC_O,   KC_I,    KC_SCLN,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSPO,  KC_Z,    KC_X,   KC_M,    KC_C,     KC_V,                         KC_K,    KC_L,   KC_COMM, KC_DOT, KC_SLSH, KC_RSPC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                          	          KC_LCTL, MO(1), KC_SPC,    KC_ENT, MO(2), KC_RALT
                                      //`--------------------------'  `--------------------------'
    ),
  */
  // BASE
  [_BASE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC,   KC_Q,    KC_W,   KC_E,    KC_R,     KC_T,                         KC_Y,    KC_U,   KC_I,    KC_O,   KC_P,    KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,   KC_A,    KC_S,   KC_D,    KC_F,     KC_G,                         KC_H,    KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,  KC_Z,    KC_X,   KC_C,    KC_V,     KC_B,                         KC_N,    KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                          	          KC_LCTL, MO(_SYM), KC_SPC,       KC_ENT, MO(_NUM), KC_LALT
                                      //`--------------------------'  `--------------------------'
    ),

  // SYM
  [_SYM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_GRV, KC_EXLM,  KC_AT,  KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC,  KC_AMPR, KC_ASTR, KC_MINS, KC_EQL, KC_BSLS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,  KC_NO,   KC_NO,  KC_NO,    KC_NO,  KC_NO,                        KC_LEFT,  KC_DOWN, KC_UP,   KC_RGHT, KC_LPRN, KC_RPRN,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_NO,   KC_NO,  KC_NO,    KC_NO,  KC_NO,                        KC_HOME,  KC_PGDN, KC_PGUP, KC_END,  KC_LBRC, KC_RBRC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, KC_TRNS, KC_SPC,     KC_ENT,  UC_MOD,   KC_TRNS
                                      //`--------------------------'  `--------------------------'
  ),

  // NUM
  [_NUM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_PSLS, KC_PAST, KC_PEQL, KC_7,     KC_8,   KC_9,                         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_PMNS, KC_PPLS, KC_PDOT, KC_4,     KC_5,   KC_6,                         KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_CIRC, KC_PERC, KC_0,    KC_1,     KC_2,   KC_3,                         UC_MOD,   KC_NO,   KC_NO,  MO(_GREEK),MO(_MATH),TG(_GAME),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, UC_MOD, KC_SPC,         KC_ENT, KC_TRNS, KC_TRNS
                                      //`--------------------------'  `--------------------------'
  ),

  // GREEK
  [_GREEK] = LAYOUT_split_3x6_3(
  //,----------------------------------------------------------------------------------------------------------.                    ,---------------------------------------------------------------------------------------------------.
     KC_NO,    KC_NO,            KC_NO,            X(EPS_LO),           X(RHO_LO),             X(TAU_LO),                             X(UPS_LO), XP(THETA_LO,THETA_UP),  X(IOTA_LO),          KC_O,             XP(PI_LO,PI_UP), KC_NO,
  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
     KC_NO,   X(ALPHA_LO), XP(SIG_LO,SIG_UP), XP(DELTA_LO,DELTA_UP), XP(PHI_LO,PHI_UP), XP(GAMMA_LO,GAMMA_UP),                       X(ETA_LO),  XP(XI_UP,XI_LO),        X(KAPPA_LO), XP(LAMBDA_LO,LAMBDA_UP),  KC_NO,          KC_RSFT,
  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
     KC_LSFT, X(ZETA_LO),    X(CHI_LO),       XP(PSI_LO,PSI_UP),     XP(OMEG_LO,OMEG_UP),    X(BETA_LO),                               X(NU_LO),   X(MU_LO),               KC_NO,             KC_TRNS,            KC_TRNS,      KC_TRNS,
  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
												 KC_TAB, KC_NO, KC_SPC,      KC_ENT, KC_TRNS,  KC_NO
											      //`--------------------------'  `--------------------------'
  ),

  // MATH
  [4] = LAYOUT_split_3x6_3(
  //,----------------------------------------------------------------------------------------------------------.                    ,---------------------------------------------------------------------------------------------------.
     KC_NO,    X(RAT),            KC_NO,            XP(IN,NIN),           X(REAL),             KC_NO,                                 KC_NO,      KC_NO,                   KC_NO,          X(NUL),                   KC_NO,      KC_NO,
  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
     KC_NO,   KC_NO,        XP(SIG_UP,INTEG),   XP(DEL,NABLA),        XP(PHI_LO,PHI_UP), XP(GAMMA_LO,GAMMA_UP),                      KC_NO,       KC_NO,                 KC_NO,           KC_NO,                KC_NO,          KC_RSFT,
  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
     KC_LSFT, X(INTS),        KC_NO,                  X(COMPLEX),       KC_NO,                 KC_NO,                                   X(NATURAL),   KC_NO,               X(LE),             X(GE),            KC_NO,         KC_NO,
  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
												 KC_TAB, KC_NO, KC_SPC,      KC_ENT, KC_TRNS,  KC_NO
											      //`--------------------------'  `--------------------------'
  ),
  // GAME
  [_GAME] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_NO,  KC_PGDN, KC_PGUP,  KC_NO,   KC_NO,  KC_TRNS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TAB, MO(_WPN), KC_SPC,     KC_ENT, KC_TRNS, KC_NO
                                      //`--------------------------'  `--------------------------'
  ),

  // WPN
  [_WPN] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC,  KC_1,     KC_2,    KC_3,   KC_4,    KC_5,                          KC_6,   KC_7,    KC_8,   KC_9,     KC_0,    KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TRNS, KC_LSFT,  KC_A,    KC_S,   KC_D,    KC_F,                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TRNS, KC_LCTL,  KC_Z,    KC_X,   KC_C,    KC_V,                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_SPC,    KC_ENT, KC_NO, KC_NO
                                      //`--------------------------'  `--------------------------'
  )

};


// Shift + Backspace for Delete
// Initialize variable holding the binary representation of active modifiers.
uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
		num_keypresses = num_keypresses + 1;
		if (current_frame < (ANIM_FRAMES - 1)) {
			if (num_keypresses % GROW_RATE == 0) {
				current_frame = current_frame + 1;
			}
		}
    }
    // Store the current modifier state in the variable for later reference
    mod_state = get_mods();
    switch (keycode) {

    case KC_BSPC:
        {
        // Initialize a boolean variable that keeps track
        // of the delete key status: registered or not?
        static bool delkey_registered;
        if (record->event.pressed) {
            // Detect the activation of either shift keys
            if (mod_state & MOD_MASK_SHIFT) {
                // First temporarily canceling both shifts so that
                // shift isn't applied to the KC_DEL keycode
                del_mods(MOD_MASK_SHIFT);
                register_code(KC_DEL);
                // Update the boolean variable to reflect the status of KC_DEL
                delkey_registered = true;
                // Reapplying modifier state so that the held shift key(s)
                // still work even after having tapped the Backspace/Delete key.
                set_mods(mod_state);
                return false;
            }
        } else { // on release of KC_BSPC
            // In case KC_DEL is still being sent even after the release of KC_BSPC
            if (delkey_registered) {
                unregister_code(KC_DEL);
                delkey_registered = false;
                return false;
            }
        }
        // Let QMK process the KC_BSPC keycode as usual outside of shift
        return true;
	}
    }
    return true;
};


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_270;
}


void process_combo_event(uint16_t combo_index, bool pressed) {
	switch(combo_index) {
		case FLWR_RESET:
			if (pressed) {
				num_keypresses = 0;
				current_frame = 0;
			}
			break;
		case FLWR_GROW:
			if (pressed) {
				if (current_frame < (ANIM_FRAMES - 1)) {
					current_frame = current_frame + 1;
				}
			}
			break;
	}
}


void oled_render_layer_state(void) {
    //  oled_write_P(PSTR("Layer: "), false);
	oled_set_cursor(0, 6);
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_raw_P(base_logo, LOGO_SIZE);
            break;
        case _SYM:
            oled_write_raw_P(sym_logo, LOGO_SIZE);
            break;
        case _NUM:
            oled_write_raw_P(num_logo, LOGO_SIZE);
            break;
        case _GREEK:
            oled_write_raw_P(greek_logo, LOGO_SIZE);
            break;
        case _MATH:
            oled_write_raw_P(math_logo, LOGO_SIZE);
            break;
        case _GAME:
			oled_scroll_off();
            oled_write_raw_P(game_logo, LOGO_SIZE);
            break;
        case _WPN:
            oled_write_raw_P(game_logo, LOGO_SIZE);
			oled_scroll_right();
            break;
    }
}


void oled_render_animation_phase(void) {
	oled_write_raw_P(plant_anim[current_frame], ANIM_SIZE);
}


void oled_task_user(void) {
    if (is_master) {
        oled_render_layer_state();
    } else {
        oled_render_animation_phase();
    }
}

#endif // OLED_DRIVER_ENABLE