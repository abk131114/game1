#pragma once

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

void blob_combo( struct app_context *app, const char *label, uint8_t *current_item_idx );
void image_combo( struct app_context *app, const char *label, uint8_t *current_item_idx );
void sound_combo( struct app_context *app, const char *label, uint8_t *current_item_idx );

void region_combo( struct app_context *app, const char *label, uint16_t *current_item_idx );
void frame_combo( struct app_context *app, const char *label, uint16_t *current_item_idx );
void anim_combo( struct app_context *app, const char *label, uint16_t *current_item_idx );
void bullet_combo( struct app_context *app, const char *label, uint16_t *current_item_idx );
void bulletspawn_combo( struct app_context *app, const char *label, uint16_t *current_item_idx );
void loadout_combo( struct app_context *app, const char *label, uint8_t *current_item_idx );
void enemy_combo( struct app_context *app, const char *label, uint16_t *current_item_idx );
void spawnpoint_combo( struct app_context *app, const char *label, uint8_t *current_item_idx );
void waypoint_combo( struct app_context *app, const char *label, uint16_t *current_item_idx );
void path_combo( struct app_context *app, const char *label, uint8_t *current_item_idx );
void wavespawn_combo( struct app_context *app, const char *label, uint16_t *current_item_idx );

void bulletkind_combo( struct app_context *app, const char *label, uint8_t *current_item_idx );
void powerup_combo( struct app_context *app, const char *label, uint8_t *current_item_idx );

void draw_anim( struct app_context *app, uint16_t anim_id );
void draw_region( struct app_context *app, uint16_t region_id );

#if defined(__cplusplus)
}
#endif
