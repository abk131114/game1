#include "app.h"
#include <kinc/input/keyboard.h>

#if defined(PX_DEV_UI)

static enum kid_handler_state devui_key_down_handler( int code, void *ctx ) {
    struct app_context *app = ctx;

    switch (code) {
        case KINC_KEY_BACK_QUOTE: {
            app->gui.visible = !app->gui.visible;

            if (app->gui.visible) {
                kid_enable_dispatcher(&app->kid, APP_INPUT_CONTEXT_KIND_IMGUI);
                kid_disable_dispatcher(&app->kid, APP_INPUT_CONTEXT_KIND_GAME);
            } else {
                kid_disable_dispatcher(&app->kid, APP_INPUT_CONTEXT_KIND_IMGUI);
                kid_enable_dispatcher(&app->kid, APP_INPUT_CONTEXT_KIND_GAME);
            }
        } break;

        case KINC_KEY_CONTROL: {
            app->gui.is_ctrl_down = true;
        } break;

        case KINC_KEY_S: {
            if (app->gui.visible && app->gui.is_ctrl_down) {
                gui_data_save(app->gui.project_path, app->gui.data);
                return kid_handler_state_halt;
            }
        } break;

        case KINC_KEY_E: {
            if (app->gui.visible && app->gui.is_ctrl_down) {
                gui_data_export(app->gui.export_path, app->gui.data);
                return kid_handler_state_halt;
            }
        } break;
    }

    return kid_handler_state_continue;
}

static enum kid_handler_state devui_key_up_handler( int code, void *ctx ) {
    struct app_context *app = ctx;

    switch (code) {
        case KINC_KEY_CONTROL: {
            app->gui.is_ctrl_down = false;
        } break;
    }

    return kid_handler_state_continue;
}

void devui_init( struct app_context *app, const char *project_path, const char *export_path ) {
    kid_enable_dispatcher(&app->kid, APP_INPUT_CONTEXT_KIND_IMGUI);
    kid_disable_dispatcher(&app->kid, APP_INPUT_CONTEXT_KIND_GAME);
    kid_register_key_down_handler(&app->kid, 0, devui_key_down_handler, app);
    kid_register_key_up_handler(&app->kid, 0, devui_key_up_handler, app);

	strncpy(app->gui.project_path, project_path, SEUCK_MAX_RESOURCE_STR_LENGTH);
	strncpy(app->gui.export_path, export_path, SEUCK_MAX_RESOURCE_STR_LENGTH);
    gui_init(app, 1);
}

#endif
