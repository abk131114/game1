#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"
#include "editor.data.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <kinc/graphics4/texture.h>
#include <stb_ds.h>

#define DEFINE_REF_COMBO(Name, Type, Fn, Array, Size) \
    void Name( struct app_context *app, const char *label, Size *current_item_idx ) { \
        struct Type *a = app->gui.data->Array; \
        struct Type *combo_preview_value = gui_##Fn##_find(app->gui.data, *current_item_idx); \
\
        const size_t n = sizeof(int) * 2; \
        char preview_value[n]; \
        snprintf(preview_value, n, "%i", combo_preview_value != NULL ? combo_preview_value->id : -1); \
\
        if (ImGui::BeginCombo(label, preview_value, ImGuiComboFlags_HeightRegular)) { \
            for (size_t item = 0; item < arrlenu(a); ++item) { \
                const bool is_selected = (*current_item_idx == item); \
\
                ImGui::PushID(item); \
\
                snprintf(preview_value, 32, "%i", a[item].id); \
\
                if (ImGui::Selectable(preview_value, is_selected)) { \
                    *current_item_idx = item; \
                } \
\
                if (is_selected) { \
                    ImGui::SetItemDefaultFocus(); \
                } \
\
                ImGui::PopID(); \
            } \
\
            ImGui::EndCombo(); \
        } \
    }

void blob_combo( struct app_context *app, const char *label, uint8_t *current_item_idx ) {
    struct gui_blob_data *blobs = app->gui.data->blobs;
    struct gui_blob_data *combo_preview_value = gui_blob_find(app->gui.data, *current_item_idx);

    if (ImGui::BeginCombo(label, combo_preview_value->d.uri, ImGuiComboFlags_HeightRegular)) {
        for (size_t item = 0; item < arrlenu(blobs); ++item) {
            const bool is_selected = (*current_item_idx == item);

            ImGui::PushID(item);

            if (ImGui::Selectable(blobs[item].d.uri, is_selected)) {
                *current_item_idx = item;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }

            ImGui::PopID();
        }

        ImGui::EndCombo();
    }
}

void image_combo( struct app_context *app, const char *label, uint8_t *current_item_idx ) {
    struct gui_image_data *images = app->gui.data->images;
    struct gui_image_data *combo_preview_value = gui_image_find(app->gui.data, *current_item_idx);

    if (ImGui::BeginCombo(label, combo_preview_value->d.uri, ImGuiComboFlags_HeightRegular)) {
        for (size_t item = 0; item < arrlenu(images); ++item) {
            const bool is_selected = (*current_item_idx == item);

            ImGui::PushID(item);

            if (ImGui::Selectable(images[item].d.uri, is_selected)) {
                *current_item_idx = item;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }

            ImGui::PopID();
        }

        ImGui::EndCombo();
    }
}

void sound_combo( struct app_context *app, const char *label, uint8_t *current_item_idx ) {
    struct gui_sound_data *sounds = app->gui.data->sounds;
    struct gui_sound_data *combo_preview_value = gui_sound_find(app->gui.data, *current_item_idx);

    if (ImGui::BeginCombo(label, combo_preview_value->d.uri, ImGuiComboFlags_HeightRegular)) {
        for (size_t item = 0; item < arrlenu(sounds); ++item) {
            const bool is_selected = (*current_item_idx == item);

            ImGui::PushID(item);

            if (ImGui::Selectable(sounds[item].d.uri, is_selected)) {
                *current_item_idx = item;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }

            ImGui::PopID();
        }

        ImGui::EndCombo();
    }
}

DEFINE_REF_COMBO(region_combo, gui_region_data, region, regions, uint16_t)
DEFINE_REF_COMBO(frame_combo, gui_frame_data, frame, frames, uint16_t)
DEFINE_REF_COMBO(anim_combo, gui_anim_data, anim, anims, uint16_t)
DEFINE_REF_COMBO(bullet_combo, gui_bullet_data, bullet, bullets, uint16_t)
DEFINE_REF_COMBO(loadout_combo, gui_loadout_data, loadout, loadouts, uint8_t)
DEFINE_REF_COMBO(bulletspawn_combo, gui_bulletspawn_data, bulletspawn, bulletspawns, uint16_t)
DEFINE_REF_COMBO(enemy_combo, gui_enemy_data_v2, enemy, enemies, uint16_t)
DEFINE_REF_COMBO(spawnpoint_combo, gui_spawnpoint_data, spawnpoint, spawnpoints, uint8_t)
DEFINE_REF_COMBO(waypoint_combo, gui_waypoint_data, waypoint, waypoints, uint16_t)
DEFINE_REF_COMBO(path_combo, gui_path_data, path, paths, uint8_t)
DEFINE_REF_COMBO(wavespawn_combo, gui_wavespawn_data, wavespawn, wavespawns, uint16_t)

void bulletkind_combo( struct app_context *app, const char *label, uint8_t *current_item_idx ) {
    int current = *current_item_idx;
    ImGui::Combo(label, &current, "a\0b\0c\0d\0");
    *current_item_idx = current & 0xff;
}

void powerup_combo( struct app_context *app, const char *label, uint8_t *current_item_idx ) {
    int current = *current_item_idx;
    ImGui::Combo(label, &current, "speed\0guns\0builder\0side\0missile\0plasma\0magma\0laser\0stealth");
    *current_item_idx = current & 0xff;
}

void draw_anim( struct app_context *app, uint16_t anim_id ) {
    struct gui_anim_cache_item *aci = gui_anim_cache_item_find(app, anim_id);
    int fidx = aci->frame;
    struct gui_frame_data *frm = gui_frame_find(app->gui.data, fidx);
    struct gui_region_data *reg = gui_region_find(app->gui.data, frm->d.region_ref);
    struct kinc_g4_texture *texture = app->gui.res.texture_cache[reg->d.atlas_ref].texture;

    if (texture == NULL || app->gui.opts.show_anims == 0) {
        ImGui::Text("");
    } else {
        float tw = texture->tex_width;
        float th = texture->tex_height;
        float u0 = reg->d.x / tw;
        float v0 = reg->d.y / th;
        float u1 = (reg->d.x + reg->d.w) / tw;
        float v1 = (reg->d.y + reg->d.h) / th;
        ImGui::Image(texture, ImVec2(reg->d.w, reg->d.h), ImVec2(u0, v0), ImVec2(u1, v1));
    }
}

void draw_region( struct app_context *app, uint16_t region_id ) {
    struct gui_region_data *reg = gui_region_find(app->gui.data, region_id);
    struct kinc_g4_texture *texture = app->gui.res.texture_cache[reg->d.atlas_ref].texture;

    if (texture == NULL || app->gui.opts.show_frames == 0) {
        ImGui::Text("");
    } else {
        float tw = texture->tex_width;
        float th = texture->tex_height;
        float u0 = reg->d.x / tw;
        float v0 = reg->d.y / th;
        float u1 = (reg->d.x + reg->d.w) / tw;
        float v1 = (reg->d.y + reg->d.h) / th;
        ImGui::Image(texture, ImVec2(reg->d.w, reg->d.h), ImVec2(u0, v0), ImVec2(u1, v1));
    }
}

#endif
