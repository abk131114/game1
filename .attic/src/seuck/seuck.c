#include "seuck.h"

#include <oxi/oxi.geometry-system.h>
#include <oxi/oxi.input.h>
#include <oxi/oxi.io.h>
#include <oxi/oxi.shader-system.h>
#include <oxi/oxi.spritebatcher.h>
#include <oxi/oxi.texture-system.h>

void seuck_init( struct seuck_context *g, struct seuck_init_args *args ) {
    memset(g, 0, sizeof(*g));

    g->cfg = *args;
}

void seuck_move_enemy_instances( struct seuck_context *g ) {
    uint64_t alive = g->enemy_instance_alive;

    for (uint8_t i = 0; i < SEUCK_MAX_ENEMIES; ++i) {
        if (!oxi_bit_has(alive, i)) {
            continue;
        }

        struct seuck_enemy_instance *en = &g->enemies[i];

        if (en->age >= 0) {
            en->age += 1;
            en->x += en->dx;
            en->y += en->dy;
        }

        if (en->age >= 180) {
            g->enemy_instance_alive = oxi_bit_clear(g->enemy_instance_alive, i);
        }
    }
}

void seuck_update_explosions( struct seuck_context *g ) {
    for (int i = 0; i < SEUCK_MAX_EXPLOSIONS; ++i) {
        struct seuck_explosion_instance *inst = &g->explosions[i];
        inst->age += 1;

        if (inst->age >= 30) { // TODO (DK) don't hardcode; just use frames instead?
            inst->flags = oxi_bit_clear(inst->flags, SEUCK_EXPLOSION_INSTANCE_FLAG_ALIVE);
        }
    }
}

void seuck_move_bullets( struct seuck_context *g ) {
    for (int f = 0; f < SEUCK_MAX_FACTIONS; ++f) {
        for (int b = 0; b < SEUCK_MAX_BULLETS; ++b) {
            struct seuck_bullet_instance *bi = &g->bullets[f][b];

            if (!oxi_bit_has(bi->flags, SEUCK_BULLET_INSTANCE_FLAG_ALIVE)) {
                continue;
            }

            bi->age += 1;
            bi->x += bi->dx;
            bi->y += bi->dy;

            if (bi->age >= 180) {
                bi->flags = oxi_bit_clear(bi->flags, SEUCK_BULLET_INSTANCE_FLAG_ALIVE);
            }
        }
    }
}

void seuck_update_camera( struct seuck_context *g ) {
    g->stage.camera.x += g->stage.autoscroll_h;
    g->stage.camera.y += g->stage.autoscroll_v;

    g->stage.camera.x = oxi_clamp_i32(g->stage.camera.x, 0, 16000); // g-> LEVEL_WIDTH - SCREEN_WIDTH);
    g->stage.camera.y = oxi_clamp_i32(g->stage.camera.y, 0, 240); //LEVEL_HEIGHT - SCREEN_HEIGHT);
}

void seuck_move_players( struct seuck_context *g ) {
    // TODO (DK) keep players in camera area

    for (int i = 0; i < SEUCK_MAX_PLAYERS; ++i) {
        if (!oxi_bit_has(g->player_alive, i)) {
            continue;
        }

        g->players[i].x += g->player_speed[i].x;
        g->players[i].y += g->player_speed[i].y;

        g->players[i].x += g->stage.autoscroll_h;
        g->players[i].y += g->stage.autoscroll_v;
    }
}

void seuck_process_playerbullet_vs_enemy_collisions( struct seuck_context *g, void(*on_enemy_destroyed)( void *, int ), void *ctx ) {
    for (int b = 0; b < SEUCK_MAX_BULLETS; ++b) {
        struct seuck_bullet_instance *bi = &g->bullets[0][b];

        if (!oxi_bit_has(bi->flags, SEUCK_BULLET_INSTANCE_FLAG_ALIVE)) {
            continue;
        }

        for (int e = 0; e < SEUCK_MAX_ENEMIES; ++e) {
            if (!oxi_bit_has(g->enemy_instance_alive, e)) {
                continue;
            }

            struct seuck_enemy_instance *en = &g->enemies[e];

            if (bi->x + bi->w   < en->x
            ||  bi->x           > en->x + en->w
            ||  bi->y + bi->h   < en->y
            ||  bi->y           > en->y + en->h
            ) {
                continue;
            }

            bi->flags = oxi_bit_clear(bi->flags, SEUCK_BULLET_INSTANCE_FLAG_ALIVE);

            if (bi->bullet.damage >= en->hp) {
                on_enemy_destroyed(ctx, e);
            } else {
                en->hp -= bi->bullet.damage;
                // TODO (DK) flash white or something, play hit sound
            }
        }
    }
}

void seuck_kill_enemy( struct seuck_context *g, int idx ) {
    g->enemy_instance_alive = oxi_bit_clear(g->enemy_instance_alive, idx);
    g->enemies[idx].hp = 0;
}

void seuck_animation_advance( struct seuck_spritesheet_animation *anim, struct seuck_spritesheet_animation_state *state ) {
    state->elapsed += 1;
    uint16_t end = anim->frames_end_ref;
    uint16_t start = anim->frames_start_ref;
    uint16_t num_frames = end - start + 1;

    if (state->elapsed >= num_frames) {
        uint16_t new_frame_idx = state->frame_index;

        if (start + new_frame_idx + 1 >= end) {
            state->repeat += 1;

            if (state->repeat < anim->repeat) {
                new_frame_idx = 0;
            }
        } else {
            new_frame_idx += 1;
        }

        state->elapsed = 0; // TODO (DK) subtract num_frames?
        state->frame_index = new_frame_idx;
        state->frame = anim->frames_start_ref + new_frame_idx;
    }
}

void seuck_reset_player_speed( struct seuck_context *g ) {
    for (int i = 0; i < SEUCK_MAX_PLAYERS; ++i) {
        g->player_speed[i].x = 0;
        g->player_speed[i].y = 0;
    }
}

void seuck_advance_animations( struct seuck_context *g ) {
    for (int fidx = 0; fidx < SEUCK_MAX_FACTIONS; ++fidx) {
        for (int bidx = 0; bidx < SEUCK_MAX_BULLETS; ++bidx) {
            struct seuck_bullet_instance *b = &g->bullets[fidx][bidx];
            seuck_animation_advance(&b->anim, &b->anim_state);
        }
    }

    for (int idx = 0; idx < SEUCK_MAX_ENEMIES; ++idx) {
        struct seuck_enemy_instance *e = &g->enemies[idx];
        seuck_animation_advance(&e->anim, &e->anim_state);
    }

    for (int idx = 0; idx < SEUCK_MAX_PLAYERS; ++idx) {
        struct seuck_player_instance *p = &g->players[idx];
        seuck_animation_advance(&p->anim, &p->anim_state);
    }

    for (int idx = 0; idx < SEUCK_MAX_EXPLOSIONS; ++idx) {
        struct seuck_explosion_instance *e = &g->explosions[idx];
        seuck_animation_advance(&e->anim, &e->anim_state);
    }
}

void seuck_autoscrollspeed_set( struct seuck_context *g, int8_t h, int8_t v ) {
    g->stage.autoscroll_h = h;
    g->stage.autoscroll_v = v;
}

uint16_t seuck_text_width( struct seuck_bmfont_string *str ) {
    int width = 0;

    for (int c = 0; c < sizeof(str->ch) / sizeof(str->ch[0]); ++c) {
        struct bmfont_char *fch = str->ch[c];

        if (fch == NULL) {
            continue;
        }

        width += fch->xadvance;
    }

    return width;
}

static void advance_to_next_selectable_entry( struct seuck_context *g, int8_t dir ) {
    int sel = g->menu.state.selected_idx;
    int idx = sel;
    int n = g->menu.state.num_items;
    int i = 0;

    do {
        idx += dir;

        if (idx >= n) {
            idx -= n;
        }

        if (idx < 0) {
            idx += n;
        }

        if (g->menu.items[idx].selectable != 0) {
            sel = idx;
            break;
        }

        i += 1;
    } while (i < n);

    g->menu.state.selected_idx = sel;
}

void seuck_menu_nav_up( struct seuck_context *g ) {
    advance_to_next_selectable_entry(g, -1);
}

void seuck_menu_nav_down( struct seuck_context *g ) {
    advance_to_next_selectable_entry(g, 1);
}

void seuck_menu_nav_left( struct seuck_context *g ) {
    oxi_trace("seuck_menu_nav_left: implement me");
}

void seuck_menu_nav_right( struct seuck_context *g ) {
    oxi_trace("seuck_menu_nav_left: implement me");
}

void seuck_cache_text( struct bmfont_file *fnt, struct seuck_bmfont_string *cache, const char *str, struct oxi_texture *tex ) {
    size_t len = strlen(str);
    cache->tex = tex;
    cache->len = len;

    for (int c = 0; c < len; ++c) {
        char ch = str[c];

        for (int fc = 0; fc < fnt->num_chars; ++fc) {
            struct bmfont_char *fch = &fnt->chars[fc];

            if (fch->id != ch) {
                continue;
            }

            cache->ch[c] = fch;
        }
    }
}

uint8_t seuck_build_menu(
    struct seuck_context *g, struct bmfont_file *fnt,
    struct seuck_menu_content *content, int menu_idx,
    struct oxi_texture *tex
) {
    struct seuck_menu_data *menu = &content->menus[menu_idx]; // pxd_menu_find()
    uint8_t idx = 0, str_idx = 1;

    for (size_t i = menu->from; i <= menu->to; ++i) {
        struct seuck_menu_item_data *item = &content->menu_items[i]; // pxd_menuitem_find()
        struct seuck_menu_item *cache = &g->menu.items[idx];

        switch (item->kind) {
            case seuck_menu_item_kind_action: {
                struct seuck_menu_action_data *act = &content->actions[item->item_index]; // pxd_menuaction_find()
                cache->kind = item->kind;
                cache->userdata = item->userdata;
                cache->ctx = g;
                cache->selectable = 1;
                cache->text_idx = str_idx;
                const char *text = content->strings[act->text_ref].buf; // pxd_menustring_find()
                seuck_cache_text(fnt, &g->menu.strings[str_idx], text, tex);
                idx += 1;
                str_idx += 1;
            } break;

            case seuck_menu_item_kind_empty_separator: {
                cache->text_idx = 0;
                cache->kind = item->kind;
                cache->userdata = item->userdata;
                cache->ctx = g;
                cache->selectable = 0;
                cache->text_idx = 0;
                idx += 1;
            } break;

            case seuck_menu_item_kind_label: {
                struct seuck_menu_label_data *lbl = &content->labels[item->item_index]; // pxd_menulabel_find()
                cache->kind = item->kind;
                cache->userdata = item->userdata;
                cache->ctx = g;
                cache->selectable = 0;
                cache->text_idx = str_idx;
                const char *text = content->strings[lbl->text_ref].buf; // pxd_menustring_find()
                seuck_cache_text(fnt, &g->menu.strings[str_idx], text, tex);
                idx += 1;
                str_idx += 1;
            } break;

            case seuck_menu_item_kind_submenu: {
                struct seuck_menu_submenu_data *sm = &content->submenus[item->item_index]; // pxd_menusubmenu_find()
                cache->kind = item->kind;
                cache->userdata = item->userdata;
                cache->ctx = g;
                cache->selectable = 1;
                cache->text_idx = str_idx;
                const char *text = content->strings[sm->text_ref].buf; // pxd_menustring_find()
                seuck_cache_text(fnt, &g->menu.strings[str_idx], text, tex);
                idx += 1;
                str_idx += 1;
            } break;

            case seuck_menu_item_kind_toggle: {
                struct seuck_menu_toggle_data *toggle = &content->toggles[item->item_index]; // pxd_menusubmenu_find()
                cache->kind = item->kind;
                cache->userdata = item->userdata;
                cache->ctx = g;
                cache->selectable = 1;
                cache->text_idx = str_idx;
                const char *a = content->strings[toggle->a_text_ref].buf; // pxd_menustring_find()
                seuck_cache_text(fnt, &g->menu.strings[str_idx], a, tex);
                idx += 1;
                str_idx += 1;
                const char *b = content->strings[toggle->b_text_ref].buf; // pxd_menustring_find()
                seuck_cache_text(fnt, &g->menu.strings[str_idx], b, tex);
                str_idx += 1;
            } break;
        }
    }

    return idx;
}
