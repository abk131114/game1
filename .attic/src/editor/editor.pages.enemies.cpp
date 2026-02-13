#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>

#include <stb_ds.h>

void draw_enemies( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_enemy_data_v2 d = { 0 };
        d.id = (int)arrlen(app->gui.data->enemies);
        arrput(app->gui.data->enemies, d);
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     gui_selected_items_delete(app->gui.data->enemies);
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

    // uint8_t faction;
    // uint16_t anim_ref;
    // uint16_t score;
    // // radius / collision mask / type ?
    // // fly path / pattern? or is this instance?

    if (ImGui::BeginTable("wls", 6, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("HITPOINTS", 0);
        ImGui::TableSetupColumn("SCORE", 0);
        ImGui::TableSetupColumn("ANIM REF", 0);
        ImGui::TableSetupColumn("PREVIEW", 0);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_enemy_data_v2 *enemies = app->gui.data->enemies;

        for (size_t row = 0; row < arrlenu(enemies); ++row) {
            struct gui_enemy_data_v2 *en = &enemies[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &en->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", en->id);

            ImGui::TableNextColumn();
            int hp = en->d.hp;
            ImGui::InputInt("##hp", &hp);
            en->d.hp = hp;

            ImGui::TableNextColumn();
            int score = en->d.score;
            ImGui::InputInt("##score", &score);
            en->d.score = score;

            ImGui::TableNextColumn();
            anim_combo(app, "##anim", &en->d.anim_ref);

            ImGui::TableNextColumn();
            draw_anim(app, en->d.anim_ref);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
