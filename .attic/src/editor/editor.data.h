#pragma once

#if defined(PX_DEV_UI)

#include <seuck/seuck.h>
#include <px/px.game.data.h>

#include <stb_ds.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct gui_version_data {
    uint8_t major, minor, patch;
};

struct gui_blob_data {
	struct seuck_resource_str d;
    int id;
    bool selected;
};

struct gui_image_data {
	struct seuck_resource_str d;
    // w + h?
    int id;
    bool selected;
};

struct gui_sound_data {
	struct seuck_resource_str d;
    // compression and stuff?
    int id;
    bool selected;
};

struct gui_level_data {
    struct px_level_data d;
    int id;
    bool selected;
};

struct gui_region_data {
    struct seuck_region_data d;
    int id;
    bool selected;
};

struct gui_frame_data {
    struct seuck_spritesheet_frame d;
    int id;
    bool selected;
};

struct gui_anim_data {
    struct seuck_spritesheet_animation d;
    int id;
    bool selected;
};

struct gui_bullet_data {
    struct seuck_bullet_data d;
    int id;
    bool selected;
};

struct gui_bulletspawn_data {
    struct seuck_bulletspawn_data d;
    int id;
    bool selected;
};

struct gui_weaponlevelshot_data {
    struct px_weapon_level_shots d;
    int id;
    bool selected;
};

struct gui_loadout_data {
    struct px_loadout d;
    int id;
    bool selected;
};

struct gui_ship_data {
    struct px_ship_data d;
    int id;
    bool selected;
};

struct gui_waypoint_data {
    struct seuck_waypoint_data d;
    int id;
    bool selected;
};

struct gui_path_data {
    struct seuck_path_data d;
    int id;
    bool selected;
};

struct gui_enemy_data {
    struct seuck_enemy_data d;
    int id;
    bool selected;
};

struct gui_enemy_data_v2 {
    struct seuck_enemy_data_v2 d;
    int id;
    bool selected;
};

struct gui_spawnpoint_data {
    struct seuck_spawnpoint_data d;
    int id;
    bool selected;

    // meta
    char name[32];
};

struct gui_wavespawn_data {
    struct seuck_wavespawn_data d;
    int id;
    bool selected;

    // meta
    int level;
};

struct gui_wave_data {
    struct seuck_wave_trigger d;
    int id;
    bool selected;

    // meta
    // int level;
};

struct gui_data {
    struct gui_blob_data *blobs;
    struct gui_image_data *images;
    struct gui_sound_data *sounds;
    struct gui_level_data *levels;
    struct gui_region_data *regions;
    struct gui_frame_data *frames;
    struct gui_anim_data *anims;
    struct gui_bullet_data *bullets;
    struct gui_bulletspawn_data *bulletspawns;
    struct gui_weaponlevelshot_data *wls;
    struct gui_loadout_data *loadouts;
    struct gui_ship_data *ships;
    struct gui_enemy_data_v2 *enemies;
    struct gui_spawnpoint_data *spawnpoints;
    struct gui_waypoint_data *waypoints;
    struct gui_path_data *paths;
    struct gui_wavespawn_data *wavespawns;
    struct gui_wave_data *waves;
};

struct gui_data	*	            gui_data_load( const char *uri );
void				            gui_data_save( const char *uri, struct gui_data *data );
void				            gui_data_export( const char *uri, struct gui_data *data );
void				            gui_data_reindex( struct gui_data *data );

uint16_t                        gui_blob_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_image_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_sound_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_region_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_frame_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_anim_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_bullet_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_bulletspawn_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_loadout_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_enemy_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_spawnpoint_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_waypoint_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_path_find_index( struct gui_data *data, uint16_t id );
uint16_t                        gui_wavespawn_find_index( struct gui_data *data, uint16_t id );

struct gui_blob_data *          gui_blob_find( struct gui_data *data, uint16_t id );
struct gui_image_data *         gui_image_find( struct gui_data *data, uint16_t id );
struct gui_sound_data *         gui_sound_find( struct gui_data *data, uint16_t id );
struct gui_region_data *        gui_region_find( struct gui_data *data, uint16_t id );
struct gui_frame_data *         gui_frame_find( struct gui_data *data, uint16_t id );
struct gui_anim_data *          gui_anim_find( struct gui_data *data, uint16_t id );
struct gui_bullet_data *        gui_bullet_find( struct gui_data *data, uint16_t id );
struct gui_bulletspawn_data *   gui_bulletspawn_find( struct gui_data *data, uint16_t id );
struct gui_loadout_data *       gui_loadout_find( struct gui_data *data, uint16_t id );
struct gui_enemy_data_v2 *      gui_enemy_find( struct gui_data *data, uint16_t id );
struct gui_spawnpoint_data *    gui_spawnpoint_find( struct gui_data *data, uint16_t id );
struct gui_waypoint_data *      gui_waypoint_find( struct gui_data *data, uint16_t id );
struct gui_path_data *          gui_path_find( struct gui_data *data, uint16_t id );
struct gui_wavespawn_data *     gui_wavespawn_find( struct gui_data *data, uint16_t id );

#define gui_selected_items_delete(Array) \
    int i = arrlen(Array) - 1; \
    \
    while (i >= 0) { \
        if (Array[i].selected) { \
            arrdel(Array, i); \
        } \
        \
        i -= 1; \
    }

#if defined(__cplusplus)
}
#endif

#endif
