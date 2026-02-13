#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <implot.h>

#include <stb_ds.h>

void draw_paths( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_path_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->paths);
        arrput(app->gui.data->paths, d);
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     gui_selected_items_delete(app->gui.data->paths);
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

    if (ImGui::BeginTable("paths", 5, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("FROM");
        ImGui::TableSetupColumn("TO");
        ImGui::TableSetupColumn("PATH");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_path_data *paths = app->gui.data->paths;

        for (size_t row = 0; row < arrlenu(paths); ++row) {
            struct gui_path_data *path = &paths[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &path->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", path->id);

            ImGui::TableNextColumn();
            waypoint_combo(app, "##from", &path->d.from_ref);

            ImGui::TableNextColumn();
            waypoint_combo(app, "##to", &path->d.to_ref);

            static float x_data[1024] = {};
            static float y_data[1024] = {};

            int num_waypoints = path->d.to_ref - path->d.from_ref + 1;

            for (int wpi = 0; wpi < num_waypoints; ++wpi) {
                int wpid = path->d.from_ref + wpi;
                struct gui_waypoint_data *wp = gui_waypoint_find(app->gui.data, wpid);
                x_data[wpi] = wp->d.pos.x;
                y_data[wpi] = wp->d.pos.y;
            }

            ImGui::TableNextColumn();

            static float h[] = {-100, 100};
            static float v[] = {-160, 160};

            if (ImPlot::BeginPlot("path", ImVec2(), ImPlotFlags_Equal)) {
                ImPlot::SetupAxes(NULL, NULL, 0, 0);
                ImPlot::PlotInfLines("Horizontal", h, 2, ImPlotInfLinesFlags_Horizontal);
                ImPlot::PlotInfLines("Vertical", v, 2);
                ImPlot::PlotLine("My Line", x_data, y_data, num_waypoints);
                ImPlot::EndPlot();
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
