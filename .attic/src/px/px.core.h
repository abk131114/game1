#pragma once

#include "px.game.data.h"

#include <bmfont/bmfont.h>
#include <oxi/oxi.core.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct kinc_a1_channel;
struct oxi_material;
struct oxi_texture;

enum px_state {
    PX_STATE_INTRO,
    PX_STATE_MENU,
    PX_STATE_GAME,
};

enum px_state_setup {
    PX_STATE_SETUP_LOAD,
    PX_STATE_SETUP_FINISHED,
};

enum px_game_phase {
    PX_GAME_PHASE_INTRO,
    PX_GAME_PHASE_GAMEPLAY,
    PX_GAME_PHASE_TRANSITION_TO_BOSS,
    PX_GAME_PHASE_OUTRO,
};

struct px_init_args {
    struct seuck_init_args seuck;
};

enum pxi_gameplay_input_action {
    pxi_gameplay_input_action_move_1,
    pxi_gameplay_input_action_fire_1,
    pxi_gameplay_input_action_use_1,

    pxi_gameplay_input_action_move_2,
    pxi_gameplay_input_action_fire_2,
    pxi_gameplay_input_action_use_2,
};

enum pxi_menu_input_action {
    pxi_menu_input_action_up,
    pxi_menu_input_action_down,
    pxi_menu_input_action_left,
    pxi_menu_input_action_right,
    pxi_menu_input_action_activate,
};

// struct px_input_wasd_composite {
//     struct tupni_composite_base base;
//     struct tupni_control up, down, left, right;
// };

struct px_context {
    struct oxi_context oxi;
    struct seuck_context seuck;
    struct tupni_context tupni;
    struct oxi_spritebatcher *batcher; // TODO (DK) move into oxi_context? probably not.

    struct px_data *data;

    struct px_loadout   player_items[SEUCK_MAX_PLAYERS];
    struct px_loadout   player_item_cooldown[SEUCK_MAX_PLAYERS];
    uint8_t             player_powerup[SEUCK_MAX_PLAYERS]; // 0 = nothing, 1 = speedup, ...
    uint8_t             player_selected_weapon[SEUCK_MAX_PLAYERS];
    uint8_t             player_lives[SEUCK_MAX_PLAYERS];

    struct {
        uint8_t num_players;
        uint8_t craft[SEUCK_MAX_PLAYERS];
        uint8_t difficulty;
        uint8_t autofire[SEUCK_MAX_PLAYERS];
        uint8_t start_level_index;
    } options;

    // struct {
    //     struct {
    //         tupni_control_id w, a, s, d, ctrl, space, left_stick_h, left_stick_v;
    //     } controls;
    // } input;

    struct {
        enum px_state_setup setup;

        struct {
            oxi_id sound[seuck_menu_action_kind_MAX];
        } res;

        struct {
            // struct kinc_a1_channel *channel[seuck_menu_action_kind_MAX];
            struct kinc_a1_sound *sound[seuck_menu_action_kind_MAX];
        } sfx;

        struct {
            uint8_t selected_player_for_craft_choice;
        } tmp;
    } menu;

    struct {
        enum px_state_setup     setup;
        enum px_game_phase      phase;
        uint16_t                intro_phase_tween[SEUCK_MAX_PLAYERS];

        struct {
            struct {
                oxi_id level_id;
                oxi_id tileset;
                oxi_id sprites;
                oxi_id explosion;
                oxi_id bullets;
            } files;

            struct {
                struct oxi_texture *tileset;
                struct oxi_texture *sprites;
                struct oxi_texture *explosion;
                struct oxi_texture *bullets;
            } textures;

            // struct oxi_material *material;
        } res;

        struct {
            struct seuck_bmfont_string loadout;
        } hud;
    } game;

    struct kinc_a1_sound **sfx;

    struct {
        struct {
            oxi_id data_file_id;
            oxi_id player;
            oxi_id font1_bin;
            oxi_id font1_img;
            oxi_id font2_bin;
            oxi_id font2_img;
        } files;

        struct {
            struct oxi_texture *player;
            struct oxi_texture *font1;
            struct oxi_texture *font2;
        } textures;

        struct {
            struct bmfont_file *font1;
            struct bmfont_file *font2;
        } fonts;

        oxi_id *sound_ids;
        uint16_t num_sounds;
    } res;

    bool                    ship_controls_enabled;
    enum px_state           state;
};

void px_init( struct px_context *px, struct px_init_args *args );
void px_update( struct px_context *px );
void px_post_update( struct px_context *px );
void px_draw( struct px_context *px );

void pxi_static_init( struct px_context *px );

void pxi_intro_init( struct px_context *px );
void pxi_intro_update( struct px_context *px );
void pxi_intro_draw( struct px_context *px );

void pxi_menu_init( struct px_context *px );
void pxi_menu_update( struct px_context *px );
void pxi_menu_draw( struct px_context *px );

void pxi_game_init( struct px_context *px );
void pxi_game_update( struct px_context *px );
void pxi_game_draw( struct px_context *px );

#if defined(__cplusplus)
}
#endif
