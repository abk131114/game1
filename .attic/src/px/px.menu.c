#include "px.core.h"

#include <bmfont/bmfont-oxi-painter.h>
#include <kinc/audio1/audio.h>
#include <kinc/input/keyboard.h> // TODO (DK) wrap in oxi?
#include <kinc/system.h> // TODO (DK) wrap in oxi?
#include <oxi/oxi.io.h>
#include <oxi/oxi.spritebatcher.h>
#include <oxi/oxi.texture-system.h>

#include <math.h> // TODO (DK) wrap in oxi?

#define ACTIONMAP 0

static const char *DataFile = "data/pxse/pxse.bin";

extern struct seuck_menu_str PX_STRINGS[];
extern struct seuck_menu_toggle_data PX_TOGGLES[];
extern struct seuck_menu_submenu_data PX_SUBMENUS[];
extern struct seuck_menu_label_data PX_LABELS[];
extern struct seuck_menu_action_data PX_ACTIONS[];
extern struct seuck_menu_item_data PX_MENU_ITEMS[];
extern struct seuck_menu_sound_data PX_MENU_SOUNDS[];
extern struct seuck_menu_data PX_MENUS[];

static struct seuck_menu_content MENU_CONTENTS = {
    .actions = PX_ACTIONS,
    .labels = PX_LABELS,
    .menu_items = PX_MENU_ITEMS,
    .menus = PX_MENUS,
    .strings = PX_STRINGS,
    .submenus = PX_SUBMENUS,
    .sounds = PX_MENU_SOUNDS,
    .toggles = PX_TOGGLES,
};

static void play_menu_sound( struct px_context *px, enum seuck_menu_action_kind kind ) {
    uint16_t sound_idx = MENU_CONTENTS.sounds[kind].sound_idx;
    kinc_a1_play_sound(px->menu.sfx.sound[kind], false, 1.0f, false);
}

static void up( void *ctx ) {
    seuck_menu_nav_up(&((struct px_context *)ctx)->seuck);
    play_menu_sound(ctx, seuck_menu_action_kind_nav);
}

static void down( void *ctx ) {
    struct px_context *px = ctx;
    seuck_menu_nav_down(&((struct px_context *)ctx)->seuck);
    play_menu_sound(ctx, seuck_menu_action_kind_nav);
}

static void left( void *ctx ) {
    seuck_menu_nav_left(&((struct px_context *)ctx)->seuck);
    play_menu_sound(ctx, seuck_menu_action_kind_nav);
}

static void right( void *ctx ) {
    struct px_context *px = ctx;
    seuck_menu_nav_right(&((struct px_context *)ctx)->seuck);
    play_menu_sound(ctx, seuck_menu_action_kind_nav);
}

static void create_menu( struct px_context *px, int idx ) {
    struct bmfont_file *fnt = px->res.fonts.font2;
    struct oxi_texture *fnt_tex = px->res.textures.font2;
    px->seuck.menu.state.num_items = seuck_build_menu(&px->seuck, fnt, &MENU_CONTENTS, idx, fnt_tex);
    px->seuck.menu.state.selected_idx = MENU_CONTENTS.menus[idx].default_selection;
}

static void activate( void *ctx ) {
    struct px_context *px = ctx;
    // seuck_menu_nav_activate(&px->seuck);

    int selected = px->seuck.menu.state.selected_idx;
    struct seuck_menu_item *item = &px->seuck.menu.items[selected];

    play_menu_sound(ctx, seuck_menu_action_kind_activate);

    switch (item->kind) {
        case seuck_menu_item_kind_label: {
        } break;

        case seuck_menu_item_kind_empty_separator: {
        } break;

        case seuck_menu_item_kind_toggle: {
            switch (item->userdata) {
                case 0: {
                    px->options.num_players += 1;
                    item->text_idx += 1;

                    if (px->options.num_players > SEUCK_MAX_PLAYERS) {
                        px->options.num_players -= SEUCK_MAX_PLAYERS;
                        item->text_idx -= SEUCK_MAX_PLAYERS;
                    }
                } break;


            }
        } break;

        case seuck_menu_item_kind_submenu: {
            switch (item->userdata) {
                case 1: { create_menu(px, 1); } break;
                case 2: { create_menu(px, 2); } break;
                case 12: { create_menu(px, 0); } break;
                case 7: { create_menu(px, 6); } break;
            }
        } break;

        case seuck_menu_item_kind_action: {
            switch (item->userdata) {
                case 3: {
                    px->state = PX_STATE_GAME;
                    pxi_game_init(px);
                } break;

                case 4: {
                    px->options.start_level_index = 0;
                    create_menu(px, 0);
                } break;

                // TODO (DK) not selectable if not unlocked during this session?
                case 5: {
                    px->options.start_level_index = 1;
                    create_menu(px, 0);
                } break;

                // TODO (DK) not selectable if not unlocked during this session?
                case 6: {
                    px->options.start_level_index = 2;
                    create_menu(px, 0);
                } break;

                case 13: {
                    int p = 0;
                    px->menu.tmp.selected_player_for_craft_choice = p;

                    switch (px->options.craft[p]) {
                        case 0: create_menu(px, 3); break;
                        case 1: create_menu(px, 4); break;
                        case 2: create_menu(px, 5); break;
                    }
                } break;

                case 14: {
                    int p = 1;
                    px->menu.tmp.selected_player_for_craft_choice = p;

                    switch (px->options.craft[p]) {
                        case 0: create_menu(px, 3); break;
                        case 1: create_menu(px, 4); break;
                        case 2: create_menu(px, 5); break;
                    }
                } break;
            }
        } break;
    }
}

void pxi_menu_static_init( struct px_context *px ) {
    struct tupni_context *t = &px->tupni;

    enum pxi_menu_input_action action;

    action = pxi_menu_input_action_up;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_button);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 0);
    tupni_action_set_button_start_callback(t, ACTIONMAP, action, up, px);
    tupni_action_enable(t, ACTIONMAP, action);

    action = pxi_menu_input_action_down;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_button);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 1);
    tupni_action_set_button_start_callback(t, ACTIONMAP, action, down, px);
    tupni_action_enable(t, ACTIONMAP, action);

    action = pxi_menu_input_action_left;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_button);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 2);
    tupni_action_set_button_start_callback(t, ACTIONMAP, action, left, px);
    tupni_action_enable(t, ACTIONMAP, action);

    action = pxi_menu_input_action_right;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_button);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 3);
    tupni_action_set_button_start_callback(t, ACTIONMAP, action, right, px);
    tupni_action_enable(t, ACTIONMAP, action);

    action = pxi_menu_input_action_activate;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_button);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 4);
    tupni_action_set_button_start_callback(t, ACTIONMAP, action, activate, px);
    tupni_action_enable(t, ACTIONMAP, action);
}

void pxi_menu_init( struct px_context *px ) {
    px->options.num_players = 1;
    px->options.craft[0] = 2;
    px->options.craft[1] = 0;
    px->options.difficulty = 1;
    px->options.autofire[0] = 1;
    px->options.autofire[1] = 1;
    px->options.start_level_index = 0;

    px->res.files.data_file_id = oxi_io_load_blob(px->oxi.sys.io, DataFile, strlen(DataFile));

    tupni_actionmap_set_active_index(&px->tupni, ACTIONMAP);
}

static enum seuck_load_status process_loading( struct px_context *px ) {
    struct oxi_io_system *io = px->oxi.sys.io;

    if (px->data == NULL) {
        if (!oxi_io_is_loaded(io, px->res.files.data_file_id)) {
            return SEUCK_LOAD_STATUS_IN_PROGRESS;
        }

        px->data = oxi_io_get_blob_data(io, px->res.files.data_file_id)->bytes;
        pxd_init(px->data);

        // TODO (DK) don't hardcode the id's?
        px->res.files.player = oxi_io_load_image(io, pxd_image_uri(px->data, 2), pxd_image_uri_len(px->data, 2));
        px->res.files.font1_bin = oxi_io_load_blob(io, pxd_blob_uri(px->data, 8), pxd_blob_uri_len(px->data, 8));
        px->res.files.font1_img = oxi_io_load_image(io, pxd_image_uri(px->data, 15), pxd_image_uri_len(px->data, 15));
        px->res.files.font2_bin = oxi_io_load_blob(io, pxd_blob_uri(px->data, 9), pxd_blob_uri_len(px->data, 9));
        px->res.files.font2_img = oxi_io_load_image(io, pxd_image_uri(px->data, 16), pxd_image_uri_len(px->data, 16));

        for (int i = 0; i < seuck_menu_action_kind_MAX; ++i) {
            uint16_t sidx = MENU_CONTENTS.sounds[i].sound_idx;
            const char *url = pxd_sound_uri(px->data, sidx);
            size_t url_len = pxd_sound_uri_len(px->data, sidx);
            px->menu.res.sound[i] = oxi_io_load_sound(px->oxi.sys.io, url, url_len);
        }

        return SEUCK_LOAD_STATUS_IN_PROGRESS;
    }

    if (px->res.textures.font1 == NULL) {
        if (!oxi_io_is_loaded(io, px->res.files.font1_bin)) {
            return SEUCK_LOAD_STATUS_IN_PROGRESS;
        }

        if (!oxi_io_is_loaded(io, px->res.files.font1_img)) {
            return SEUCK_LOAD_STATUS_IN_PROGRESS;
        }

        px->res.textures.font1 = oxi_texture_from_image(px->oxi.sys.textures, &oxi_io_get_image_data(io, px->res.files.font1_img)->image);
        px->res.fonts.font1 = oxi_io_get_blob_data(io, px->res.files.font1_bin)->bytes;
        bmfont_init(px->res.fonts.font1, px->res.fonts.font1);
    }

    if (px->res.textures.font2 == NULL) {
        if (!oxi_io_is_loaded(io, px->res.files.font2_bin)) {
            return SEUCK_LOAD_STATUS_IN_PROGRESS;
        }

        if (!oxi_io_is_loaded(io, px->res.files.font2_img)) {
            return SEUCK_LOAD_STATUS_IN_PROGRESS;
        }

        px->res.textures.font2 = oxi_texture_from_image(px->oxi.sys.textures, &oxi_io_get_image_data(io, px->res.files.font2_img)->image);
        px->res.fonts.font2 = oxi_io_get_blob_data(io, px->res.files.font2_bin)->bytes;
        bmfont_init(px->res.fonts.font2, px->res.fonts.font2);
    }

    for (int i = 0; i < seuck_menu_action_kind_MAX; ++i) {
        if (!oxi_io_is_loaded(io, px->menu.res.sound[i])) {
            return SEUCK_LOAD_STATUS_IN_PROGRESS;
        }

        px->menu.sfx.sound[i] = oxi_io_get_sound_data(io, px->menu.res.sound[i])->a1;
    }

    create_menu(px, 0);

    return SEUCK_LOAD_STATUS_SUCCESS;
}

void pxi_menu_update( struct px_context *px ) {
    switch (px->menu.setup) {
        case PX_STATE_SETUP_LOAD: {
            switch (process_loading(px)) {
                case SEUCK_LOAD_STATUS_FAILURE:
                    break;

                case SEUCK_LOAD_STATUS_IN_PROGRESS:
                    break;

                case SEUCK_LOAD_STATUS_SUCCESS:
                    px->menu.setup = PX_STATE_SETUP_FINISHED;
                    tupni_actionmap_enable(&px->tupni, ACTIONMAP);
                    break;
            }
        } break;

        case PX_STATE_SETUP_FINISHED:
            break;
    }
}

static void draw_ui( struct px_context *px ) {
    int x = 100, y = 100;
    int yo = 0;
    int font_height = 9; // TODO (DK) font height

    uint8_t fade = ((sin(kinc_time() * 16) + 1) / 2) * 255;

    for (int i = 0; i < px->seuck.menu.state.num_items; ++i) {
        uint8_t idx = px->seuck.menu.items[i].text_idx;

        if (idx > 0) {
            struct seuck_bmfont_string *txt = &px->seuck.menu.strings[idx];
            struct oxi_color color = oxi_rgba_c(0xff, 0xff, 0xff, i == px->seuck.menu.state.selected_idx ? fade : 0xff);
            bmfont_draw_chars_tinted(px->batcher, txt->tex, txt->ch, txt->len, x, y + yo, color.impl.rgba);
        }

        yo += font_height;
    }
}

void pxi_menu_draw( struct px_context *px ) {
    struct oxi_spritebatcher *b = px->batcher;

    oxi_spritebatcher_begin(b);
        draw_ui(px);
    oxi_spritebatcher_end(b);
}
