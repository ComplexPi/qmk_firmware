// This is the canonical layout file for the Quantum project. If you want to add another keyboard,
// this is the style you want to emulate.

#include "planck.h"
#include "action_layer.h"
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#include "eeconfig.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum planck_layers {
  _QWERTY,
  _LOWER
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  BACKLIT,
  EXT_PLV
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Basic layer
 *
 * ,-----------------------------------------------------------------------,          
 * |  ESC   |   F1 |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |   
 * |--------+------+------+------+------+------+------+------+------+------|  
 * |  ~     |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   
 * |--------+------+------+------+------+------+------+------+------+------| 
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |
 * |--------+------+------+------+------+------+------+------+------+------|      
 * |  Raise |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |
 * |--------+------+------+------+------+------+------+------+------+------'    
 * | LShift |   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |
 * |--------+------+------+------+------+------+------+------+------|        
 * |  Ctrl  |  Win |  Alt |                Spacebar                 |                         
 * `----------------------------------------------------------------'          
 * 
 * ,--------------------------------------------------------------,          
 * |  F10 |  F11 |  F12 |  Ins |  Del | PgUp | Home | End  |PrtSc |   
 * |------+------+------+------+------+------+------+------+------|  
 * |   0  |   -  |   +  |   BkSpc     | PgDn |   /  |   *  |   -  |   
 * |------+------+------+------+------+------+------+------+------| 
 * |   P  |   [  |   ]  |      \      |   7  |   8  |   9  |      |
 * '------+------+------+------+------+------+------+------|   +  |      
 *        |   ;  |   '  |    Enter    |   4  |   5  |   6  |      |
 *        +------+------+------+------+------+------+------+------|    
 *        |   .  |   /  |    Shift    |   1  |   2  |   3  |      |
 * ,------+------+------+------+------+------+------+------+  Ent |        
 * | Alt   | MyCom |   Win |   Ctrl   |       0     |   .  |      |                         
 * `-------------------------------------------------------+------'                    
 *
 */
[_QWERTY] = {  // layer 0 : default
        // Both hands, had to move F10 down and 0 over 2 to fit actual wiring placements
        {KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F11,   KC_F12,  KC_INS,  KC_DEL,  KC_PGUP,  KC_HOME, KC_END,  KC_PSCR},
        {KC_GRAVE, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,   KC_MINUS, KC_EQL,  KC_0,    KC_BSPC,           KC_PGDN, KC_PSLS, KC_PAST, KC_PMNS},
        {KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,    KC_LBRC,  KC_RBRC, KC_BSLS,           KC_P7,   KC_P8,   KC_P9,   KC_NO},
        {LOWER,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,            KC_SCLN,  KC_QUOT, KC_F10,   KC_ENTER,          KC_P4,   KC_P5,   KC_P6,   KC_PPLS},
        {KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMMA,KC_NO,           KC_DOT,   KC_SLSH, KC_NO, KC_RSFT,           KC_P1,   KC_P2,   KC_P3,   KC_NO},
        {KC_LCTL,  KC_LGUI, KC_LALT, KC_NO,   KC_NO,   KC_SPC,  KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_RALT, KC_MYCM,KC_RGUI, KC_RCTL,          KC_NO,    KC_P0,   KC_PDOT, KC_PENT}
    },

/* Keymap 1: Raise layer
 *
 * ,-----------------------------------------------------------------------,          
 * |        |      |      |      |      |      |      |      |      |      |   
 * |--------+------+------+------+------+------+------+------+------+------|  
 * |        |      |      |      |      |      |      |      |      |      |   
 * |--------+------+------+------+------+------+------+------+------+------| 
 * |        |      |   ^  |      |      |      |      |      |      |      |
 * |--------+------+------+------+------+------+------+------+------+------|      
 * |  Raise |   <  |   v  |   >  |      |      |      |      |      |      |
 * |--------+------+------+------+------+------+------+------+------+------'    
 * |        |      |      |      |      |      |      |      |      |
 * |--------+------+------+------+------+------+------+------+------|        
 * |        |      |      |                                         |                         
 * `----------------------------------------------------------------'          
 * 
 * ,--------------------------------------------------------------,          
 * |      |      |      | Play | Stop |VolUp | TkBk | TkFw | Mute |   
 * |------+------+------+------+------+------+------+------+------|  
 * |      |      |      |             |VolDn |      |      |      |   
 * |------+------+------+------+------+------+------+------+------| 
 * |      |      |      |             |      |      |      |      |
 * '------+------+------+------+------+------+------+------|      |      
 *        |      |      |             |      |      |      |      |
 *        +------+------+------+------+------+------+------+------|    
 *        |      |      |             |      |      |      |      |
 * ,------+------+------+------+------+------+------+------+      |        
 * |       | Calc  |       |          |             |      |      |                         
 * `-------------------------------------------------------+------'                    
 *
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[_LOWER] = {  // layer 0 : default
        // left hand
        {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY, KC_MSTP, KC__VOLUP,   KC_MPRV, KC_MNXT, KC__MUTE},
        {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC__VOLDOWN, KC_TRNS, KC_TRNS, KC_TRNS},
        {KC_TRNS, KC_TRNS, KC_UP,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,     KC_TRNS, KC_TRNS},
        {KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,          KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS},
        {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,     KC_TRNS, KC_TRNS},
        {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                              KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,              KC_TRNS, KC_TRNS}
    },
};

#ifdef AUDIO_ENABLE

float tone_startup[][2]    = SONG(STARTUP_SOUND);
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_colemak[][2]    = SONG(COLEMAK_SOUND);
float tone_plover[][2]     = SONG(PLOVER_SOUND);
float tone_plover_gb[][2]  = SONG(PLOVER_GOODBYE_SOUND);
float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);

float tone_goodbye[][2] = SONG(GOODBYE_SOUND);
#endif


void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_NOTE_ARRAY(tone_qwerty, false, 0);
        #endif
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
      } else {
        layer_off(_LOWER);
      }
      return false;
      break;
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
}

#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(20); // gets rid of tick
    PLAY_NOTE_ARRAY(tone_startup, false, 0);
}

void shutdown_user()
{
    PLAY_NOTE_ARRAY(tone_goodbye, false, 0);
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_NOTE_ARRAY(music_scale, false, 0);
}

#endif
