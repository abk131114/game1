#include "px.core.h"

#include <bmfont/bmfont.h>
#include <bmfont/bmfont-oxi-painter.h>
#include <oxi/oxi.spritebatcher.h>
#include <oxi/oxi.texture-system.h>

#include <math.h>

int TILE_SIZE = 16;
float TILE_SIZEf = 16.0f;

static void draw_map( struct px_context *px ) {
    struct oxi_spritebatcher *b = px->batcher;
    uint16_t start_tile_x = px->seuck.stage.camera.x / TILE_SIZE;
    uint16_t start_tile_y = px->seuck.stage.camera.y / TILE_SIZE;
    uint16_t tiles_w = px->seuck.cfg.screen.width / TILE_SIZE + 1;
    uint16_t tiles_h = px->seuck.cfg.screen.height / TILE_SIZE + 1;
    int tex_width = oxi_texture_width(px->game.res.textures.tileset);
    int tex_height = oxi_texture_height(px->game.res.textures.tileset);
    float sw = TILE_SIZEf / (float)tex_width;
    float sh = TILE_SIZEf / (float)tex_height;

    for (uint16_t row = 0; row < tiles_h; ++row) {
        for (uint16_t col = 0; col < tiles_w; ++col) {
            int idx = (start_tile_y + row) * px->seuck.level_data->width + (start_tile_x + col);
            uint16_t tile = px->seuck.level_data->layer.tiles[idx];
            uint8_t tile_u = tile % px->seuck.level_data->layer.tileset_columns;
            uint8_t tile_v = tile / px->seuck.level_data->layer.tileset_columns;
            float sx = tile_u * sw;
            float sy = tile_v * sh;
            float x = floorf((col * TILE_SIZEf) - px->seuck.stage.camera.x % TILE_SIZE);
            float y = row * TILE_SIZEf; // TODO (DK) scroll y as well
            oxi_spritebatcher_add_uv_tint(b, px->game.res.textures.tileset, sx, sy, sw, sh, x, y, -5.0f, TILE_SIZEf, TILE_SIZEf, oxi_rgba(0xffffffff));
        }
    }
}

static void draw_stars( struct px_context *px ) {
}

static void draw_animation(
    struct px_context *px,
    int pos_x, int pos_y,
    struct seuck_spritesheet_animation *anim,
    struct seuck_spritesheet_animation_state *state,
    struct oxi_texture *texture
) {
    struct seuck_spritesheet_frame *frm = pxd_frame_get(px->data, state->frame);
    struct seuck_region_data *region = pxd_region_get(px->data, frm->region_ref);
    uint16_t sx = region->x;
    uint16_t sy = region->y;
    uint16_t sw = region->w;
    uint16_t sh = region->h;
    float x = pos_x - sw / 2;
    float y = pos_y - sh / 2;
    float w = sw;
    float h = sh;
    oxi_spritebatcher_add_px_tint(px->batcher, texture, sx, sy, sw, sh, x, y, -5.0f, w, h, oxi_rgba(0xffffffff));
}

static void draw_enemy_instances( struct px_context *px ) {
    uint64_t alive = px->seuck.enemy_instance_alive;
    int32_t cx = px->seuck.stage.camera.x, cy = px->seuck.stage.camera.y;

    for (int i = 0; i < SEUCK_MAX_ENEMIES; ++i) {
        if (!oxi_bit_has(alive, i)) {
            continue;
        }

        // struct vec2f position = g->enemy_instance_position[i];

        struct seuck_enemy_instance *e = &px->seuck.enemies[i];
        // TODO (DK) lookup texture, or better save them in the bullet instance or anim
        // TODO (DK) add cx/cy here?
        draw_animation(px, e->x - cx, e->y - cy, &e->anim, &e->anim_state, px->game.res.textures.sprites);
    }
}

static void draw_player( struct px_context *px, int idx ) {
    struct seuck_player_instance *p = &px->seuck.players[idx];
    int32_t cx = px->seuck.stage.camera.x, cy = px->seuck.stage.camera.y;
    // TODO (DK) lookup texture, or better save them in the bullet instance or anim
    // TODO (DK) add cx/cy here?
    draw_animation(px, p->x - cx, p->y - cy, &p->anim, &p->anim_state, px->res.textures.player);
}

static void draw_player_bullets( struct px_context *px ) {
    int32_t cx = px->seuck.stage.camera.x, cy = px->seuck.stage.camera.y;

    for (int i = 0; i < SEUCK_MAX_BULLETS; ++i) {
        struct seuck_bullet_instance *b = &px->seuck.bullets[0][i];

        if (!oxi_bit_has(b->flags, SEUCK_BULLET_INSTANCE_FLAG_ALIVE)) {
            continue;
        }

        // TODO (DK) lookup texture, or better save them in the bullet instance or anim
        draw_animation(px, b->x - cx, b->y - cy, &b->anim, &b->anim_state, px->game.res.textures.bullets);
    }
}

static void draw_explosions( struct px_context *px ) {
    int32_t cx = px->seuck.stage.camera.x, cy = px->seuck.stage.camera.y;

    for (int i = 0; i < SEUCK_MAX_EXPLOSIONS; ++i) {
        struct seuck_explosion_instance *inst = &px->seuck.explosions[i];

        if (!oxi_bit_has(inst->flags, SEUCK_EXPLOSION_INSTANCE_FLAG_ALIVE)) {
            continue;
        }

        // TODO (DK) lookup texture, or better save them in the bullet instance or anim
        draw_animation(px, inst->x - cx, inst->y - cy, &inst->anim, &inst->anim_state, px->game.res.textures.explosion);
    }
}

static void draw_enemy_bullets( struct px_context *px ) {
    int32_t cx = px->seuck.stage.camera.x, cy = px->seuck.stage.camera.y;

    for (int i = 0; i < SEUCK_MAX_BULLETS; ++i) {
        struct seuck_bullet_instance *b = &px->seuck.bullets[1][i];

        if (!oxi_bit_has(b->flags, SEUCK_BULLET_INSTANCE_FLAG_ALIVE)) {
            continue;
        }

        // TODO (DK) lookup texture, or better save them in the bullet instance or anim
        draw_animation(px, b->x - cx, b->y - cy, &b->anim, &b->anim_state, px->game.res.textures.bullets);
    }
}

static void draw_ui( struct px_context *px ) {
    int x = 4;
    int y = 185;
    int is_selected_gun = 0;
    struct oxi_color color = is_selected_gun == 1
        ? oxi_rgba_c(0xff, 0xff, 0xff, 0xff)
        : oxi_rgba_c(0xff, 0xff, 0xff, 0xff);

    bmfont_draw_chars_tinted(px->batcher, px->game.hud.loadout.tex, px->game.hud.loadout.ch, px->game.hud.loadout.len, x, y, color.impl.rgba);
}

void pxi_game_draw( struct px_context *px ) {
    struct oxi_spritebatcher *b = px->batcher;

    oxi_spritebatcher_begin(b);

    switch (px->game.setup) {
        case PX_STATE_SETUP_LOAD: {
        } break;

        case PX_STATE_SETUP_FINISHED: {
            draw_map(px);
            draw_stars(px);

            draw_explosions(px);
            draw_player_bullets(px);
            draw_enemy_bullets(px);

            draw_enemy_instances(px);

            for (int i = 0; i < SEUCK_MAX_PLAYERS; ++i) {
                if (!oxi_bit_has(px->seuck.player_alive, i)) {
                    continue;
                }

                draw_player(px, i);
            }

            draw_ui(px);
        } break;
    }

    oxi_spritebatcher_end(b);
}
