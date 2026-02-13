#include "app.h"

#include <oxi/oxi.input.h>
#include <oxi/oxi.shader-system.h>

#include <px/px.core.h>

#include <kore3/gpu/api.h>
// #include <kore3/graphics4/graphics.h>
// #include <kore3/graphics4/texture.h>
#include <kore3/math/random.h>
#include <kore3/system.h>

#define VIRTUAL_SCREEN_WIDTH 320
#define VIRTUAL_SCREEN_HEIGHT 200
#define WINDOW_TITLE "px"

#if defined(KINC_LINUX)
#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 200
#else
#define WINDOW_WIDTH 320 * 3
#define WINDOW_HEIGHT 200 * 3
#endif

static const char *ProjectPath = "../design/px/res/pxse.csv"; // TODO (DK) use last saved?
static const char *ExportPath = "./data/pxse/pxse.bin"; // TODO (DK) use last saved?

static struct kinc_g4_render_target     target;
extern uint32_t *                       kinc_internal_g1_image;
static uint8_t                          pixels[VIRTUAL_SCREEN_WIDTH * VIRTUAL_SCREEN_HEIGHT * 4];

static enum kid_handler_state game_key_down_handler( int code, void *ctx ) {
    oxi_input_inject_key_down(ctx, code);
    return kid_handler_state_continue;
}

static enum kid_handler_state game_key_up_handler( int code, void *ctx ) {
    oxi_input_inject_key_up(ctx, code);
    return kid_handler_state_continue;
}

static enum kid_handler_state game_key_press_handler( unsigned character, void *ctx ) {
    oxi_input_inject_key_press(ctx, character);
    return kid_handler_state_continue;
}

static enum kid_handler_state game_mouse_down_handler( int window, int button, int x, int y, void *ctx ) {
    oxi_input_inject_mouse_down(ctx, window, button, x, y);
    return kid_handler_state_continue;
}

static enum kid_handler_state game_mouse_up_handler( int window, int button, int x, int y, void *ctx ) {
    oxi_input_inject_mouse_up(ctx, window, button, x, y);
    return kid_handler_state_continue;
}

static enum kid_handler_state game_mouse_move_handler( int window, int x, int y, int dx, int dy, void *ctx ) {
    oxi_input_inject_mouse_move(ctx, window, x, y, dx, dy);
    return kid_handler_state_continue;
}

static enum kid_handler_state game_mouse_wheel_handler( int window, int delta, void *ctx ) {
    oxi_input_inject_mouse_wheel(ctx, window, delta);
    return kid_handler_state_continue;
}

static enum kid_handler_state game_gamepad_axis_handler( int gamepad, int axis, float value, void *ctx ) {
    oxi_input_inject_gamepad_axis(ctx, gamepad, axis, value);
    return kid_handler_state_continue;
}

static enum kid_handler_state game_gamepad_button_handler( int gamepad, int button, float value, void *ctx ) {
    oxi_input_inject_gamepad_button(ctx, gamepad, button, value);
    return kid_handler_state_continue;
}

#if defined(PX_DEV_UI)
#define GAME_INPUT_CONTEXT_INDEX 2
#else
#define GAME_INPUT_CONTEXT_INDEX 1
#endif

static void init_input( struct app_context *app ) {
    kid_init(&app->kid);
    kid_register_key_down_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_key_down_handler, app->px.oxi.sys.input);
    kid_register_key_up_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_key_up_handler, app->px.oxi.sys.input);
    kid_register_key_press_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_key_press_handler, app->px.oxi.sys.input);
    kid_register_mouse_down_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_mouse_down_handler, app->px.oxi.sys.input);
    kid_register_mouse_up_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_mouse_up_handler, app->px.oxi.sys.input);
    kid_register_mouse_move_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_mouse_move_handler, app->px.oxi.sys.input);
    kid_register_mouse_wheel_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_mouse_wheel_handler, app->px.oxi.sys.input);
    kid_register_gamepad_axis_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_gamepad_axis_handler, app->px.oxi.sys.input);
    kid_register_gamepad_button_handler(&app->kid, GAME_INPUT_CONTEXT_INDEX, game_gamepad_button_handler, app->px.oxi.sys.input);
    kid_set_kinc_callbacks(&app->kid);

    // app->seuck.module_callbacks.input_init();
}

static void setup( struct app_context *app ) {
    oxi_trace("[main.setup]");

    struct px_init_args args = {
        .seuck = {
            .screen = {
                .width = VIRTUAL_SCREEN_WIDTH,
                .height = VIRTUAL_SCREEN_HEIGHT,
            },
        }
    };

    px_init(&app->px, &args);
    init_input(app);

#if defined(PX_DEV_UI)
    devui_init(app, ProjectPath, ExportPath);
#endif
}

#if defined(PX_DEV_UI)
static void process_devui( struct app_context *app ) {
    kinc_g4_begin(0);
    kinc_g4_clear(KINC_G4_CLEAR_COLOR | KINC_G4_CLEAR_DEPTH | KINC_G4_CLEAR_STENCIL, 0xff000000, 1.0f, 0);

    gui_update(app);
    gui_draw(app);

    kinc_g4_end(0);
    kinc_g4_swap_buffers();
}
#endif

static void process( struct app_context *app ) {
	kinc_g4_render_target_t *targets = { &target };
	kinc_g4_set_render_targets(&targets, 1);
    kinc_g4_clear(KINC_G4_CLEAR_COLOR | KINC_G4_CLEAR_DEPTH | KINC_G4_CLEAR_STENCIL, 0xff000000, 1.0f, 0);

    px_update(&app->px);
    px_draw(&app->px);
    px_post_update(&app->px);

	kinc_g4_restore_render_target();
	kinc_g4_render_target_get_pixels(&target, pixels);

	kinc_g1_begin();
	uint8_t *g1image = (uint8_t *)kinc_internal_g1_image;

    if (kinc_g4_render_targets_inverted_y()) {
        size_t bytes_per_row = VIRTUAL_SCREEN_WIDTH * 4;
        unsigned dst_idx = 0;
        unsigned src_idx = (VIRTUAL_SCREEN_HEIGHT - 1) * bytes_per_row;

        for (unsigned row = 0; row < VIRTUAL_SCREEN_HEIGHT; ++row) {
            memcpy(g1image + dst_idx, pixels + src_idx, bytes_per_row);
            dst_idx += bytes_per_row;
            src_idx -= bytes_per_row;
        }
    } else {
        for (int i = 0; i < VIRTUAL_SCREEN_WIDTH * VIRTUAL_SCREEN_HEIGHT * 4; ++i) {
            g1image[i] = pixels[i];
        }
    }

	kinc_g1_end();
}

static void update( void *ctx ) {
    struct app_context *app = ctx;
    memset(&app->metrics, 0, sizeof(app->metrics));

#if defined(PX_DEV_UI)
    if (app->gui.visible) {
        process_devui(app);
    } else {
        process(app);
    }
#else
    process(app);
#endif
}

static struct app_context app = { 0 };

int kickstart( int argc, char *argv[] ) {
    kinc_init(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL);

	kinc_g4_render_target_init(&target, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT, KINC_G4_RENDER_TARGET_FORMAT_32BIT, 0, 0);
	kinc_g1_init(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
    kinc_g1_set_texture_minification_filter(KINC_G1_TEXTURE_FILTER_POINT);
    kinc_g1_set_texture_magnification_filter(KINC_G1_TEXTURE_FILTER_POINT);

    setup(&app);
    kinc_set_update_callback(update, &app);
    kinc_start();

#if defined(PX_DEV_UI)
    gui_shutdown(&app);
#endif

    return 0;
}
