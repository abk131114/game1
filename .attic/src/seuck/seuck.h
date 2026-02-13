#pragma once

#include <bmfont/bmfont.h>
#include <tupni/tupni.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define SEUCK_MAX_RESOURCE_STR_LENGTH 64 /* TODO (DK) remove me here? */
#define SEUCK_MENU_CACHE_SIZE 32
#define SEUCK_MENU_MAX_STRINGS 128
#define SEUCK_MENU_MAX_STR 64

// #define SEUCK_MAX_INSTANCES_PER_WAVE 64
#define SEUCK_MAX_PLAYERS 2
#define SEUCK_MAX_ENEMIES 64
#define SEUCK_MAX_FACTIONS 2
#define SEUCK_MAX_BULLETS 256
#define SEUCK_MAX_INPUT_ACTIONS 32
#define SEUCK_MAX_EXPLOSIONS 128

struct seuck_resource_str {
    char uri[SEUCK_MAX_RESOURCE_STR_LENGTH];
};

struct vec2f { // TODO (DK) remove me and use ints?
    float x, y;
};

// struct vec2u8 {
//     uint8_t x, y;
// };

// struct vec2u16 {
//     uint16_t x, y;
// };

struct seuck_layer_data {
    uint8_t tileset_columns, padding1;
    uint16_t tiles[];
};

struct seuck_level_data {
    uint16_t width, height;
    uint8_t num_layers, padding1;
    struct seuck_layer_data layer;//s[];
};

struct seuck_bullet_data {
    uint8_t damage;
    uint8_t kind; // how it's flying: path / homing / react to player
    uint8_t kind_data; // ?
    // disappear after impact
    // radius / collision mask / type?
        // uint8_t width; // in case it's different than the sprite
        // uint8_t height; // in case it's different than the sprite
    uint16_t anim_ref;
};

// struct mountpoint_data {
//     uint8_t offset_x;       // offset to ship
//     uint8_t offset_y;
//     uint8_t cooldown;
// };

struct seuck_bulletspawn_data {
    uint16_t bullet_ref;
    uint8_t flags;
    int8_t dir_x;
    int8_t dir_y;
    // add offsets? so we can build the laser from parts?
};

struct seuck_enemy_data {
    uint8_t faction;
    uint16_t anim_ref;
    uint16_t score;
};

struct seuck_enemy_data_v2 {
    uint16_t anim_ref;
    uint16_t score;
    uint16_t hp;
    // uint8_t hp;
    // radius / collision mask / type ?
    // fly path / pattern? or is this instance?
};

struct seuck_region_data {
    uint8_t atlas_ref;
    uint16_t x, y, w, h;    // TODO (DK) uint8_t and pow2?
};

struct seuck_spritesheet_frame {
    uint16_t region_ref;
    uint16_t duration;
};

struct seuck_spritesheet_animation {
    uint16_t frames_start_ref;
    uint16_t frames_end_ref;

    // 0xffff is unlimited repeat
    uint8_t repeat; // TODO (DK) uint8_t is enough, or lets be real, anything more than 1 or 2 repeats is useless?
};

struct seuck_spritesheet_animation_state {
    uint16_t frame;
    uint16_t frame_index;
    uint16_t elapsed;
    int16_t  repeat;
};

enum seuck_bullet_instance_flag {
    SEUCK_BULLET_INSTANCE_FLAG_ALIVE = 0,
};

struct seuck_bullet_instance {
    struct seuck_spritesheet_animation anim;                                    // 6
    struct seuck_spritesheet_animation_state anim_state;                        // 8
    struct seuck_bullet_data bullet;                                            // 5
    uint8_t flags, age;                                                         // 2
    int16_t x, y;
    uint8_t w, h;
    int8_t dx, dy; // includes speed                                            // 2

    // TODO (DK) remove me and use embedded anim/bullet fields?
    uint16_t bullet_ref;                                                        // 2
    uint16_t anim_ref; // keep a separate ref for faster lookup or bullet_ref?  // 2
};

struct seuck_enemy_instance {
    struct seuck_spritesheet_animation anim;                                    // 6
    struct seuck_spritesheet_animation_state anim_state;                        // 8
    uint16_t enemy_ref;                                                         // 2
    int16_t x, y;
    uint16_t w, h;                                                                      // TODO (DK) uint8_t and pow2
    uint16_t hp;                                                                // 2 // TODO (DK) or uint8_t
    int16_t age;                                                                // 1
    int8_t dx, dy; // includes speed                                            // 2
};

struct seuck_player_instance {
    struct seuck_spritesheet_animation anim;                                    // 6
    struct seuck_spritesheet_animation_state anim_state;                        // 8
    uint16_t ship_ref;                                                          // 2
    int16_t x, y;
    uint8_t w, h;
    int8_t dx, dy;                                                              // 2
};

// enum seuck_movement_kind {
//     seuck_movement_kind_path,
//     // seuck_movement_kind_ik, // for the worm thingy?
//     seuck_movement_kind_shooty_asteroid,

//     // bosses (and normal enemies) might target the player x/y position as well during their movement
//         // like the small rockets in project-x
// };

// struct seuck_waypoint {
//     uint16_t x, y;
// };

// struct seuck_path {
//     uint16_t first_waypoint_idx, last_waypoint_idx;
// };

// struct enemy_instance_data {
//     uint8_t enemy;
//     // spawnpoint?
//     // spawn delay?
// }

struct seuck_context;

enum seuck_load_status {
    SEUCK_LOAD_STATUS_IN_PROGRESS,
    SEUCK_LOAD_STATUS_SUCCESS,
    SEUCK_LOAD_STATUS_FAILURE,
};

enum seuck_explosion_instance_flag {
    SEUCK_EXPLOSION_INSTANCE_FLAG_ALIVE = 0,
};

struct seuck_point {
    int16_t x, y;
};

struct seuck_waypoint_data {
    struct seuck_point pos;
};

struct seuck_path_data {
    uint16_t from_ref, to_ref;
};

struct seuck_spawnpoint_data {
    struct seuck_point pos;
};

// struct seuck_spawnpoint_data {
//     uint16_t x, y; // TODO (DK) check the video
// };

// TODO (DK) we could also add time triggers?
//   for games like this "small-area topdown3d with the spider boss" i liked?
        // space tripper
struct seuck_wavespawn_data {
    uint16_t enemy_ref;
    uint8_t spawnpoint_ref;
    uint8_t path_ref;
    uint8_t delay; // or 16 bit?
};

struct seuck_wave_trigger {
    uint16_t offset_x, offset_y;
    uint16_t spawn_ref_from, spawn_ref_to;
};

// enum seuck_primary_direction {
//     SEUCK_PRIMARY_DIRECTION_H,
//     SEUCK_PRIMARY_DIRECTION_V,
// };

struct seuck_levelsetup_data {
    uint8_t initial_autoscroll_x, initial_autoscroll_y;
};

struct seuck_level_desc {
    // uint8_t primary_dir: 1; // enum seuck_primary_direction
    // uint8_t scroll_h : 1;
    // uint8_t scroll_v : 1;

    uint8_t autoscroll_x, autoscroll_y; // speed; can be disabled by wave
};

struct seuck_explosion_instance {
    struct seuck_spritesheet_animation anim;                                    // 6
    struct seuck_spritesheet_animation_state anim_state;                        // 8
    int16_t x, y;
    // uint16_t w, h;                                                           // TODO (DK) uint8_t and pow2
    // uint16_t hp;                                                             // 2 // TODO (DK) or uint8_t
    int16_t age;                                                                // 1
    uint8_t flags;
};

struct seuck_bmfont_string {
    struct bmfont_char *ch[SEUCK_MENU_MAX_STR];
    uint8_t len;
    struct oxi_texture *tex;
};

struct seuck_menu_str {
    char buf[SEUCK_MENU_MAX_STR];
};

struct seuck_menu_toggle_data {
    uint8_t a_text_ref, b_text_ref;
};

struct seuck_menu_submenu_data {
    uint8_t text_ref;
};

struct seuck_menu_label_data {
    uint8_t text_ref;
};

struct seuck_menu_action_data {
    uint8_t text_ref;
};

enum seuck_menu_item_kind {
    seuck_menu_item_kind_label,
    seuck_menu_item_kind_toggle,
    seuck_menu_item_kind_submenu,
    seuck_menu_item_kind_action,
    seuck_menu_item_kind_empty_separator,
};

struct seuck_menu_item_data {
    enum seuck_menu_item_kind kind;
    uint8_t item_index, userdata;
};

enum seuck_menu_action_kind {
    seuck_menu_action_kind_nav,
    seuck_menu_action_kind_activate, // TODO (DK) make a difference between toggles, buttons, etc?

    seuck_menu_action_kind_MAX,
};

struct seuck_menu_sound_data {
    uint8_t sound_idx;
};

struct seuck_menu_data {
    uint8_t from, to;
    int8_t default_selection;
};

struct seuck_menu_item {
    // struct string_cache_item text;
    // void (*fn)( uint8_t, void * );
    uint8_t text_idx;
    enum seuck_menu_item_kind kind;
    void *ctx;
    int selectable;
    uint8_t userdata;
};

struct seuck_menu_state {
    int8_t selected_idx;
    uint8_t fade_counter;
    uint8_t num_items;
};

struct seuck_init_args {
    struct {
        int width, height;
    } screen;
};

struct seuck_context {
    struct seuck_init_args cfg;
    struct seuck_level_data *       level_data; // TODO (DK) rename to level_data?

    uint8_t                         player_alive;
    struct seuck_player_instance    players[SEUCK_MAX_PLAYERS];
    struct vec2f                    player_speed[SEUCK_MAX_PLAYERS]; // uint8? move into player_instance?

    struct seuck_bullet_instance    bullets[SEUCK_MAX_FACTIONS][SEUCK_MAX_BULLETS];

    uint64_t                        enemy_instance_alive;
    struct seuck_enemy_instance     enemies[SEUCK_MAX_ENEMIES];

    struct seuck_explosion_instance explosions[SEUCK_MAX_EXPLOSIONS];

    uint32_t                        player_score[SEUCK_MAX_PLAYERS];

    // struct vec2f                    enemy_instance_position[SEUCK_MAX_INSTANCES_PER_WAVE]; // TODO (DK) remove me?

    uint8_t                         wave_instance_spawned_count;

    struct {
        struct {
            int32_t x, y;

            // struct {
            //     uint8_t x, y;
            // } track_bounds;

            // tracked dimension
        } camera;

        int8_t autoscroll_h, autoscroll_v; // TODO (DK) or give the ship some speed and track them?
    } stage;

    struct {
        // struct seuck_menu_cache cache;
        struct seuck_menu_item      items[SEUCK_MENU_CACHE_SIZE];
        struct seuck_bmfont_string  strings[SEUCK_MENU_MAX_STRINGS];
        struct seuck_menu_state     state;
    } menu;
};

struct seuck_menu_content {
    struct seuck_menu_str *strings;
    struct seuck_menu_toggle_data *toggles;
    struct seuck_menu_submenu_data *submenus;
    struct seuck_menu_label_data *labels;
    struct seuck_menu_action_data *actions;
    struct seuck_menu_item_data *menu_items;
    struct seuck_menu_sound_data *sounds;
    struct seuck_menu_data *menus;
};

void seuck_init( struct seuck_context *g, struct seuck_init_args *args );

void seuck_animation_advance( struct seuck_spritesheet_animation *anim, struct seuck_spritesheet_animation_state *state );
void seuck_autoscrollspeed_set( struct seuck_context *g, int8_t h, int8_t v );
void seuck_move_enemy_instances( struct seuck_context *g );
void seuck_move_bullets( struct seuck_context *g );
void seuck_update_camera( struct seuck_context *g );
void seuck_move_players( struct seuck_context *g );
void seuck_process_playerbullet_vs_enemy_collisions( struct seuck_context *g, void(*on_enemy_destroyed)( void *, int ), void *ctx );
void seuck_reset_player_speed( struct seuck_context *g );
void seuck_advance_animations( struct seuck_context *g );
void seuck_kill_enemy( struct seuck_context *g, int idx );
void seuck_update_explosions( struct seuck_context *g );

void        seuck_cache_text( struct bmfont_file *fnt, struct seuck_bmfont_string *cache, const char *str, struct oxi_texture *tex );
uint16_t    seuck_text_width( struct seuck_bmfont_string *item );
uint8_t     seuck_build_menu( struct seuck_context *g, struct bmfont_file *fnt, struct seuck_menu_content *content, int menu_idx, struct oxi_texture *tex );
void        seuck_menu_nav_up( struct seuck_context *g );
void        seuck_menu_nav_down( struct seuck_context *g );
void        seuck_menu_nav_left( struct seuck_context *g );
void        seuck_menu_nav_right( struct seuck_context *g );
void        seuck_menu_nav_activate( struct seuck_context *g );

#if defined(__cplusplus)
}
#endif
