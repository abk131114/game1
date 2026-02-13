#pragma once

#if defined(PX_DEV_UI)
#include <imgui-kinc.h>
#include "editor/editor.data.h"
#endif

#include <kid/kid.h>
#include <px/px.core.h>

#if defined(__cplusplus)
extern "C" {
#endif

enum app_input_context_kind {
    APP_INPUT_CONTEXT_KIND_MAIN,
#if defined(PX_DEV_UI)
    APP_INPUT_CONTEXT_KIND_IMGUI,
#endif
    APP_INPUT_CONTEXT_KIND_GAME,
};

#if defined(PX_DEV_UI)
struct gui_texture_cache_item {
    oxi_id                  id;
    struct kinc_g4_texture *texture;
    uint16_t                atlas_ref;
};

struct gui_anim_cache_item {
    int         frame, counter;
    uint16_t    anim_ref;
};
#endif

struct app_context {
    struct {
        uint64_t render, tick;
#if defined(PX_DEV_UI)
        uint64_t gui_render, gui_tick;
#endif
    } metrics;

    struct kid_context kid;
    struct px_context px;

#if defined(PX_DEV_UI)
    struct {
        struct imgui_kinc_context ctx;
        struct gui_data *data;
        bool visible;
        char project_path[SEUCK_MAX_RESOURCE_STR_LENGTH];
        char export_path[SEUCK_MAX_RESOURCE_STR_LENGTH];
        bool is_ctrl_down;
        struct {
            struct gui_texture_cache_item *texture_cache;
            struct gui_anim_cache_item *anim_cache;
        } res;

        struct {
            uint8_t show_frames : 1;
            uint8_t show_anims : 1;
        } opts;
    } gui;
#endif
};

#if defined(PX_DEV_UI)
void                            devui_init( struct app_context *app, const char *project_path, const char *export_path );

void                            gui_init( struct app_context *app, int kid_context );
void                            gui_shutdown( struct app_context *app );
void                            gui_update( struct app_context *app );
void                            gui_draw( struct app_context *app );
struct kinc_g4_texture *        gui_texture_find( struct app_context *app, uint16_t atlas_ref );
struct gui_anim_cache_item *    gui_anim_cache_item_find( struct app_context *app, uint16_t id );
#endif

#if defined(__cplusplus)
}
#endif
