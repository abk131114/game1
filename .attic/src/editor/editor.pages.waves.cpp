#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>

#include <stb_ds.h>

void draw_waves( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_wave_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->waves);
        arrput(app->gui.data->waves, d);
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     gui_selected_items_delete(app->gui.data->waves);
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

    if (ImGui::BeginTable("wave", 7, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("FROM", 0);
        ImGui::TableSetupColumn("TO", 0);
        ImGui::TableSetupColumn("X OFFSET", 0);
        ImGui::TableSetupColumn("Y OFFSET", 0);

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_wave_data *waves = app->gui.data->waves;

        for (size_t row = 0; row < arrlenu(waves); ++row) {
            struct gui_wave_data *wave = &waves[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &wave->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", wave->id);

            ImGui::TableNextColumn();
            wavespawn_combo(app, "##from", &wave->d.spawn_ref_from);

            ImGui::TableNextColumn();
            wavespawn_combo(app, "##to", &wave->d.spawn_ref_to);

            ImGui::TableNextColumn();
            int x = wave->d.offset_x;
            ImGui::InputInt("##x", &x);
            wave->d.offset_x = x & 0xffff;

            ImGui::TableNextColumn();
            int y = wave->d.offset_y;
            ImGui::InputInt("##y", &y);
            wave->d.offset_y = y & 0xffff;

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
