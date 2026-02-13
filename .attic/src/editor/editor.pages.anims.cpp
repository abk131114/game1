#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <stb_ds.h>

void draw_anims( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_anim_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->anims);
        arrput(app->gui.data->anims, d);

        struct gui_anim_cache_item c = { 0 };
        c.anim_ref = d.id;
        arrput(app->gui.res.anim_cache, c);
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE UP (FIRST)")) {
        for (size_t row = 0; row < arrlenu(app->gui.data->anims); ++row) {
            struct gui_anim_data *a = &app->gui.data->anims[row];

            if (a->selected) {
                if (row > 0) {
                    struct gui_anim_data d = app->gui.data->anims[row];
                    arrins(app->gui.data->anims, row - 1, d);
                    arrdel(app->gui.data->anims, row + 1);
                    break;
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE DOWN (FIRST)")) {
        size_t n = arrlenu(app->gui.data->anims);

        for (size_t row = 0; row < n; ++row) {
            struct gui_anim_data *a = &app->gui.data->anims[row];

            if (a->selected) {
                if (row < n - 1) {
                    struct gui_anim_data d = app->gui.data->anims[row];
                    arrins(app->gui.data->anims, row + 2, d);
                    arrdel(app->gui.data->anims, row);
                    break;
                }
            }
        }
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     gui_selected_items_delete(app->gui.data->anims);
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

    if (ImGui::BeginTable("anims", 6, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("START_FRAME_REF");
        ImGui::TableSetupColumn("END_FRAME_REF");
        ImGui::TableSetupColumn("REPEAT");
        ImGui::TableSetupColumn("PREVIEW");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_anim_data *anims = app->gui.data->anims;

        for (size_t row = 0; row < arrlenu(anims); ++row) {
            struct gui_anim_data *anm = &anims[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &anm->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", anm->id);

            ImGui::TableNextColumn();
            frame_combo(app, "##start", &anm->d.frames_start_ref);

            ImGui::TableNextColumn();
            frame_combo(app, "##end", &anm->d.frames_end_ref);

            ImGui::TableNextColumn();
            int r = anm->d.repeat;
            ImGui::InputInt("##repeat", &r);
            anm->d.repeat = r;

            ImGui::TableNextColumn();
            draw_anim(app, anm->id);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
