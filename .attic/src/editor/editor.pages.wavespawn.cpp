#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>

#include <stb_ds.h>

void draw_wavespawns( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_wavespawn_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->wavespawns);
        arrput(app->gui.data->wavespawns, d);
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     gui_selected_items_delete(app->gui.data->wavespawns);
    // }

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

    if (ImGui::BeginTable("wavespawn", 8, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("LEVEL", 0); // meta
        ImGui::TableSetupColumn("ENEMY", 0); //
        ImGui::TableSetupColumn("SPAWNPOINT", 0);
        ImGui::TableSetupColumn("PATH", 0);
        ImGui::TableSetupColumn("DELAY", 0);
        ImGui::TableSetupColumn("PREVIEW", 0);

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_wavespawn_data *spawns = app->gui.data->wavespawns;

        for (size_t row = 0; row < arrlenu(spawns); ++row) {
            struct gui_wavespawn_data *spawn = &spawns[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &spawn->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", spawn->id);

            ImGui::TableNextColumn();
            int lvl = spawn->level;
            ImGui::InputInt("##level", &lvl);
            spawn->level = lvl;

            ImGui::TableNextColumn();
            enemy_combo(app, "##enemy", &spawn->d.enemy_ref);

            ImGui::TableNextColumn();
            spawnpoint_combo(app, "##spawnpoint", &spawn->d.spawnpoint_ref);

            ImGui::TableNextColumn();
            path_combo(app, "##path", &spawn->d.path_ref);

            ImGui::TableNextColumn();
            int delay = 0;
            ImGui::InputInt("##delay", &delay);
            spawn->d.delay = delay;

            ImGui::TableNextColumn();
            struct gui_enemy_data_v2 *enemy = gui_enemy_find(app->gui.data, spawn->d.enemy_ref);
            if (enemy != NULL) {
                draw_anim(app, enemy->d.anim_ref);
            } else {
                ImGui::Spacing();
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
