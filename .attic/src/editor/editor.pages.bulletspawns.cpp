#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <kinc/graphics4/texture.h>
#include <stb_ds.h>

void draw_bulletspawns( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_bulletspawn_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->bulletspawns);
        arrput(app->gui.data->bulletspawns, d);
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE UP (FIRST)")) {
        for (size_t row = 0; row < arrlenu(app->gui.data->bulletspawns); ++row) {
            struct gui_bulletspawn_data *a = &app->gui.data->bulletspawns[row];

            if (a->selected) {
                if (row > 0) {
                    struct gui_bulletspawn_data d = app->gui.data->bulletspawns[row];
                    arrins(app->gui.data->bulletspawns, row - 1, d);
                    arrdel(app->gui.data->bulletspawns, row + 1);
                    break;
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE DOWN (FIRST)")) {
        size_t n = arrlenu(app->gui.data->bulletspawns);

        for (size_t row = 0; row < n; ++row) {
            struct gui_bulletspawn_data *a = &app->gui.data->bulletspawns[row];

            if (a->selected) {
                if (row < n - 1) {
                    struct gui_bulletspawn_data d = app->gui.data->bulletspawns[row];
                    arrins(app->gui.data->bulletspawns, row + 2, d);
                    arrdel(app->gui.data->bulletspawns, row);
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

    if (ImGui::BeginTable("bulletspawns", 6, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("REF");
        ImGui::TableSetupColumn("FLAGS");
        ImGui::TableSetupColumn("DX");
        ImGui::TableSetupColumn("DY");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_bulletspawn_data *bulletspawns = app->gui.data->bulletspawns;

        for (size_t row = 0; row < arrlenu(bulletspawns); ++row) {
            struct gui_bulletspawn_data *blt = &bulletspawns[row];

            ImGui::PushID(row);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Checkbox("##selected", &blt->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", blt->id);

            ImGui::TableNextColumn();
            bullet_combo(app, "##bullet", &blt->d.bullet_ref);

            ImGui::TableNextColumn();
            int flags = blt->d.flags;
            ImGui::InputInt("##flags", &flags);
            blt->d.flags = flags & 0xff;

            ImGui::TableNextColumn();
            int dx = blt->d.dir_x;
            ImGui::InputInt("##dx", &dx);
            blt->d.dir_x = dx & 0xff;

            ImGui::TableNextColumn();
            int dy = blt->d.dir_y;
            ImGui::InputInt("##dy", &dy);
            blt->d.dir_y = dy & 0xff;

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
