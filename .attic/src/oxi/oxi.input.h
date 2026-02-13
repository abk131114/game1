#pragma once

#include "oxi.core.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct oxi_input_system;

// TODO (DK) add num handlers? currently only a single callback can be connected
size_t  oxi_input_system_alloc_size( void );
void    oxi_input_system_init( struct oxi_input_system *sys );
// void    oxi_input_system_update( struct oxi_input_system *sys );
// void    oxi_input_system_reset( struct oxi_input_system *sys );

void    oxi_input_inject_key_down( struct oxi_input_system *sys, int code );
void    oxi_input_inject_key_up( struct oxi_input_system *sys, int code );
void    oxi_input_inject_key_press( struct oxi_input_system *sys, unsigned character );
void    oxi_input_inject_mouse_down( struct oxi_input_system *sys, int window, int button, int x, int y );
void    oxi_input_inject_mouse_up( struct oxi_input_system *sys, int window, int button, int x, int y );
void    oxi_input_inject_mouse_move( struct oxi_input_system *sys, int window, int x, int y, int dx, int dy );
void    oxi_input_inject_mouse_wheel( struct oxi_input_system *sys, int window, int delta );
void    oxi_input_inject_gamepad_axis( struct oxi_input_system *sys, int gamepad, int axis, float value );
void    oxi_input_inject_gamepad_button( struct oxi_input_system *sys, int gamepad, int button, float value );

// TODO (DK) allow multiple? currently only a single callback can be connected
 // how to handle priority? just inject `int slot` and let the user take care of it
void    oxi_input_register_key_press_handler( struct oxi_input_system *sys, void (*fn)( int, void *), void *ctx );
void    oxi_input_register_key_release_handler( struct oxi_input_system *sys, void (*fn)( int, void *), void *ctx );

void    oxi_input_register_gamepad_axis_handler( struct oxi_input_system *sys, void (*fn)( int, int, float, void *), void *ctx );
void    oxi_input_register_gamepad_button_handler( struct oxi_input_system *sys, void (*fn)( int, int, float, void *), void *ctx );

#if defined(__cplusplus)
}
#endif
