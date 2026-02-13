#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <kinc/graphics4/texture.h>
#include <stb_ds.h>

void draw_spawnpoints( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_spawnpoint_data d = { 0 };
        struct gui_spawnpoint_data *points = app->gui.data->spawnpoints;
        d.id = (int)arrlen(points);
        arrput(points, d);
    }

    ImGui::SameLine();

    if (ImGui::Button("DEL SELECTED ROWS")) {
        struct gui_spawnpoint_data *points = app->gui.data->spawnpoints;

        int i = arrlen(points) - 1;

        while (i >= 0) {
            if (points[i].selected) {
                arrdel(points, i);
            }

            i -= 1;
        }
    }

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

    if (ImGui::BeginTable("loadouts", 5, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("NAME");
        ImGui::TableSetupColumn("X");
        ImGui::TableSetupColumn("Y");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_spawnpoint_data *points = app->gui.data->spawnpoints;

        for (size_t row = 0; row < arrlenu(points); ++row) {
            struct gui_spawnpoint_data *p = &points[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &p->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", p->id);

            ImGui::TableNextColumn();
            ImGui::InputText("##name", p->name, sizeof(p->name));

            ImGui::TableNextColumn();
            int x = p->d.pos.x;
            ImGui::InputInt("##x", &x);
            p->d.pos.x = x;

            ImGui::TableNextColumn();
            int y = p->d.pos.y;
            ImGui::InputInt("##y", &y);
            p->d.pos.y = y;

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
