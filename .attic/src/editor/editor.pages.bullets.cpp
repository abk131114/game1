#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <kinc/graphics4/texture.h>
#include <stb_ds.h>

void draw_bullets( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_bullet_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->bullets);
        arrput(app->gui.data->bullets, d);
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE UP (FIRST)")) {
        for (size_t row = 0; row < arrlenu(app->gui.data->bullets); ++row) {
            struct gui_bullet_data *a = &app->gui.data->bullets[row];

            if (a->selected) {
                if (row > 0) {
                    struct gui_bullet_data d = app->gui.data->bullets[row];
                    arrins(app->gui.data->bullets, row - 1, d);
                    arrdel(app->gui.data->bullets, row + 1);
                    break;
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("MOVE DOWN (FIRST)")) {
        size_t n = arrlenu(app->gui.data->bullets);

        for (size_t row = 0; row < n; ++row) {
            struct gui_bullet_data *a = &app->gui.data->bullets[row];

            if (a->selected) {
                if (row < n - 1) {
                    struct gui_bullet_data d = app->gui.data->bullets[row];
                    arrins(app->gui.data->bullets, row + 2, d);
                    arrdel(app->gui.data->bullets, row);
                    break;
                }
            }
        }
    }

    // ImGui::SameLine();

    // if (ImGui::Button("DELETE SELECTED")) {
    //     gui_selected_items_delete(app->gui.data->bullets);
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

    if (ImGui::BeginTable("bullets", 7, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("KIND");
        ImGui::TableSetupColumn("KIND DATA");
        ImGui::TableSetupColumn("DAMAGE");
        ImGui::TableSetupColumn("ANIM REF");
        ImGui::TableSetupColumn("ANIM PREVIEW");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_bullet_data *bullets = app->gui.data->bullets;

        for (size_t row = 0; row < arrlenu(bullets); ++row) {
            struct gui_bullet_data *blt = &bullets[row];

            ImGui::PushID(row);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Checkbox("##selected", &blt->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", blt->id);

            ImGui::TableNextColumn();
            bulletkind_combo(app, "##kind", &blt->d.kind);

            ImGui::TableNextColumn();
            int kind_data = blt->d.kind_data;
            ImGui::InputInt("##kind_data", &kind_data);
            blt->d.kind_data = kind_data & 0xff;

            ImGui::TableNextColumn();
            int dmg = blt->d.damage;
            ImGui::InputInt("##damage", &dmg);
            blt->d.damage = dmg & 0xff;

            ImGui::TableNextColumn();
            anim_combo(app, "##anim", &blt->d.anim_ref);

            ImGui::TableNextColumn();
            draw_anim(app, blt->d.anim_ref);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
