#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <kinc/graphics4/texture.h>
#include <stb_ds.h>

void draw_frames( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_frame_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->frames);
        arrput(app->gui.data->frames, d);
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE UP (FIRST)")) {
        for (size_t row = 0; row < arrlenu(app->gui.data->frames); ++row) {
            struct gui_frame_data *f = &app->gui.data->frames[row];

            if (f->selected) {
                if (row > 0) {
                    struct gui_frame_data d = app->gui.data->frames[row];
                    arrins(app->gui.data->frames, row - 1, d);
                    arrdel(app->gui.data->frames, row + 1);
                    break;
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE DOWN (FIRST)")) {
        size_t n = arrlenu(app->gui.data->frames);

        for (size_t row = 0; row < n; ++row) {
            struct gui_frame_data *f = &app->gui.data->frames[row];

            if (f->selected) {
                if (row < n - 1) {
                    struct gui_frame_data d = app->gui.data->frames[row];
                    arrins(app->gui.data->frames, row + 2, d);
                    arrdel(app->gui.data->frames, row);
                    break;
                }
            }
        }
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     // TODO (DK) update all anims first
    //     // gui_selected_items_delete(app->gui.data->frames);
    // }

    ImGui::SameLine();

    if (ImGui::Button("MAKE ANIM")) {
        uint16_t start = 0xffff, end = 0xffff;

        for (size_t row = 0; row < arrlenu(app->gui.data->frames); ++row) {

            struct gui_frame_data *f = &app->gui.data->frames[row];

            if (f->selected) {
                f->selected = false;

                if (start == 0xffff) {
                    start = f->id;
                } else if (f->id < start) {
                    start = f->id;
                }

                if (end == 0xffff) {
                    end = f->id;
                } else if (f->id > end) {
                    end = f->id;
                }
            }
        }

        if (start != 0xffff && end != 0xffff) {
            struct gui_anim_data anm = { 0 };
            anm.id = (int)arrlen(app->gui.data->anims);
            anm.d.frames_start_ref = start;
            anm.d.frames_end_ref = end;
            anm.d.repeat = 0;
            arrput(app->gui.data->anims, anm);

			struct gui_anim_cache_item c = { 0 };
            c.anim_ref = anm.id;
			arrput(app->gui.res.anim_cache, c);
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

    if (ImGui::BeginTable("frames", 5, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("REGION_REF");
        ImGui::TableSetupColumn("DURATION");
        ImGui::TableSetupColumn("PREVIEW");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_frame_data *frames = app->gui.data->frames;

        for (size_t row = 0; row < arrlenu(frames); ++row) {
            struct gui_frame_data *frame = &frames[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &frame->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", frame->id);

            ImGui::TableNextColumn();
            region_combo(app, "##region", &frame->d.region_ref);

            ImGui::TableNextColumn();
            int d = frame->d.duration;
            ImGui::InputInt("##d", &d);
            frame->d.duration = d;

            ImGui::TableNextColumn();
            draw_region(app, frame->d.region_ref);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
