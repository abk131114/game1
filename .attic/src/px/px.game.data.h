#pragma once

#include <seuck/seuck.h>

#include <stdint.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

// TODO (DK) reorder so we can use it for indexing into WLS data?
enum px_powerup_kind {
    PX_POWERUP_KIND_SPEED,
    PX_POWERUP_KIND_GUNS,
    PX_POWERUP_KIND_BUILDER,
    PX_POWERUP_KIND_SIDE,
    PX_POWERUP_KIND_MISSILE,
    PX_POWERUP_KIND_PLASMA,
    PX_POWERUP_KIND_MAGMA,
    PX_POWERUP_KIND_LAZER,
    PX_POWERUP_KIND_STEALTH,

    MAX_PX_POWERUP_KIND,
};

struct px_loadout {
    uint8_t level[MAX_PX_POWERUP_KIND];
};

struct px_ship_data {
    uint8_t initial_loadout_ref;
    uint8_t maximum_loadout_ref;
    uint8_t faction; // TODO (DK) for collision?
    uint16_t idle_anim_ref, flyup_anim_ref, flydown_anim_ref; // TODO (DK) keep it here or another struct?
    // TODO (DK) explode, stealth
};

struct px_level_data {
    uint8_t level_ref, tileset_ref, sprites_ref;
};

#define PX_WLS_MAKE_KEY(Weapon, Level) (uint8_t)((0x0f & Weapon) << 4) | (0x0f & Level)
#define PX_WLS_GET_WEAPON(Key) (uint8_t)(Key >> 4) & 0x0f
#define PX_WLS_GET_LEVEL(Key) (Key & 0x0f)

struct px_weapon_level_shots_sound_ref {
    uint8_t enabled : 1;
    uint8_t ref : 7;
};

// TODO (DK) could be generic?
struct px_weapon_level_shots {
    // struct {
    //     uint8_t weapon: 4;
    //     uint8_t level: 4;
    // } key;

    uint8_t key;
    uint16_t start_ref, end_ref;
    uint8_t cooldown; // keep it here or in a separate weapons object?
    struct px_weapon_level_shots_sound_ref sound;
};

struct px_data;

void                        pxd_init( struct px_data *px );

uint16_t                    pxd_num_images( struct px_data *px );
char *                      pxd_image_uri( struct px_data *px, uint16_t idx );
size_t                      pxd_image_uri_len( struct px_data *px, uint16_t idx );

uint16_t                    pxd_num_blobs( struct px_data *px );
char *                      pxd_blob_uri( struct px_data *px, uint16_t idx );
size_t                      pxd_blob_uri_len( struct px_data *px, uint16_t idx );

uint16_t                    pxd_num_sounds( struct px_data *px );
char *                      pxd_sound_uri( struct px_data *px, uint16_t idx );
size_t                      pxd_sound_uri_len( struct px_data *px, uint16_t idx );

uint16_t                    pxd_num_regions( struct px_data *px );
struct seuck_region_data *  pxd_region_get( struct px_data *px, uint16_t idx );

uint16_t                    pxd_num_levels( struct px_data *px );
struct px_level_data *      pxd_level_get( struct px_data *px, uint16_t idx );

uint16_t                    pxd_num_loadouts( struct px_data *px );
struct px_loadout *         pxd_loadout_get( struct px_data *px, uint16_t idx );

uint16_t                    pxd_num_bullets( struct px_data *px );
struct seuck_bullet_data *  pxd_bullet_get( struct px_data *px, uint16_t idx );

uint16_t                        pxd_num_bulletspawns( struct px_data *px );
struct seuck_bulletspawn_data * pxd_bulletspawn_get( struct px_data *px, uint16_t idx );

uint16_t                        pxd_num_weapon_level_shots( struct px_data *px );
struct px_weapon_level_shots *  pxd_weapon_level_shot_get( struct px_data *px, uint16_t idx );

uint16_t                                pxd_num_frames( struct px_data *px );
struct seuck_spritesheet_frame *        pxd_frame_get( struct px_data *px, uint16_t idx );

uint16_t                                pxd_num_animations( struct px_data *px );
struct seuck_spritesheet_animation *    pxd_animation_get( struct px_data *px, uint16_t idx );

uint16_t                    pxd_num_ships( struct px_data *px );
struct px_ship_data *       pxd_ship_get( struct px_data *px, uint16_t idx );

uint16_t                    pxd_num_enemies( struct px_data *px );
struct seuck_enemy_data_v2 *pxd_enemy_get( struct px_data *px, uint16_t idx );

#if defined(__cplusplus)
}
#endif
