#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>

#include <stb_ds.h>

void draw_wls( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_weaponlevelshot_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->wls);
        arrput(app->gui.data->wls, d);
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE UP (FIRST)")) {
        for (size_t row = 0; row < arrlenu(app->gui.data->wls); ++row) {
            struct gui_weaponlevelshot_data *a = &app->gui.data->wls[row];

            if (a->selected) {
                if (row > 0) {
                    struct gui_weaponlevelshot_data d = app->gui.data->wls[row];
                    arrins(app->gui.data->wls, row - 1, d);
                    arrdel(app->gui.data->wls, row + 1);
                    break;
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE DOWN (FIRST)")) {
        size_t n = arrlenu(app->gui.data->wls);

        for (size_t row = 0; row < n; ++row) {
            struct gui_weaponlevelshot_data *a = &app->gui.data->wls[row];

            if (a->selected) {
                if (row < n - 1) {
                    struct gui_weaponlevelshot_data d = app->gui.data->wls[row];
                    arrins(app->gui.data->wls, row + 2, d);
                    arrdel(app->gui.data->wls, row);
                    break;
                }
            }
        }
    }

    // if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs()) {
    // 	if (sort_specs->SpecsDirty) {
    // 		MyItem::SortWithSortSpecs(sort_specs, items.Data, items.Size);
    // 		sort_specs->SpecsDirty = false;
    // 	}
    // }

    ImGuiTableFlags flags = ImGuiTableFlags_Resizable
        | ImGuiTableFlags_Reorderable
        | ImGuiTableFlags_Hideable
        | ImGuiTableFlags_Sortable
        | ImGuiTableFlags_SortMulti
        | ImGuiTableFlags_RowBg
        | ImGuiTableFlags_BordersOuter
        | ImGuiTableFlags_BordersV
        | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("wls", 9, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("POWERUP", 0);
        ImGui::TableSetupColumn("LEVEL", 0);
        ImGui::TableSetupColumn("START REF", 0);
        ImGui::TableSetupColumn("END REF", 0);
        ImGui::TableSetupColumn("COOLDOWN", 0);
        ImGui::TableSetupColumn("SOUND", 0);
        ImGui::TableSetupColumn("SOUND REF", 0);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_weaponlevelshot_data *wlss = app->gui.data->wls;

        for (size_t row = 0; row < arrlenu(wlss); ++row) {
            struct gui_weaponlevelshot_data *shot = &wlss[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &shot->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", shot->id);

            ImGui::TableNextColumn();
            uint8_t powerup = PX_WLS_GET_WEAPON(shot->d.key);
            powerup_combo(app, "##powerup", &powerup);

            ImGui::TableNextColumn();
            int lvl = PX_WLS_GET_LEVEL(shot->d.key);
            ImGui::InputInt("##level", &lvl);
            shot->d.key = PX_WLS_MAKE_KEY(powerup, lvl);

            ImGui::TableNextColumn();
            bulletspawn_combo(app, "##start_ref", &shot->d.start_ref);

            ImGui::TableNextColumn();
            bulletspawn_combo(app, "##end_ref", &shot->d.end_ref);

            ImGui::TableNextColumn();
            int cd = shot->d.cooldown;
            ImGui::InputInt("##cooldown", &cd);
            shot->d.cooldown = cd;

            ImGui::TableNextColumn();
            bool has_sound = shot->d.sound.enabled;
            ImGui::Checkbox("##sound", &has_sound);
            shot->d.sound.enabled = has_sound;

            ImGui::TableNextColumn();
            uint8_t ref = shot->d.sound.ref;
            sound_combo(app, "##soundref", &ref);
            shot->d.sound.ref = ref;

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
