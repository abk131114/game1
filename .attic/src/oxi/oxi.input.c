#include "oxi.input.h"

#include <kinc/input/gamepad.h>
#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>

#include <string.h>

// enum oxi_keystate_flag {
//     oxi_keystate_flag_was_pressed_this_frame,
//     oxi_keystate_flag_state_changed,
//     oxi_keystate_flag_is_down,
// };

// #define OXI_INPUT_MAX_HANDLERS 1

struct oxi_input_system {
    void (*key_press_fn)( int, void* );
    void *key_press_ctx;

    void (*key_release_fn)( int, void* );
    void *key_release_ctx;

    void (*gamepad_axis_fn)( int, int, float, void* );
    void *gamepad_axis_ctx;

    void (*gamepad_button_fn)( int, int, float, void* );
    void *gamepad_button_ctx;

    // uint8_t                     keys[OXI_INPUT_MAX_KEYS];
    // struct oxi_mouse_state      mouse;
    // struct oxi_gamepad_state    gamepad[OXI_INPUT_MAX_GAMEPADS];
};

// static void press_key( uint8_t *key ) {
//     if (oxi_bit_has(*key, oxi_keystate_flag_state_changed)) {
//         *key = oxi_bit_clear(*key, oxi_keystate_flag_was_pressed_this_frame);
//     } {
//         *key = oxi_bit_set(*key, oxi_keystate_flag_was_pressed_this_frame);
//     }

//     *key = oxi_bit_set(*key, oxi_keystate_flag_state_changed);
//     *key = oxi_bit_set(*key, oxi_keystate_flag_is_down);
// }

// static void release_key( uint8_t *key ) {
//     *key = oxi_bit_clear(*key, oxi_keystate_flag_is_down);
// }

// static void reset_key( uint8_t *key ) {
//     if (!oxi_bit_has(*key, oxi_keystate_flag_is_down)) {
//         *key = oxi_bit_clear(*key, oxi_keystate_flag_state_changed);
//         *key = oxi_bit_clear(*key, oxi_keystate_flag_was_pressed_this_frame);
//     } else if (oxi_bit_has(*key, oxi_keystate_flag_state_changed)) {
//         *key = oxi_bit_clear(*key, oxi_keystate_flag_was_pressed_this_frame);
//     }
// }

static void key_down_handler( int code, struct oxi_input_system *sys ) {
    // if (code < 0 || code >= OXI_INPUT_MAX_KEYS) {
    //     oxi_warn("[input.key_down_handler] ignoring keycode '%i'", code);
    //     return;
    // }

    // press_key(&sys->keys[code]);
    sys->key_press_fn(code, sys->key_press_ctx);
}

static void key_up_handler( int code, struct oxi_input_system *sys ) {
    // if (code < 0 || code >= OXI_INPUT_MAX_KEYS) {
    //     oxi_warn("[input.key_up_handler] ignoring keycode '%i'", code);
    //     return;
    // }

    // release_key(&sys->keys[code]);
    sys->key_release_fn(code, sys->key_release_ctx);
}

static void key_press_handler( unsigned int character, struct oxi_input_system *sys ) {
    // oxi_input_event_t *e = &sys->event_queue[sys->event_queue_top];
    // e->kind = OXI_INPUT_EVENT_KIND_KEY_PRESS;
    // e->payload.key.character = character;
    // sys->event_queue_top += 1;
    // sys->event_queue_top %= OXI_INPUT_EVENT_QUEUE_SIZE;
}

static uint8_t * translate_mouse_button( struct oxi_input_system *sys, int button ) {
    // switch (button) {
    //     case 0: return &sys->mouse.left;
    //     case 1: return &sys->mouse.right;
    //     case 2: return &sys->mouse.middle;
    //     default:
    //         oxi_warn("[input.translate_mouse_button] unmapped mouse button (%i)", button);
    //         return NULL;
    // }
    return NULL;
}

static void mouse_down_handler( int w, int b, int x, int y, struct oxi_input_system *sys ) {
    // sys->mouse.window.x = x;
    // sys->mouse.window.y = y;
    // uint8_t *button = translate_mouse_button(sys, b);

    // if (button == NULL) {
    //     return;
    // }

    // press_key(button);
}

static void mouse_up_handler( int w, int b, int x, int y, struct oxi_input_system *sys ) {
    // sys->mouse.window.x = x;
    // sys->mouse.window.y = y;
    // uint8_t *button = translate_mouse_button(sys, b);

    // if (button == NULL) {
    //     return;
    // }

    // release_key(button);
}

static void mouse_move_handler( int w, int x, int y, int mx, int my, struct oxi_input_system *sys ) {
    // sys->mouse.window.x = x;
    // sys->mouse.window.y = y;
}

static void mouse_wheel_handler( int w, int delta, struct oxi_input_system *sys ) {
}

static void gamepad_axis_handler( int gamepad, int axis, float value, struct oxi_input_system *sys ) {
    // if (gamepad < 0 || gamepad > OXI_INPUT_MAX_GAMEPADS) {
    //     oxi_warn("[oxi.input.gamepad_axis_handler] gamepad '%i' out of range (0...%i) ", gamepad, OXI_INPUT_MAX_GAMEPADS);
    //     return;
    // }

    // if (axis < 0 || axis > OXI_INPUT_MAX_GAMEPAD_AXIS) {
    //     oxi_warn("[oxi.input.gamepad_axis_handler][%i] axis '%i' is out of range (0...%i) ", gamepad, axis, OXI_INPUT_MAX_GAMEPAD_AXIS);
    //     return;
    // }

    // sys->gamepad[gamepad].axis[axis] = value;

    sys->gamepad_axis_fn(gamepad, axis, value, sys->gamepad_axis_ctx);
}

static void gamepad_button_handler( int gamepad, int button, float value, struct oxi_input_system *sys ) {
    // if (gamepad < 0 || gamepad > OXI_INPUT_MAX_GAMEPADS) {
    //     oxi_warn("[oxi.input.gamepad_button_handler] gamepad '%i' out of range (0...%i) ", gamepad, OXI_INPUT_MAX_GAMEPADS);
    //     return;
    // }

    // if (button < 0 || button > OXI_INPUT_MAX_GAMEPAD_BUTTONS) {
    //     oxi_warn("[oxi.input.gamepad_button_handler][%i] button '%i' is out of range (0...%i) ", gamepad, button, OXI_INPUT_MAX_GAMEPAD_BUTTONS);
    //     return;
    // }

    // sys->gamepad[gamepad].button[button] = value;

    sys->gamepad_button_fn(gamepad, button, value, sys->gamepad_button_ctx);
}

void oxi_input_inject_key_down( struct oxi_input_system *sys, int code ) {
    key_down_handler(code, sys);
}

void oxi_input_inject_key_up( struct oxi_input_system *sys, int code ) {
    key_up_handler(code, sys);
}

void oxi_input_inject_key_press( struct oxi_input_system *sys, unsigned character ) {
    key_press_handler(character, sys);
}

void oxi_input_inject_mouse_down( struct oxi_input_system *sys, int window, int button, int x, int y ) {
    mouse_down_handler(window, button, x, y, sys);
}

void oxi_input_inject_mouse_up( struct oxi_input_system *sys, int window, int button, int x, int y ) {
    mouse_up_handler(window, button, x, y, sys);
}

void oxi_input_inject_mouse_move( struct oxi_input_system *sys, int window, int x, int y, int dx, int dy ) {
    mouse_move_handler(window, x, y, dx, dy, sys);
}

void oxi_input_inject_mouse_wheel( struct oxi_input_system *sys, int window, int delta ) {
    mouse_wheel_handler(window, delta, sys);
}

void oxi_input_inject_gamepad_axis( struct oxi_input_system *sys, int gamepad, int axis, float value ) {
    gamepad_axis_handler(gamepad, axis, value, sys);
}

void oxi_input_inject_gamepad_button( struct oxi_input_system *sys, int gamepad, int button, float value ) {
    gamepad_button_handler(gamepad, button, value, sys);
}

size_t oxi_input_system_alloc_size( void ) {
    return sizeof(struct oxi_input_system);
}

void oxi_input_system_init( struct oxi_input_system *sys ) {
    memset(sys, 0, sizeof(*sys));
}

// void oxi_input_system_update( struct oxi_input_system *sys ) {
// }

// void oxi_input_system_reset( struct oxi_input_system *sys ) {
//     for (int i = 0; i < OXI_INPUT_MAX_KEYS; ++i) {
//         reset_key(&sys->keys[i]);
//     }
// }

// bool oxi_input_key_is_down( struct oxi_input_system *sys, int code ) {
//     if (code < 0 || code >= OXI_INPUT_MAX_KEYS) {
//         oxi_warn("[input.oxi_input_key_is_down] ignoring keycode '%i'", code);
//         return false;
//     }

//     return oxi_bit_has(sys->keys[code], oxi_keystate_flag_is_down);
// }

// bool oxi_input_key_is_pressed( struct oxi_input_system *sys, int code ) {
//     if (code < 0 || code >= OXI_INPUT_MAX_KEYS) {
//         oxi_warn("[input.oxi_input_key_is_pressed] ignoring keycode '%i'", code);
//         return false;
//     }

//     return oxi_bit_has(sys->keys[code], oxi_keystate_flag_was_pressed_this_frame);
// }

// bool oxi_input_key_is_released( struct oxi_input_system *sys, int code ) {
//     if (code < 0 || code >= OXI_INPUT_MAX_KEYS) {
//         oxi_warn("[input.oxi_input_key_is_released] ignoring keycode '%i'", code);
//         return false;
//     }

//     return  oxi_bit_has(sys->keys[code], oxi_keystate_flag_state_changed)
//         &&  !oxi_bit_has(sys->keys[code], oxi_keystate_flag_is_down)
//     ;
// }

void oxi_input_register_key_press_handler( struct oxi_input_system *sys, void (*fn)( int, void *), void *ctx ) {
    sys->key_press_fn = fn;
    sys->key_press_ctx = ctx;
}

void oxi_input_register_key_release_handler( struct oxi_input_system *sys, void (*fn)( int, void *), void *ctx ) {
    sys->key_release_fn = fn;
    sys->key_release_ctx = ctx;
}

void oxi_input_register_gamepad_axis_handler( struct oxi_input_system *sys, void (*fn)( int, int, float, void *), void *ctx ) {
    sys->gamepad_axis_fn = fn;
    sys->gamepad_axis_ctx = ctx;

}

void oxi_input_register_gamepad_button_handler( struct oxi_input_system *sys, void (*fn)( int, int, float, void *), void *ctx ) {
    sys->gamepad_button_fn = fn;
    sys->gamepad_button_ctx = ctx;
}
