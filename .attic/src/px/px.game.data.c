#include "px.game.data.h"

// THIS FILE IS GENERATED (WELL, WILL BE IN THE FUTURE. DO NOT EDIT BY HAND!)

enum sheet_kind {
    SHEET_KIND_BLOBS,
    SHEET_KIND_IMAGES,
    SHEET_KIND_SOUNDS,
    SHEET_KIND_LEVELS,
    SHEET_KIND_REGIONS,
    SHEET_KIND_FRAMES,
    SHEET_KIND_ANIMATIONS,
    SHEET_KIND_BULLETS,
    SHEET_KIND_BULLET_SPAWNS,
    SHEET_KIND_WEAPON_LEVEL_SHOTS,
    SHEET_KIND_LOADOUTS,
    SHEET_KIND_SHIPS,
    SHEET_KIND_ENEMIES,
    SHEET_KIND_SPAWNPOINTS,
    SHEET_KIND_WAYPOINTS,
    SHEET_KIND_PATHS,
    SHEET_KIND_WAVESPAWNS,
    SHEET_KIND_WAVES,

    MAX_SHEET_KIND,
};

// TODO (DK) these should be saved in the data file ideally?
static size_t offsets[MAX_SHEET_KIND] = { 0 };

struct px_data {
    uint8_t major, minor, patch, padding1;
    uint16_t num[MAX_SHEET_KIND];
    uint16_t size[MAX_SHEET_KIND];
    uint8_t mem[];
};

static size_t skip_sheet( struct px_data *pxd, enum sheet_kind kind ) {
    size_t s = pxd->size[kind];
    size_t n = pxd->num[kind];
    return n * s;
}

static size_t skip_until( struct px_data *pxd, enum sheet_kind kind ) {
    size_t bytes = 0;

    for (int i = 0; i < kind; ++i) {
        bytes += skip_sheet(pxd, i);
    }

    return bytes;
}

// TODO (DK) remove me and write this into the datafile as well
void pxd_init( struct px_data *pxd ) {
    for (int i = 0; i < MAX_SHEET_KIND; ++i) {
        size_t o = skip_until(pxd, i);
        offsets[i] = o;
    }
}

uint16_t pxd_num_blobs( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_BLOBS];
}

char *pxd_blob_uri( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_BLOBS] + (pxd->size[SHEET_KIND_BLOBS] * idx);
    char *uri = (char *)pxd->mem + o;
    return uri;
}

size_t pxd_blob_uri_len( struct px_data *pxd, uint16_t idx ) {
    char *uri = pxd_blob_uri(pxd, idx);
    size_t l = strlen(uri);
    return l;
}

uint16_t pxd_num_images( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_IMAGES];
}

char *pxd_image_uri( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_IMAGES] + (pxd->size[SHEET_KIND_IMAGES] * idx);
    char *uri = (char *)pxd->mem + o;
    return uri;
}

size_t pxd_image_uri_len( struct px_data *pxd, uint16_t idx ) {
    char *uri = pxd_image_uri(pxd, idx);
    size_t l = strlen(uri);
    return l;
}

uint16_t pxd_num_sounds( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_SOUNDS];
}

char *pxd_sound_uri( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_SOUNDS] + (pxd->size[SHEET_KIND_SOUNDS] * idx);
    char *uri = (char *)pxd->mem + o;
    return uri;
}

size_t pxd_sound_uri_len( struct px_data *pxd, uint16_t idx ) {
    char *uri = pxd_sound_uri(pxd, idx);
    size_t l = strlen(uri);
    return l;
}

uint16_t pxd_num_atlasregions( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_REGIONS];
}

struct seuck_region_data * pxd_region_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_REGIONS] + (pxd->size[SHEET_KIND_REGIONS] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_levels( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_LEVELS];
}

struct px_level_data * pxd_level_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_LEVELS] + (pxd->size[SHEET_KIND_LEVELS] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_loadouts( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_LOADOUTS];
}

struct px_loadout *pxd_loadout_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_LOADOUTS] + (pxd->size[SHEET_KIND_LOADOUTS] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_bullets( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_BULLETS];
}

struct seuck_bullet_data * pxd_bullet_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_BULLETS] + (pxd->size[SHEET_KIND_BULLETS] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_bulletspawns( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_BULLET_SPAWNS];
}

struct seuck_bulletspawn_data * pxd_bulletspawn_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_BULLET_SPAWNS] + (pxd->size[SHEET_KIND_BULLET_SPAWNS] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_weapon_level_shots( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_WEAPON_LEVEL_SHOTS];
}

struct px_weapon_level_shots *pxd_weapon_level_shot_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_WEAPON_LEVEL_SHOTS] + (pxd->size[SHEET_KIND_WEAPON_LEVEL_SHOTS] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_frames( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_FRAMES];
}

struct seuck_spritesheet_frame * pxd_frame_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_FRAMES] + (pxd->size[SHEET_KIND_FRAMES] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_animations( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_ANIMATIONS];
}

struct seuck_spritesheet_animation * pxd_animation_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_ANIMATIONS] + (pxd->size[SHEET_KIND_ANIMATIONS] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_ships( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_SHIPS];
}

struct px_ship_data *pxd_ship_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_SHIPS] + (pxd->size[SHEET_KIND_SHIPS] * idx);
    return (void*)(pxd->mem + o);
}

uint16_t pxd_num_enemies( struct px_data *pxd ) {
    return pxd->num[SHEET_KIND_ENEMIES];
}

struct seuck_enemy_data_v2 * pxd_enemy_get( struct px_data *pxd, uint16_t idx ) {
    size_t o = offsets[SHEET_KIND_ENEMIES] + (pxd->size[SHEET_KIND_ENEMIES] * idx);
    return (void*)(pxd->mem + o);
}
