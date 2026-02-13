#if defined(PX_DEV_UI)

#include "../app.h"
#include "editor.controls.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <kinc/graphics4/texture.h>
#include <stb_ds.h>

void draw_loadouts( struct app_context *app ) {
    if (ImGui::Button("ADD ROW")) {
        struct gui_loadout_data d = { 0 };
        d.id = (int)arrlen(app->gui.data->loadouts);
        arrput(app->gui.data->loadouts, d);
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

    if (ImGui::BeginTable("loadouts", 11, flags)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 16);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort, 16);
        ImGui::TableSetupColumn("SPEED");
        ImGui::TableSetupColumn("GUNS");
        ImGui::TableSetupColumn("BUILDER");
        ImGui::TableSetupColumn("SIDE");
        ImGui::TableSetupColumn("MISSILE");
        ImGui::TableSetupColumn("PLASMA");
        ImGui::TableSetupColumn("MAGMA");
        ImGui::TableSetupColumn("LASER");
        ImGui::TableSetupColumn("STEALTH");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        struct gui_loadout_data *loadouts = app->gui.data->loadouts;

        for (size_t row = 0; row < arrlenu(loadouts); ++row) {
            struct gui_loadout_data *lo = &loadouts[row];

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushID(row);
            ImGui::Checkbox("##selected", &lo->selected);

            ImGui::TableNextColumn();
            ImGui::Text("%i", lo->id);

            int v;

            for (int i = 0; i < MAX_PX_POWERUP_KIND; ++i) {
                ImGui::TableNextColumn();
                v = lo->d.level[i];
                char lbl[32] = { 0 };
                snprintf(lbl, 32, "##%i", i);
                ImGui::InputInt(lbl, &v);
                lo->d.level[i] = v & 0xff;
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

#endif
