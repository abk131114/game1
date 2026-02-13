#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>

#include <stb_ds.h>

void draw_blobs( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_blob_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->blobs);
        arrput(app->gui.data->blobs, d);
    }

    ImGui::SameLine();

    if (ImGui::Button("DELETE SELECTED")) {
        gui_selected_items_delete(app->gui.data->blobs);
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

    if (ImGui::BeginTable("BLOBS", 3, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("FILE", ImGuiTableColumnFlags_WidthStretch);
        // ImGui::TableSetupColumn("##load");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_blob_data *blobs = app->gui.data->blobs;

        for (size_t row = 0; row < arrlenu(blobs); ++row) {
            struct gui_blob_data *b = &blobs[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &b->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", b->id);

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::InputText("", b->d.uri, sizeof(b->d.uri));
            ImGui::SameLine();
            if (ImGui::Button("...")) {
                // https://github.com/mlabbe/nativefiledialog
            }

            ImGui::PopID();
            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
