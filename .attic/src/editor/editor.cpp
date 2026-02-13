#if defined(PX_DEV_UI)

#include <px/px.core.h> // TODO (DK) remove me

#include "../app.h"

#include <imgui.h>
#include <imgui-kinc.h>
#include <imgui-kinc-g4.h>
#include <implot.h>
#include <kid/kid.h>
#include <kinc/graphics4/texture.h>
#include <kinc/system.h>
#include <kinc/window.h>
#include <oxi/oxi.io.h>
#include <stb_ds.h>
#include <stdlib.h>

static void load_data( struct app_context *app ) {
	struct gui_data *new_data = gui_data_load(app->gui.project_path);

	if (new_data != NULL) {
		app->gui.data = new_data;

		size_t num_textures = arrlenu(app->gui.data->images);
		arrsetlen(app->gui.res.texture_cache, num_textures);

		for (size_t i = 0; i < num_textures; ++i) {
			struct gui_image_data *img = &app->gui.data->images[i];
			app->gui.res.texture_cache[i].id = oxi_io_load_image(app->px.oxi.sys.io, img->d.uri, strlen(img->d.uri));
			app->gui.res.texture_cache[i].atlas_ref = i;
			app->gui.res.texture_cache[i].texture = NULL;
		}

		for (size_t i = 0, n = arrlenu(app->gui.data->anims); i < n; ++i) {
			struct gui_anim_cache_item c = { 0 };
			c.anim_ref = app->gui.data->anims[i].id;
			arrput(app->gui.res.anim_cache, c);
		}
	}
}

static void * load_module_data( const char *filename ) {
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);
	void *data = malloc(length);
	fread(data, length, 1, f);
	fclose(f);
	return data;
}

static enum kid_handler_state inject_key_down( int code, void *userdata ) {
	imgui_kinc_inject_key_down(code, userdata);
	return kid_handler_state_continue;
}

static enum kid_handler_state inject_key_up( int code, void *userdata ) {
	imgui_kinc_inject_key_up(code, userdata);
	return kid_handler_state_continue;
}

static enum kid_handler_state inject_key_press( unsigned character, void *userdata) {
	imgui_kinc_inject_key_press(character, userdata);
	return kid_handler_state_continue;
}

static enum kid_handler_state inject_mouse_down( int window, int button, int x, int y, void *userdata ) {
	imgui_kinc_inject_mouse_down(window, button, x, y, userdata);
	return kid_handler_state_continue;
}

static enum kid_handler_state inject_mouse_up( int window, int button, int x, int y, void *userdata ) {
	imgui_kinc_inject_mouse_up(window, button, x, y, userdata);
	return kid_handler_state_continue;
}

static enum kid_handler_state inject_mouse_move( int window, int x, int y, int dx, int dy, void *userdata ) {
	imgui_kinc_inject_mouse_move(window, x, y, dx, dy, userdata);
	return kid_handler_state_continue;
}

static enum kid_handler_state inject_mouse_scroll( int window, int delta, void *userdata ) {
	imgui_kinc_inject_mouse_scroll(window, delta, userdata);
	return kid_handler_state_continue;
}

void gui_init( struct app_context *app, int kid_ctx_idx ) {
    app->gui.visible = true;

	ImGui::CreateContext(NULL);
	ImGui::StyleColorsDark(NULL);
	ImPlot::CreateContext();
	imgui_kinc_init(&app->gui.ctx, 0, false);
	imgui_kinc_g4_init(0);

	struct kid_context *kid = &app->kid;
	struct imgui_kinc_context *gui = &app->gui.ctx;
    kid_register_key_down_handler(kid, kid_ctx_idx, inject_key_down, gui);
    kid_register_key_up_handler(kid, kid_ctx_idx, inject_key_up, gui);
    kid_register_key_press_handler(kid, kid_ctx_idx, inject_key_press, gui);
    kid_register_mouse_down_handler(kid, kid_ctx_idx, inject_mouse_down, gui);
    kid_register_mouse_up_handler(kid, kid_ctx_idx, inject_mouse_up, gui);
    kid_register_mouse_move_handler(kid, kid_ctx_idx, inject_mouse_move, gui);
    kid_register_mouse_wheel_handler(kid, kid_ctx_idx, inject_mouse_scroll, gui);

	load_data(app);

	if (app->gui.data == NULL) {
		app->gui.data = (gui_data*)malloc(sizeof(*app->gui.data));
		memset(app->gui.data, 0, sizeof(*app->gui.data));
	}
}

void gui_shutdown( struct app_context *app ) {
	imgui_kinc_g4_shutdown();
	imgui_kinc_shutdown(&app->gui.ctx);
	ImGui::DestroyContext(NULL);
}

static void update_texture_cache( struct app_context *app ) {
	for (size_t i = 0; i < arrlenu(app->gui.res.texture_cache); ++i) {
		struct gui_texture_cache_item *item = &app->gui.res.texture_cache[i];

		// TODO (DK) use an additional "loading" array
		if (item->texture != NULL) {
			continue;
		}

		struct oxi_image_data *data = oxi_io_find_image_data(app->px.oxi.sys.io, item->id);

		if (data == NULL) {
			continue;
		}

		item->texture = (struct kinc_g4_texture*)malloc(sizeof(*item->texture));
		kinc_g4_texture_init_from_image(item->texture, &data->image);
	}
}

extern void draw_blobs( struct app_context *app );
extern void draw_images( struct app_context *app );
extern void draw_sounds( struct app_context *app );
extern void draw_levels( struct app_context *app );
extern void draw_regions( struct app_context *app );
extern void draw_frames( struct app_context *app );
extern void draw_anims( struct app_context *app );
extern void draw_bullets( struct app_context *app );
extern void draw_bulletspawns( struct app_context *app );
extern void draw_wls( struct app_context *app );
extern void draw_loadouts( struct app_context *app );
extern void draw_ships( struct app_context *app );
extern void draw_enemies( struct app_context *app );
extern void draw_spawnpoints( struct app_context *app );
extern void draw_waypoints( struct app_context *app );
extern void draw_paths( struct app_context *app );
extern void draw_wavespawns( struct app_context *app );
extern void draw_waves( struct app_context *app );

static void draw_ui( struct app_context *app ) {
	int wnd = app->gui.ctx.window;
	int w = kinc_window_width(wnd);
	int h = kinc_window_height(wnd);

	for (size_t i = 0, n = arrlenu(app->gui.res.anim_cache); i < n; ++i) {
		struct gui_anim_cache_item *c = &app->gui.res.anim_cache[i];
		struct gui_anim_data *anm = &app->gui.data->anims[i];
		struct gui_frame_data *frame = &app->gui.data->frames[c->frame];

		c->counter -= 1;

		if (c->counter == -1) {
			c->frame += 1;

			// we don't handle repeat in the ui, otherwise we would need a way to restart the anims
			if (c->frame > anm->d.frames_end_ref) {
				c->frame = anm->d.frames_start_ref;
				frame = &app->gui.data->frames[c->frame];
			}

			c->counter = frame->d.duration;
		}
	}

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(w, h));
	ImGui::Begin("Data", NULL, 0);

	if (ImGui::Button("LOAD DATA")) {
		load_data(app);
	}

	ImGui::SameLine();

	if (ImGui::Button("SAVE DATA")) {
		gui_data_save(app->gui.project_path, app->gui.data);
	}

	ImGui::SameLine();

	if (ImGui::Button("EXPORT (AND SAVE) DATA")) {
		gui_data_save(app->gui.project_path, app->gui.data);
		gui_data_export(app->gui.export_path, app->gui.data);

		void *data = load_module_data(app->gui.export_path); // TODO (DK) refactor: use the oxi_io_system?

		if (data != NULL) {
			struct px_context *px = &app->px;
			oxi_free(px->data, NULL); // TODO (DK) refactor: use oxi_io_system?
			px->data = (struct px_data *)data;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("REINDEX DATA")) {
		gui_data_reindex(app->gui.data);
	}

    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("BLOBS", NULL, 0)) {
			draw_blobs(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("IMAGES", NULL, 0)) {
			draw_images(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("SOUNDS", NULL, 0)) {
			draw_sounds(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("LEVELS", NULL, 0)) {
			draw_levels(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("REGIONS", NULL, 0)) {
			draw_regions(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("FRAMES", NULL, 0)) {
			draw_frames(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("ANIMS", NULL, 0)) {
			draw_anims(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("BULLETS", NULL, 0)) {
			draw_bullets(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("BULLETSPAWNS", NULL, 0)) {
			draw_bulletspawns(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("WEAPONLEVELSHOTS", NULL, 0)) {
			draw_wls(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("LOADOUTS", NULL, 0)) {
			draw_loadouts(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("SHIPS", NULL, 0)) {
			draw_ships(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("ENEMIES", NULL, 0)) {
			draw_enemies(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("SPAWNPOINTS", NULL, 0)) {
			draw_spawnpoints(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("WAYPOINTS", NULL, 0)) {
			draw_waypoints(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("PATHS", NULL, 0)) {
			draw_paths(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("WAVESPAWNS", NULL, 0)) {
			draw_wavespawns(app);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("WAVES", NULL, 0)) {
			draw_waves(app);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void gui_update( struct app_context *app ) {
	kinc_ticks_t start_ticks = kinc_timestamp();
	imgui_kinc_g4_new_frame();
	imgui_kinc_new_frame(&app->gui.ctx, 0);

	update_texture_cache(app);

	ImGui::NewFrame();
	draw_ui(app);
	// ImGui::ShowDemoWindow(NULL);
	ImGui::Render();
	kinc_ticks_t end_ticks = kinc_timestamp();
	app->metrics.gui_tick = end_ticks - start_ticks;
}

void gui_draw( struct app_context *app ) {
	kinc_ticks_t start_ticks = kinc_timestamp();
	imgui_kinc_g4_render_drawdata(ImGui::GetDrawData());
	kinc_ticks_t end_ticks = kinc_timestamp();
	app->metrics.gui_render = end_ticks - start_ticks;
}

struct kinc_g4_texture *gui_texture_find( struct app_context *app, uint16_t atlas_ref ) {
    for (uint16_t i = 0; i < arrlenu(app->gui.res.texture_cache); ++i) {
        if (app->gui.res.texture_cache[i].atlas_ref == atlas_ref) {
            return app->gui.res.texture_cache[i].texture;
        }
    }

    return NULL;
}

struct gui_anim_cache_item *gui_anim_cache_item_find( struct app_context *app, uint16_t id ) {
    for (uint16_t i = 0; i < arrlenu(app->gui.res.anim_cache); ++i) {
        if (app->gui.res.anim_cache[i].anim_ref == id) {
            return &app->gui.res.anim_cache[i];
        }
    }

    return NULL;

}

#endif
