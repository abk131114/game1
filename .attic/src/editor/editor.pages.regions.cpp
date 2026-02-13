#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <kinc/graphics4/texture.h>
#include <stb_ds.h>

void draw_regions( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_region_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->regions);
        arrput(app->gui.data->regions, d);
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     // TODO (DK) update all frames first
    //     // gui_selected_items_delete(app->gui.data->regions);
    // }

    ImGui::SameLine();

    if (ImGui::Button("MOVE UP (FIRST)")) {
        for (size_t row = 0; row < arrlenu(app->gui.data->regions); ++row) {
            struct gui_region_data *r = &app->gui.data->regions[row];

            if (r->selected) {
                if (row > 0) {
                    struct gui_region_data d = app->gui.data->regions[row];
                    arrins(app->gui.data->regions, row - 1, d);
                    arrdel(app->gui.data->regions, row + 1);
                    break;
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE DOWN (FIRST)")) {
        size_t n = arrlenu(app->gui.data->regions);

        for (size_t row = 0; row < n; ++row) {
            struct gui_region_data *r = &app->gui.data->regions[row];

            if (r->selected) {
                if (row < n - 1) {
                    struct gui_region_data d = app->gui.data->regions[row];
                    arrins(app->gui.data->regions, row + 2, d);
                    arrdel(app->gui.data->regions, row);
                    break;
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("MAKE FRAMES")) {
        for (size_t row = 0; row < arrlenu(app->gui.data->regions); ++row) {
            struct gui_region_data *r = &app->gui.data->regions[row];

            if (r->selected) {
                r->selected = false;

                struct gui_frame_data f = { 0 };
                f.id = (int)arrlen(app->gui.data->frames);
                f.d.region_ref = r->id;
                f.d.duration = 2;
                arrput(app->gui.data->frames, f);
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

    if (ImGui::BeginTable("regions", 8, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("ATLAS");
        ImGui::TableSetupColumn("X");
        ImGui::TableSetupColumn("Y");
        ImGui::TableSetupColumn("W");
        ImGui::TableSetupColumn("H");
        ImGui::TableSetupColumn("PREVIEW");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_region_data *regions = app->gui.data->regions;

        for (size_t row = 0; row < arrlenu(regions); ++row) {
            struct gui_region_data *region = &regions[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &region->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", region->id);

            ImGui::TableNextColumn();
            image_combo(app, "##atlas", &region->d.atlas_ref);

            ImGui::TableNextColumn();
            int x = region->d.x;
            ImGui::InputInt("##x", &x);
            region->d.x = x;

            ImGui::TableNextColumn();
            int y = region->d.y;
            ImGui::InputInt("##y", &y);
            region->d.y = y;

            ImGui::TableNextColumn();
            int w = region->d.w;
            ImGui::InputInt("##w", &w);
            region->d.w = w;

            ImGui::TableNextColumn();
            int h = region->d.h;
            ImGui::InputInt("##h", &h);
            region->d.h = h;

            ImGui::TableNextColumn();
            struct kinc_g4_texture *texture = gui_texture_find(app, region->d.atlas_ref);

            if (texture == NULL || app->gui.opts.show_frames == 0) {
                ImGui::Text("");
            } else {
                float tw = texture->tex_width;
                float th = texture->tex_height;
                float u0 = x / tw;
                float v0 = y / th;
                float u1 = (x + w) / tw;
                float v1 = (y + h) / th;
                ImGui::Image(texture, ImVec2(w, h), ImVec2(u0, v0), ImVec2(u1, v1));
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
