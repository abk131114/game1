#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>

#include <stb_ds.h>

void draw_levels( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_level_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->levels);
        arrput(app->gui.data->levels, d);
    }

    ImGui::SameLine();

    if (ImGui::Button("DELETE SELECTED")) {
        gui_selected_items_delete(app->gui.data->levels);
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

    if (ImGui::BeginTable("levels", 5, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("DATA", 0);
        ImGui::TableSetupColumn("TILES", 0);
        ImGui::TableSetupColumn("SPRITES", 0);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_level_data *levels = app->gui.data->levels;

        for (size_t row = 0; row < arrlenu(levels); ++row) {
            struct gui_level_data *lvl = &levels[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &lvl->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", lvl->id);

            ImGui::TableNextColumn();
            blob_combo(app, "##level", &lvl->d.level_ref);

            ImGui::TableNextColumn();
            image_combo(app, "##tiles", &lvl->d.tileset_ref);

            ImGui::TableNextColumn();
            image_combo(app, "##sprites", &lvl->d.sprites_ref);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
