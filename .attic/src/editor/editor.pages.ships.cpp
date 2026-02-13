#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>

#include <stb_ds.h>

void draw_ships( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_ship_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->ships);
        arrput(app->gui.data->ships, d);
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     gui_selected_items_delete(app->gui.data->ships);
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

    if (ImGui::BeginTable("ships", 11, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("FACTION", 0);
        ImGui::TableSetupColumn("INITIAL LOADOUT", 0);
        ImGui::TableSetupColumn("MAX LOADOUT", 0);
        ImGui::TableSetupColumn("IDLE ANIM", 0);
        ImGui::TableSetupColumn("IDLE ANIM PREVIEW", 0);
        ImGui::TableSetupColumn("FLYUP ANIM", 0);
        ImGui::TableSetupColumn("FLYUP ANIM PREVIEW", 0);
        ImGui::TableSetupColumn("FLYDOWN ANIM", 0);
        ImGui::TableSetupColumn("FLYDOWN ANIM PREVIEW", 0);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_ship_data *ships = app->gui.data->ships;

        for (size_t row = 0; row < arrlenu(ships); ++row) {
            struct gui_ship_data *ship = &ships[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &ship->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", ship->id);

            ImGui::TableNextColumn();
            int fac = ship->d.faction;
            ImGui::InputInt("##faction", &fac);
            ship->d.faction = fac;

            ImGui::TableNextColumn();
            loadout_combo(app, "##initial_l", &ship->d.initial_loadout_ref);

            ImGui::TableNextColumn();
            loadout_combo(app, "##max_l", &ship->d.maximum_loadout_ref);

            ImGui::TableNextColumn();
            anim_combo(app, "##idle", &ship->d.idle_anim_ref);

            ImGui::TableNextColumn();
            draw_anim(app, ship->d.idle_anim_ref);

            ImGui::TableNextColumn();
            anim_combo(app, "##flyup", &ship->d.flyup_anim_ref);

            ImGui::TableNextColumn();
            draw_anim(app, ship->d.flyup_anim_ref);

            ImGui::TableNextColumn();
            anim_combo(app, "##flydown", &ship->d.flydown_anim_ref);

            ImGui::TableNextColumn();
            draw_anim(app, ship->d.flydown_anim_ref);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
