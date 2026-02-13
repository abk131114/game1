#include "px.core.h"

#include <oxi/oxi.input.h>
#include <oxi/oxi.shader-system.h>
#include <oxi/oxi.spritebatcher.h>

void pxi_game_static_init( struct px_context *px );
void pxi_menu_static_init( struct px_context *px );

static void forward_key_press( int code, void *ctx ) {
    tupni_inject_key_press(ctx, code);
}

static void forward_key_release( int code, void *ctx) {
    tupni_inject_key_release(ctx, code);
}

static void forward_gamepad_axis( int gamepad, int axis, float value, void *ctx ) {
    tupni_inject_gamepad_axis(ctx, gamepad, axis, value);
}

static void forward_gamepad_button( int gamepad, int button, float value, void *ctx) {
    tupni_inject_gamepad_button(ctx, gamepad, button, value);
}

void px_init( struct px_context *px, struct px_init_args *args ) {
    struct oxi_init_args oxi_args = {
        .screen = {
            .width = args->seuck.screen.width,
            .height = args->seuck.screen.height,
        }
    };

	oxi_init(&px->oxi, &oxi_args);

    uint16_t num_sprites = 512;
    px->batcher = oxi_alloc(oxi_spritebatcher_alloc_size(num_sprites), NULL);
    oxi_spritebatcher_init(px->batcher, &px->oxi, num_sprites);

    seuck_init(&px->seuck, &args->seuck);
    tupni_init(&px->tupni);
    pxi_menu_static_init(px);
    pxi_game_static_init(px);
    pxi_intro_init(px);

    oxi_input_register_key_press_handler(px->oxi.sys.input, forward_key_press, &px->tupni);
    oxi_input_register_key_release_handler(px->oxi.sys.input, forward_key_release, &px->tupni);
    oxi_input_register_gamepad_axis_handler(px->oxi.sys.input, forward_gamepad_axis, &px->tupni);
    oxi_input_register_gamepad_button_handler(px->oxi.sys.input, forward_gamepad_button, &px->tupni);
}

void px_update( struct px_context *px ) {
    tupni_update(&px->tupni);
    oxi_shader_system_update(px->oxi.sys.shaders);

    switch (px->state) {
        case PX_STATE_INTRO:
            pxi_intro_update(px);
            break;

        case PX_STATE_MENU:
            pxi_menu_update(px);
            break;

        case PX_STATE_GAME:
            pxi_game_update(px);
            break;
    }
}

void px_post_update( struct px_context *px ) {
    tupni_reset(&px->tupni);
}

void px_draw( struct px_context *px ) {
    switch (px->state) {
        case PX_STATE_INTRO:
            pxi_intro_draw(px);
            break;

        case PX_STATE_MENU:
            pxi_menu_draw(px);
            break;

        case PX_STATE_GAME:
            pxi_game_draw(px);
            break;
    }
}
