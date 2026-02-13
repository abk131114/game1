#include "px.core.h"

#include <bmfont/bmfont.h>
#include <oxi/oxi.io.h>
#include <oxi/oxi.spritebatcher.h>
#include <oxi/oxi.texture-system.h>
#include <kinc/audio1/audio.h> // TODO (DK) wrap in oxi?

#define EXPLOSION_ANIMATION_SHEET_ROW 5 // TODO (DK) don't hardcode here
#define EXPLOSION_SOUND_ROW 8 // TODO (DK) don't hardcode here
#define ACTIONMAP 1

static int find_first_free_bullet_index( struct seuck_context *ctx, int faction ) {
    for (int i = 0; i < SEUCK_MAX_BULLETS; ++i) {
        if (!oxi_bit_has(ctx->bullets[faction][i].flags, SEUCK_BULLET_INSTANCE_FLAG_ALIVE)) {
            return i;
        }
    }

    return SEUCK_MAX_BULLETS;
}

static void fire_gun( struct px_context *px, uint8_t player_idx, enum px_powerup_kind gun ) {
    uint8_t level = px->player_items[player_idx].level[gun];

    if (level == 0) {
        return;
    }

    uint8_t key = PX_WLS_MAKE_KEY(gun, level);

    // TODO (DK) build a lookup table instead of a loop?
    for (uint16_t i = 0; i < pxd_num_weapon_level_shots(px->data); ++i) {
        struct px_weapon_level_shots *shot = pxd_weapon_level_shot_get(px->data, i);

        if (shot->key != key) {
            continue;
        }

        if (px->player_item_cooldown->level[gun] > 0) {
            return;
        }

        px->player_item_cooldown->level[gun] = shot->cooldown;

        for (uint16_t j = shot->start_ref; j <= shot->end_ref; ++j) {
            struct seuck_bulletspawn_data *bs = pxd_bulletspawn_get(px->data, j);
            struct seuck_bullet_data *b = pxd_bullet_get(px->data, bs->bullet_ref);

            int faction = 0;
            int bidx = find_first_free_bullet_index(&px->seuck, faction);

            if (bidx == SEUCK_MAX_BULLETS) {
                oxi_warn("px.core.fire_gun: bullet array is already full");
                return;
            }

// TODO (DK) make an utility function ... bullet, anims, ...
            struct seuck_bullet_instance *inst = &px->seuck.bullets[faction][bidx];
            inst->dx = bs->dir_x;
            inst->dy = bs->dir_y;
            inst->bullet_ref = bs->bullet_ref;
            inst->x = px->seuck.players[player_idx].x;
            inst->y = px->seuck.players[player_idx].y;
            inst->anim_ref = b->anim_ref;
            inst->flags = oxi_bit_set(bs->flags, SEUCK_BULLET_INSTANCE_FLAG_ALIVE);
            inst->anim = *pxd_animation_get(px->data, b->anim_ref);
            inst->bullet = *pxd_bullet_get(px->data, bs->bullet_ref);
            inst->anim_state.elapsed = 0;
            inst->anim_state.frame_index = 0;
            inst->anim_state.frame = inst->anim.frames_start_ref;
            inst->anim_state.repeat = 0;
            inst->age = 0;

            struct seuck_spritesheet_frame *frame = pxd_frame_get(px->data, inst->anim_state.frame);
            struct seuck_region_data *region = pxd_region_get(px->data, frame->region_ref);
            inst->w = region->w;
            inst->h = region->h;

            // seuck_bullet_spawn();
        }

        if (shot->sound.enabled) {
            // get sound by ref
            // play it

            kinc_a1_play_sound(px->sfx[shot->sound.ref], false, 1.0f, false);
        }
    }
}

static void fire( struct px_context *px, uint8_t player_idx ) {
    // speed, guns, builder, side, missile, plasma, magma, lazer, stealth

    fire_gun(px, player_idx, px->player_selected_weapon[player_idx]);
    fire_gun(px, player_idx, PX_POWERUP_KIND_SIDE);
    fire_gun(px, player_idx, PX_POWERUP_KIND_MISSILE);
}

static void activate_powerup( struct px_context *px, uint8_t idx ) {
    // int powerup_idx = px->player_powerup[player_idx];

    // if (powerup_idx > 0) {
    //     uint8_t *powerup = (uint8_t *)(&ctx->game.ctx.player_items[player_idx] + (powerup_idx - 1));
    //     uint8_t *max = (uint8_t *)(&ctx->game.ctx.ships->data[ctx->game.ctx.setup.craft[player_idx]].maximum + (powerup_idx - 1));

    //     if (*powerup < *max) {
    //         *powerup += 1;
    //         ctx->game.ctx.player_powerup[player_idx] = 0;
    //     }
    // }
}

static void move( float h, float v, void *ctx, int idx ) {
    struct px_context *px = ctx;
    px->seuck.player_speed[idx].x += h * px->player_items[idx].level[PX_POWERUP_KIND_SPEED];
    px->seuck.player_speed[idx].y += v * px->player_items[idx].level[PX_POWERUP_KIND_SPEED];
}

static void move1( float h, float v, void *ctx ) {
    move(h, v, ctx, 0);
}

static void fire1( void *ctx ) {
    fire(ctx, 0);
}

static void use1( void *ctx ) {
    activate_powerup(ctx, 0);
}

static void move2( float h, float v, void *ctx ) {
    move(h, v, ctx, 1);
}

static void fire2( void *ctx ) {
    fire(ctx, 1);
}

static void use2( void *ctx ) {
    activate_powerup(ctx, 1);
}

void pxi_game_static_init( struct px_context *px ) {
    struct tupni_context *t = &px->tupni;

    enum pxi_gameplay_input_action action;

    action = pxi_gameplay_input_action_move_1;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_value);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_vector2d);
    tupni_action_bind_composite_vector2d(t, ACTIONMAP, action, 0, 0, 1, 2, 3, tupni_vector2d_mode_digital_normalized);
    tupni_action_set_vector2d_trigger_callback(t, ACTIONMAP, action, move1, px);
    tupni_action_enable(t, ACTIONMAP, action);

    action = pxi_gameplay_input_action_fire_1;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_value);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 5);
    tupni_action_set_button_trigger_callback(t, ACTIONMAP, action, fire1, px);
    tupni_action_enable(t, ACTIONMAP, action);

    action = pxi_gameplay_input_action_use_1;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_button);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 4);
    tupni_action_set_button_start_callback(t, ACTIONMAP, action, use1, px);
    tupni_action_enable(t, ACTIONMAP, action);

// TODO (DK) have _2 enum, or find a better way?

    action = pxi_gameplay_input_action_move_2;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_value);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_vector2d);
    tupni_action_bind_composite_vector2d(t, ACTIONMAP, action, 0, 0, 1, 2, 3, tupni_vector2d_mode_digital_normalized); // TODO (DK)
    tupni_action_set_vector2d_trigger_callback(t, ACTIONMAP, action, move2, px);
    tupni_action_enable(t, ACTIONMAP, action);

    action = pxi_gameplay_input_action_fire_2;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_value);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 5); // TODO (DK)
    tupni_action_set_button_trigger_callback(t, ACTIONMAP, action, fire2, px);
    tupni_action_enable(t, ACTIONMAP, action);

    action = pxi_gameplay_input_action_use_2;
    tupni_action_set_action_kind(t, ACTIONMAP, action, tupni_action_kind_button);
    tupni_action_set_control_kind(t, ACTIONMAP, action, tupni_control_kind_button);
    tupni_action_bind_button(t, ACTIONMAP, action, 0, 4); // TODO (DK)
    tupni_action_set_button_start_callback(t, ACTIONMAP, action, use2, px);
    tupni_action_enable(t, ACTIONMAP, action);
}

void pxi_game_init( struct px_context *px ) {
    px->game.setup = PX_STATE_SETUP_LOAD;

    struct px_data *pxd = px->data;
    uint8_t level_idx = px->options.start_level_index;
    uint8_t num_levels = pxd_num_levels(pxd);

    if (level_idx >= num_levels) {
        oxi_err("[px.core.px_level_setup] target level index '%i' is out of bounds '%i'", level_idx, num_levels);
        return;
    }

    struct oxi_io_system *sys = px->oxi.sys.io;
    struct px_level_data *res = pxd_level_get(pxd, level_idx);

    px->game.res.files.level_id = oxi_io_load_blob(sys, pxd_blob_uri(pxd, res->level_ref), pxd_blob_uri_len(pxd, res->level_ref));
    px->game.res.files.tileset = oxi_io_load_image(sys, pxd_image_uri(pxd, res->tileset_ref), pxd_image_uri_len(pxd, res->tileset_ref));
    px->game.res.files.sprites = oxi_io_load_image(sys, pxd_image_uri(pxd, res->sprites_ref), pxd_image_uri_len(pxd, res->sprites_ref));
    // TODO (DK) "why" do we know to load these, put into db?
    px->game.res.files.explosion = oxi_io_load_image(sys, pxd_image_uri(pxd, 0), pxd_image_uri_len(pxd, 0));
    px->game.res.files.bullets = oxi_io_load_image(sys, pxd_image_uri(pxd, 1), pxd_image_uri_len(pxd, 1));

    px->res.num_sounds = pxd_num_sounds(px->data);
    px->res.sound_ids = oxi_alloc(px->res.num_sounds * sizeof(oxi_id), NULL);

    for (int i = 0; i < px->res.num_sounds; ++i) {
        px->res.sound_ids[i] = oxi_io_load_sound(sys, pxd_sound_uri(pxd, i), pxd_sound_uri_len(pxd, i));
    }

    // TODO (DK) load fonts

    tupni_actionmap_set_active_index(&px->tupni, ACTIONMAP);
}

static void init_player( struct px_context *px, int idx ) {
    struct px_ship_data *ship = pxd_ship_get(px->data, px->options.craft[idx]);
    px->player_items[idx] = *pxd_loadout_get(px->data, ship->initial_loadout_ref);
    px->player_selected_weapon[idx] = PX_POWERUP_KIND_GUNS;
    px->seuck.player_alive = oxi_bit_set(px->seuck.player_alive, idx);

    struct seuck_player_instance *p = &px->seuck.players[idx];
    struct seuck_spritesheet_animation *anm = pxd_animation_get(px->data, ship->idle_anim_ref);
    p->ship_ref = px->options.craft[idx];
    p->anim = *anm;
    p->anim_state.elapsed = 0;
    p->anim_state.frame_index = 0;
    p->anim_state.frame = p->anim.frames_start_ref;
    p->anim_state.repeat = 0;
}

static void toggle_shipcontrols( struct px_context *px, bool enabled ) {
    px->ship_controls_enabled = enabled;
}

static void set_game_phase( struct px_context *px, enum px_game_phase phase ) {
    px->game.phase = phase;
}

static void init_level( struct px_context *px ) {
    init_player(px, 0);
    init_player(px, 1);

    if (px->options.num_players == 1) {
        px->seuck.player_alive = oxi_bit_clear(px->seuck.player_alive, 1);
    }

    // TODO (DK) they should fly in from the left
    px->seuck.players[0].x = 80;
    px->seuck.players[0].y = 75;
    px->seuck.players[1].x = 64;
    px->seuck.players[1].y = 115;

    for (int i = 0; i < SEUCK_MAX_ENEMIES; ++i) {
        struct seuck_enemy_instance *e = &px->seuck.enemies[i];
        struct seuck_enemy_data_v2 *d = pxd_enemy_get(px->data, 0);
        e->anim = *pxd_animation_get(px->data, d->anim_ref);
        e->anim_state.elapsed = 0;
        e->anim_state.frame_index = 0;
        e->anim_state.frame = e->anim.frames_start_ref;
        e->anim_state.repeat = 0;
        e->x = oxi_random_range_inc_i(0, 320);
        e->y = oxi_random_range_inc_i(0, 200);

        struct seuck_spritesheet_frame *frame = pxd_frame_get(px->data, e->anim_state.frame);
        struct seuck_region_data *region = pxd_region_get(px->data, frame->region_ref);
        e->w = region->w;
        e->h = region->h;
    }

    toggle_shipcontrols(px, false);
    set_game_phase(px, PX_GAME_PHASE_INTRO);

    // TODO (DK) "play intro"
        // disable ship controls
        // show player 1 or 2 | get ready
        // wait for button press
        // hide text while crafts fly in from left
        // enable controls
        // set scroll speed
}

enum seuck_load_status px_level_load_update( struct px_context *px ) {
    oxi_id *files = (void *)&px->game.res.files;
    uint8_t n = sizeof(px->game.res.files) / sizeof(oxi_id);

    for (uint8_t i = 0; i < n; ++i) {
        if (!oxi_io_is_loaded(px->oxi.sys.io, files[i])) {
            return SEUCK_LOAD_STATUS_IN_PROGRESS;
        }
    }

    for (int i = 0; i < px->res.num_sounds; ++i) {
        if (!oxi_io_is_loaded(px->oxi.sys.io, px->res.sound_ids[i])) {
            return SEUCK_LOAD_STATUS_IN_PROGRESS;
        }
    }

    struct oxi_texture_system *textures = px->oxi.sys.textures;
    struct oxi_io_system *io = px->oxi.sys.io;

    px->game.res.textures.tileset = oxi_texture_from_image(textures, &oxi_io_get_image_data(io, px->game.res.files.tileset)->image);
    px->game.res.textures.sprites = oxi_texture_from_image(textures, &oxi_io_get_image_data(io, px->game.res.files.sprites)->image);
    px->game.res.textures.explosion = oxi_texture_from_image(textures, &oxi_io_get_image_data(io, px->game.res.files.explosion)->image);
    px->game.res.textures.bullets = oxi_texture_from_image(textures, &oxi_io_get_image_data(io, px->game.res.files.bullets)->image);
    px->res.textures.player = oxi_texture_from_image(textures, &oxi_io_get_image_data(io, px->res.files.player)->image);
    px->seuck.level_data = oxi_io_get_blob_data(io, px->game.res.files.level_id)->bytes;

    px->sfx = oxi_alloc(px->res.num_sounds * sizeof(struct kinc_a1_sound *), NULL);

    for (int i = 0; i < px->res.num_sounds; ++i) {
        px->sfx[i] = oxi_io_get_sound_data(io, px->res.sound_ids[i])->a1;
    }

    static const char * HUD_STR = "SPEED GUNS BUILDER SIDE MISSLE PLASMA MAGMA LAZER STEALTH"; // TODO (DK) don't hardcode here
    seuck_cache_text(px->res.fonts.font2, &px->game.hud.loadout, HUD_STR, px->res.textures.font2);

    init_level(px);

    return SEUCK_LOAD_STATUS_SUCCESS;
}

static int spawn_delay = 30;
static int max_spawn_delay = 30;

static void spawn_enemies( struct px_context *px ) {
    spawn_delay -= 1;

    if (spawn_delay == 0) {
        spawn_delay = max_spawn_delay;

        for (int i = 0; i < SEUCK_MAX_ENEMIES; ++i) {
            if (oxi_bit_has(px->seuck.enemy_instance_alive, i)) {
                continue;
            }

            struct seuck_enemy_data_v2 *dat = pxd_enemy_get(px->data, 0); // TODO (DK) proper id
            struct seuck_enemy_instance *e = &px->seuck.enemies[i];
            e->x = px->seuck.stage.camera.x + 400; // TODO (DK)
            e->y = oxi_random_range_inc_i(0, 200); // TODO (DK)
            e->dx = -oxi_random_range_inc_i(1, 3); // TODO (DK) path
            e->dy = 0; // TODO (DK) path
            e->anim = *pxd_animation_get(px->data, dat->anim_ref);
            e->anim_state.frame = e->anim.frames_start_ref;
            e->anim_state.frame_index = 0;
            e->anim_state.elapsed = 0;
            e->anim_state.repeat = 0;
            e->enemy_ref = 1;
            e->age = 0;
            e->hp = dat->hp;
            px->seuck.enemy_instance_alive = oxi_bit_set(px->seuck.enemy_instance_alive, i);

            struct seuck_spritesheet_frame *frame = pxd_frame_get(px->data, e->anim_state.frame);
            struct seuck_region_data *region = pxd_region_get(px->data, frame->region_ref);
            e->w = region->w;
            e->h = region->h;

            break;
        }
    }
}

static void spawn_explosion( struct px_context *px, int x, int y ) {
    struct seuck_context *g = &px->seuck;
    struct seuck_explosion_instance *e = NULL;

    for (int i = 0; i < SEUCK_MAX_EXPLOSIONS; ++i) {
        struct seuck_explosion_instance *inst = &px->seuck.explosions[i];

        if (!oxi_bit_has(inst->flags, SEUCK_EXPLOSION_INSTANCE_FLAG_ALIVE)) {
            e = inst;
            break;
        }
    }

    if (e == NULL) {
        oxi_trace("spawn_explosion: increase SEUCK_MAX_EXPLOSIONS, current buffer is too small");
        return;
    }

    e->anim = *pxd_animation_get(px->data, EXPLOSION_ANIMATION_SHEET_ROW); // TODO (DK)
    e->anim_state.frame = e->anim.frames_start_ref;
    e->anim_state.frame_index = 0;
    e->anim_state.elapsed = 0;
    e->anim_state.repeat = 0;
    e->x = x;// + 200;
    e->y = y;// + 100;
    e->age = 0;
    e->flags = oxi_bit_set(e->flags, SEUCK_EXPLOSION_INSTANCE_FLAG_ALIVE);

    kinc_a1_play_sound(px->sfx[EXPLOSION_SOUND_ROW], false, 1.0f, false);
}

static void on_enemy_destroyed( void *ctx, int idx ) {
    struct px_context *px = ctx;
    struct seuck_context *g = &px->seuck;
    // play explosion
    // play explosion sound
    // add score
    // check for wave end and spawn powerup (project x)
    // spawn powerup if it was a powerup container (r-type / katakis / stargunner / ...)
    // spawn gems/coins (stargunner / ...)
    // check for level end

    seuck_kill_enemy(g, idx);

    struct seuck_enemy_instance *en = &g->enemies[idx];

    // TODO (DK) multiple or even animated explosions for bigger enemies
        // put into data: { num, delay }
    spawn_explosion(px, en->x, en->y);
}

static void update( struct px_context *px ) {
    // TODO (DK) check collisions while moving? to prevent tunneling

    spawn_enemies(px); // TODO (DK) or after seuck_level_update()?
    seuck_move_enemy_instances(&px->seuck);
    seuck_move_bullets(&px->seuck);
    // move powerups
    seuck_move_players(&px->seuck);
    seuck_process_playerbullet_vs_enemy_collisions(&px->seuck, on_enemy_destroyed, px);
    seuck_update_camera(&px->seuck);
    seuck_reset_player_speed(&px->seuck);
    seuck_advance_animations(&px->seuck);
    seuck_update_explosions(&px->seuck);

    switch (px->game.phase) {
        case PX_GAME_PHASE_INTRO: {
            // TODO (DK) call after "intro" is finished
            set_game_phase(px, PX_GAME_PHASE_GAMEPLAY);
            toggle_shipcontrols(px, true);
            seuck_autoscrollspeed_set(&px->seuck, 2, 0);
        } break;

        case PX_GAME_PHASE_GAMEPLAY: {
            for (int p = 0; p < SEUCK_MAX_PLAYERS; ++p) {
                for (int g = 0; g < MAX_PX_POWERUP_KIND; ++g) {
                    uint8_t *cd = &px->player_item_cooldown[p].level[g]; // TODO (DK) could be made generic and moved into seuck?

                    if (*cd > 0) {
                        *cd -= 1;
                    }
                }
            }
        } break;

        case PX_GAME_PHASE_TRANSITION_TO_BOSS: {
        } break;

        case PX_GAME_PHASE_OUTRO: {
        } break;
    }
}

void pxi_game_update( struct px_context *px ) {
    switch (px->game.setup) {
        case PX_STATE_SETUP_LOAD: {
            switch (px_level_load_update(px)) {
                case SEUCK_LOAD_STATUS_IN_PROGRESS: {
                } break;

                case SEUCK_LOAD_STATUS_SUCCESS: {
                    px->game.setup = PX_STATE_SETUP_FINISHED;
                } break;

                case SEUCK_LOAD_STATUS_FAILURE: {
                } break;
            }
        } break;

        case PX_STATE_SETUP_FINISHED: {
            update(px);
        } break;
    }
}
