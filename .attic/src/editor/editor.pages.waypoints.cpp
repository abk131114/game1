#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <kinc/graphics4/texture.h>
#include <stb_ds.h>

void draw_waypoints( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_waypoint_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->waypoints);
        arrput(app->gui.data->waypoints, d);
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

    if (ImGui::BeginTable("waypoints", 4, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("X");
        ImGui::TableSetupColumn("Y");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_waypoint_data *points = app->gui.data->waypoints;

        for (size_t row = 0; row < arrlenu(points); ++row) {
            struct gui_waypoint_data *p = &points[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &p->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", p->id);

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
