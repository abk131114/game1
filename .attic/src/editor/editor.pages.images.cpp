#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>

#include <stb_ds.h>

void draw_images( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_image_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->images);
        arrput(app->gui.data->images, d);
    }

    ImGui::SameLine();

    if (ImGui::Button("DELETE SELECTED")) {
        gui_selected_items_delete(app->gui.data->images);
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

    if (ImGui::BeginTable("IMAGES", 3, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("FILE", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_image_data *images = app->gui.data->images;

        for (size_t row = 0; row < arrlenu(images); ++row) {
            struct gui_image_data *img = &images[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &img->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", img->id);

            ImGui::TableNextColumn();
            ImGui::InputText("##uri", img->d.uri, SEUCK_MAX_RESOURCE_STR_LENGTH);
            ImGui::SameLine();
            if (ImGui::Button("...")) {
                // https://github.com/mlabbe/nativefiledialog
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
