#if defined(PX_DEV_UI)

#include "editor.data.h"
#include <oxi/oxi.core.h>

#include <stb_ds.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define VERSION_MAJOR 1
#define VERSION_MINOR 1
#define VERSION_PATCH 7

static const char *VERSION = "#version %i %i %i\n";
static const char *BLOBS_DESC = "\n#blobs %i\n";
static const char *BLOBS_HEADER = "id uri\n";
static const char *BLOBS_IN_ROW = "%i %s\n";
static const char *BLOBS_OUT_ROW = "%i %s\n";
static const char *IMAGES_DESC = "\n#images %i\n";
static const char *IMAGES_HEADER = "id uri\n";
static const char *IMAGES_IN_ROW = "%i %s\n";
static const char *IMAGES_OUT_ROW = "%i %s\n";
static const char *SOUNDS_DESC = "\n#sounds %i\n";
static const char *SOUNDS_HEADER = "id uri\n";
static const char *SOUNDS_IN_ROW = "%i %s\n";
static const char *SOUNDS_OUT_ROW = "%i %s\n";
static const char *LEVELS_DESC = "\n#levels %i\n";
static const char *LEVELS_HEADER = "id level_ref tileset_ref sprites_ref\n";
static const char *LEVELS_IN_ROW = "%i %"SCNu8" %"SCNu8" %"SCNu8"\n";
static const char *LEVELS_OUT_ROW = "%i %"PRIu8" %"PRIu8" %"PRIu8"\n";
static const char *REGIONS_DESC = "\n#regions %i\n";
static const char *REGIONS_HEADER = "id atlas_ref x y w h\n";
static const char *REGIONS_IN_ROW = "%i %"SCNu8" %"SCNu16" %"SCNu16" %"SCNu16" %"SCNu16"\n";
static const char *REGIONS_OUT_ROW = "%i %"PRIu8" %"PRIu16" %"PRIu16" %"PRIu16" %"PRIu16"\n";
static const char *FRAMES_DESC = "\n#frames %i\n";
static const char *FRAMES_HEADER = "id region_ref duration\n";
static const char *FRAMES_IN_ROW = "%i %"SCNu16" %"SCNu16"\n";
static const char *FRAMES_OUT_ROW = "%i %"PRIu16" %"PRIu16"\n";
static const char *ANIMS_DESC = "\n#anims %i\n";
static const char *ANIMS_HEADER = "id frames_start_ref frames_end_ref repeat\n";
static const char *ANIMS_IN_ROW = "%i %"SCNu16" %"SCNu16" %"SCNu8"\n";
static const char *ANIMS_OUT_ROW = "%i %"PRIu16" %"PRIu16" %"PRIu8"\n";
static const char *BULLETS_DESC = "\n#bullets %i\n";
static const char *BULLETS_HEADER = "id anim_ref damage kind kind_data\n";
static const char *BULLETS_IN_ROW = "%i %"SCNu16" %"SCNu8" %"SCNu8" %"SCNu8"\n";
static const char *BULLETS_OUT_ROW = "%i %"PRIu16" %"PRIu8" %"PRIu8" %"PRIu8"\n";
static const char *BULLETSPAWNS_DESC = "\n#bulletspawns %i\n";
static const char *BULLETSPAWNS_HEADER = "id bullet_ref dx dy flags\n";
static const char *BULLETSPAWNS_IN_ROW = "%i %"SCNu16" %"SCNu8" %"SCNu8" %"SCNu8"\n";
static const char *BULLETSPAWNS_OUT_ROW = "%i %"PRIu16" %"PRIu8" %"PRIu8" %"PRIu8"\n";
static const char *WLS_DESC = "\n#wls %i\n";
static const char *WLS_HEADER = "id cooldown end_ref key start_ref sound\n";
static const char *WLS_IN_ROW = "%i %"SCNu8" %"SCNu16" %"SCNu8" %"SCNu16" %"SCNu8"\n";
static const char *WLS_OUT_ROW = "%i %"PRIu8" %"PRIu16" %"PRIu8" %"PRIu16" %"PRIu8"\n";
static const char *LOADOUTS_DESC = "\n#loadouts %i\n";
static const char *LOADOUTS_HEADER = "id _0 _1 _2 _3,_4,_5,_6,_7,_8\n";
static const char *LOADOUTS_IN_ROW = "%i %"SCNu8" %"SCNu8" %"SCNu8" %"SCNu8" %"SCNu8" %"SCNu8" %"SCNu8" %"SCNu8" %"SCNu8"\n";
static const char *LOADOUTS_OUT_ROW = "%i %"PRIu8" %"PRIu8" %"PRIu8" %"PRIu8" %"PRIu8" %"PRIu8" %"PRIu8" %"PRIu8" %"PRIu8"\n";
static const char *SHIPS_DESC = "\n#ships %i\n";
static const char *SHIPS_HEADER = "id faction flydown_anim_ref flyup_anim_ref idle_anim_ref inital_loadout_ref max_loadout_ref\n";
static const char *SHIPS_IN_ROW = "%i %"SCNu16" %"SCNu16" %"SCNu16" %"SCNu8" %"SCNu8" %"SCNu8"\n";
static const char *SHIPS_OUT_ROW = "%i %"PRIu16" %"PRIu16" %"PRIu16" %"PRIu8" %"PRIu8" %"PRIu8"\n";
static const char *ENEMIES_DESC = "\n#enemies %i\n";
static const char *ENEMIES_HEADER = "id anim_ref hp score\n";
static const char *ENEMIES_IN_ROW = "%i %"SCNu16" %"SCNu16" %"SCNu16"\n";
static const char *ENEMIES_OUT_ROW = "%i %"PRIu16" %"PRIu16" %"PRIu16"\n";
static const char *SPAWNPOINTS_DESC = "\n#spawnpoints %i\n";
static const char *SPAWNPOINTS_HEADER = "id x y\n";
static const char *SPAWNPOINTS_IN_ROW = "%i %"SCNu16" %"SCNu16"\n";
static const char *SPAWNPOINTS_OUT_ROW = "%i %"PRIu16" %"PRIu16"\n";
static const char *WAYPOINTS_DESC = "\n#waypoints %i\n";
static const char *WAYPOINTS_HEADER = "id x y\n";
static const char *WAYPOINTS_IN_ROW = "%i %"SCNu16" %"SCNu16"\n";
static const char *WAYPOINTS_OUT_ROW = "%i %"PRIu16" %"PRIu16"\n";
static const char *PATHS_DESC = "\n#paths %i\n";
static const char *PATHS_HEADER = "id from_ref to_ref\n";
static const char *PATHS_IN_ROW = "%i %"SCNu16" %"SCNu16"\n";
static const char *PATHS_OUT_ROW = "%i %"PRIu16" %"PRIu16"\n";
static const char *WAVESPAWNS_DESC = "\n#wavespawns %i\n";
static const char *WAVESPAWNS_HEADER = "id delay enemy_ref path_ref spawnpoint_ref\n";
static const char *WAVESPAWNS_IN_ROW = "%i %"SCNu8" %"SCNu16" %"SCNu8" %"SCNu8"\n";
static const char *WAVESPAWNS_OUT_ROW = "%i %"PRIu8" %"PRIu16" %"PRIu8" %"PRIu8"\n";
static const char *WAVES_DESC = "\n#waves %i\n";
static const char *WAVES_HEADER = "id ox oy from_ref to_ref\n";
static const char *WAVES_IN_ROW = "%i %"SCNu16" %"SCNu16" %"SCNu16" %"SCNu16"\n";
static const char *WAVES_OUT_ROW = "%i %"PRIu16" %"PRIu16" %"PRIu16" %"PRIu16"\n";

static bool version_ge( struct gui_version_data *version, int major, int minor, int patch ) {
    if (version->major >= major) {
        if (version->minor >= minor) {
            if (version->patch >= patch) {
                return true;
            }
        }
    }

    return false;
}

#define readline() fgets(line, 2048, f)

#define BEGIN_IN_SECTION(Desc, Type) \
    { \
        int num = 0; \
        readline(); \
        sscanf(line, Desc, &num); \
        readline();  \
        \
        for (int i = 0; i < num; ++i) { \
            struct Type d = { 0 }; \
            readline();

#define END_IN_SECTION() \
        } \
        \
        readline(); \
    }

struct gui_data	*gui_data_load( const char *uri ) {
    struct gui_data *data = malloc(sizeof(*data));
    memset(data, 0, sizeof(*data));

    FILE *f = fopen(uri, "rt");

    if (f == NULL) {
        oxi_err("failed to open '%s' for reading", uri);
        return NULL;
    }

    char line[2048];
    int major = 0, minor = 0, patch = 0;
    readline();
    sscanf(line, VERSION, &major, &minor, &patch);
    readline();

    BEGIN_IN_SECTION(BLOBS_DESC, gui_blob_data)
        sscanf(line, BLOBS_IN_ROW, &d.id, d.d.uri);
        arrput(data->blobs, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(IMAGES_DESC, gui_image_data)
        sscanf(line, IMAGES_IN_ROW, &d.id, d.d.uri);
        arrput(data->images, d);
    END_IN_SECTION()

    if (major >= 1 && minor >= 1 && patch >= 7) {
        BEGIN_IN_SECTION(SOUNDS_DESC, gui_sound_data)
            sscanf(line, SOUNDS_IN_ROW, &d.id, d.d.uri);
            arrput(data->sounds, d);
        END_IN_SECTION()
    }

    BEGIN_IN_SECTION(LEVELS_DESC, gui_level_data)
        sscanf(line, LEVELS_IN_ROW, &d.id, &d.d.level_ref, &d.d.tileset_ref, &d.d.sprites_ref);
        arrput(data->levels, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(REGIONS_DESC, gui_region_data)
        sscanf(line, REGIONS_IN_ROW, &d.id, &d.d.atlas_ref, &d.d.x, &d.d.y, &d.d.w, &d.d.h);
        arrput(data->regions, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(FRAMES_DESC, gui_frame_data)
        sscanf(line, FRAMES_IN_ROW, &d.id, &d.d.region_ref, &d.d.duration);
        arrput(data->frames, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(ANIMS_DESC, gui_anim_data)
        sscanf(line, ANIMS_IN_ROW, &d.id, &d.d.frames_start_ref, &d.d.frames_end_ref, &d.d.repeat);
        arrput(data->anims, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(BULLETS_DESC, gui_bullet_data)
        sscanf(line, BULLETS_IN_ROW, &d.id, &d.d.anim_ref, &d.d.damage, &d.d.kind, &d.d.kind_data);
        arrput(data->bullets, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(BULLETSPAWNS_DESC, gui_bulletspawn_data)
        sscanf(line, BULLETSPAWNS_IN_ROW, &d.id, &d.d.bullet_ref, &d.d.dir_x, &d.d.dir_y, &d.d.flags);
        arrput(data->bulletspawns, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(WLS_DESC, gui_weaponlevelshot_data)
        sscanf(line, WLS_IN_ROW, &d.id, &d.d.cooldown, &d.d.end_ref, &d.d.key, &d.d.start_ref, &d.d.sound);
        arrput(data->wls, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(LOADOUTS_DESC, gui_loadout_data)
        sscanf(line, LOADOUTS_IN_ROW, &d.id,
            &d.d.level[0], &d.d.level[1], &d.d.level[2],
            &d.d.level[3], &d.d.level[4], &d.d.level[5],
            &d.d.level[6], &d.d.level[7], &d.d.level[8]
        );
        arrput(data->loadouts, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(SHIPS_DESC, gui_ship_data)
        sscanf(line, SHIPS_IN_ROW, &d.id, &d.d.faction,
            &d.d.flydown_anim_ref, &d.d.flyup_anim_ref, &d.d.idle_anim_ref,
            &d.d.initial_loadout_ref, &d.d.maximum_loadout_ref);
        arrput(data->ships, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(ENEMIES_DESC, gui_enemy_data_v2)
        sscanf(line, ENEMIES_IN_ROW, &d.id, &d.d.anim_ref, &d.d.hp, &d.d.score);
        arrput(data->enemies, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(SPAWNPOINTS_DESC, gui_spawnpoint_data)
        sscanf(line, SPAWNPOINTS_IN_ROW, &d.id, &d.d.pos.x, &d.d.pos.y);
        arrput(data->spawnpoints, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(WAYPOINTS_DESC, gui_waypoint_data)
        sscanf(line, WAYPOINTS_IN_ROW, &d.id, &d.d.pos.x, &d.d.pos.y);
        arrput(data->waypoints, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(PATHS_DESC, gui_path_data)
        sscanf(line, PATHS_IN_ROW, &d.id, &d.d.from_ref, &d.d.to_ref);
        arrput(data->paths, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(WAVESPAWNS_DESC, gui_wavespawn_data)
        sscanf(line, WAVESPAWNS_IN_ROW, &d.id, &d.d.delay, &d.d.enemy_ref, &d.d.path_ref, &d.d.spawnpoint_ref);
        arrput(data->wavespawns, d);
    END_IN_SECTION()

    BEGIN_IN_SECTION(WAVES_DESC, gui_wave_data)
        sscanf(line, WAVES_IN_ROW, &d.id, &d.d.offset_x, &d.d.offset_y, &d.d.spawn_ref_from, &d.d.spawn_ref_to);
        arrput(data->waves, d);
    END_IN_SECTION()

    fclose(f);
    return data;
}

#define BEGIN_OUT_SECTION(Pattern, Field, Type) \
    fprintf(f, Pattern##_DESC, arrlen(Field)); \
    fprintf(f, Pattern##_HEADER); \
    for (int i = 0; i < arrlen(Field); ++i) { \
        struct Type *row = &Field[i];

#define END_OUT_SECTION() \
    }

void gui_data_save( const char *uri, struct gui_data *data ) {
    FILE *f = fopen(uri, "wt");

    if (f == NULL) {
        oxi_err("failed to open '%s' for writing", uri);
        return;
    }

    fprintf(f, VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    BEGIN_OUT_SECTION(BLOBS, data->blobs, gui_blob_data);
        fprintf(f, BLOBS_OUT_ROW, row->id, row->d.uri);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(IMAGES, data->images, gui_image_data);
        fprintf(f, IMAGES_OUT_ROW, row->id, row->d.uri);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(SOUNDS, data->sounds, gui_sound_data);
        fprintf(f, SOUNDS_OUT_ROW, row->id, row->d.uri);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(LEVELS, data->levels, gui_level_data);
        fprintf(f, LEVELS_OUT_ROW, row->id, row->d.level_ref, row->d.tileset_ref, row->d.sprites_ref);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(REGIONS, data->regions, gui_region_data);
        fprintf(f, REGIONS_OUT_ROW, row->id, row->d.atlas_ref, row->d.x, row->d.y, row->d.w, row->d.h);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(FRAMES, data->frames, gui_frame_data);
        fprintf(f, FRAMES_OUT_ROW, row->id, row->d.region_ref, row->d.duration);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(ANIMS, data->anims, gui_anim_data);
        fprintf(f, ANIMS_OUT_ROW, row->id, row->d.frames_start_ref, row->d.frames_end_ref, row->d.repeat);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(BULLETS, data->bullets, gui_bullet_data);
        fprintf(f, BULLETS_OUT_ROW, row->id, row->d.anim_ref, row->d.damage, row->d.kind, row->d.kind_data);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(BULLETSPAWNS, data->bulletspawns, gui_bulletspawn_data);
        fprintf(f, BULLETSPAWNS_OUT_ROW, row->id, row->d.bullet_ref, row->d.dir_x, row->d.dir_y, row->d.flags);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(WLS, data->wls, gui_weaponlevelshot_data);
        fprintf(f, WLS_OUT_ROW, row->id, row->d.cooldown, row->d.end_ref, row->d.key, row->d.start_ref, row->d.sound);
    END_OUT_SECTION()

    assert(MAX_PX_POWERUP_KIND == 9);
    BEGIN_OUT_SECTION(LOADOUTS, data->loadouts, gui_loadout_data);
        fprintf(f, LOADOUTS_OUT_ROW, row->id,
            row->d.level[0], row->d.level[1], row->d.level[2],
            row->d.level[3], row->d.level[4], row->d.level[5],
            row->d.level[6], row->d.level[7], row->d.level[8]);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(SHIPS, data->ships, gui_ship_data);
        fprintf(f, SHIPS_OUT_ROW, row->id,
            row->d.faction, row->d.flydown_anim_ref, row->d.flyup_anim_ref, row->d.idle_anim_ref,
            row->d.initial_loadout_ref, row->d.maximum_loadout_ref
        );
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(ENEMIES, data->enemies, gui_enemy_data_v2);
        fprintf(f, ENEMIES_OUT_ROW, row->id, row->d.anim_ref, row->d.hp, row->d.score);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(SPAWNPOINTS, data->spawnpoints, gui_spawnpoint_data);
        fprintf(f, SPAWNPOINTS_OUT_ROW, row->id, row->d.pos.x, row->d.pos.y);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(WAYPOINTS, data->waypoints, gui_waypoint_data);
        fprintf(f, WAYPOINTS_OUT_ROW, row->id, row->d.pos.x, row->d.pos.y);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(PATHS, data->paths, gui_path_data);
        fprintf(f, PATHS_OUT_ROW, row->id, row->d.from_ref, row->d.to_ref);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(WAVESPAWNS, data->wavespawns, gui_wavespawn_data);
        fprintf(f, WAVESPAWNS_OUT_ROW, row->id, row->d.delay, row->d.enemy_ref, row->d.path_ref, row->d.spawnpoint_ref);
    END_OUT_SECTION()

    BEGIN_OUT_SECTION(WAVES, data->waves, gui_wave_data);
        fprintf(f, WAVES_OUT_ROW, row->id, row->d.offset_x, row->d.offset_y, row->d.spawn_ref_from, row->d.spawn_ref_to);
    END_OUT_SECTION()

    fclose(f);
}

void gui_data_export( const char *uri, struct gui_data *data ) {
    FILE *f = fopen(uri, "wb");

    if (f == NULL) {
        oxi_err("failed to open '%s' for writing", uri);
        return;
    }

    uint16_t num_blobs = arrlenu(data->blobs);
    uint16_t num_images = arrlenu(data->images);
    uint16_t num_sounds = arrlenu(data->sounds);
    uint16_t num_levels = arrlenu(data->levels);
    uint16_t num_regions = arrlenu(data->regions);
    uint16_t num_frames = arrlenu(data->frames);
    uint16_t num_anims = arrlenu(data->anims);
    uint16_t num_bullets = arrlenu(data->bullets);
    uint16_t num_bulletspawns = arrlenu(data->bulletspawns);
    uint16_t num_wls = arrlenu(data->wls);
    uint16_t num_loadouts = arrlenu(data->loadouts);
    uint16_t num_ships = arrlenu(data->ships);
    uint16_t num_enemies = arrlenu(data->enemies);
    uint16_t num_spawnpoints = arrlenu(data->spawnpoints);
    uint16_t num_waypoints = arrlenu(data->waypoints);
    uint16_t num_paths = arrlenu(data->paths);
    uint16_t num_wavespawns = arrlenu(data->wavespawns);
    uint16_t num_waves = arrlenu(data->waves);

    struct gui_version_data version = {
        .major = VERSION_MAJOR,
        .minor = VERSION_MINOR,
        .patch = VERSION_PATCH,
    };

    fwrite(&version, sizeof(version), 1, f);
    uint8_t padding = 0;
    fwrite(&padding, sizeof(padding), 1, f);

    fwrite(&num_blobs, sizeof(num_blobs), 1, f);
    fwrite(&num_images, sizeof(num_images), 1, f);
    fwrite(&num_sounds, sizeof(num_sounds), 1, f);
    fwrite(&num_levels, sizeof(num_levels), 1, f);
    fwrite(&num_regions, sizeof(num_regions), 1, f);
    fwrite(&num_frames, sizeof(num_frames), 1, f);
    fwrite(&num_anims, sizeof(num_anims), 1, f);
    fwrite(&num_bullets, sizeof(num_bullets), 1, f);
    fwrite(&num_bulletspawns, sizeof(num_bulletspawns), 1, f);
    fwrite(&num_wls, sizeof(num_wls), 1, f);
    fwrite(&num_loadouts, sizeof(num_loadouts), 1, f);
    fwrite(&num_ships, sizeof(num_ships), 1, f);
    fwrite(&num_enemies, sizeof(num_enemies), 1, f);
    fwrite(&num_spawnpoints, sizeof(num_spawnpoints), 1, f);
    fwrite(&num_waypoints, sizeof(num_waypoints), 1, f);
    fwrite(&num_paths, sizeof(num_paths), 1, f);
    fwrite(&num_wavespawns, sizeof(num_wavespawns), 1, f);
    fwrite(&num_waves, sizeof(num_waves), 1, f);

    uint16_t size_blobs = sizeof(data->blobs[0].d);
    uint16_t size_images = sizeof(data->images[0].d);
    uint16_t size_sounds = sizeof(data->sounds[0].d);
    uint16_t size_levels = sizeof(data->levels[0].d);
    uint16_t size_regions = sizeof(data->regions[0].d);
    uint16_t size_frames = sizeof(data->frames[0].d);
    uint16_t size_anims = sizeof(data->anims[0].d);
    uint16_t size_bullets = sizeof(data->bullets[0].d);
    uint16_t size_bulletspawns = sizeof(data->bulletspawns[0].d);
    uint16_t size_wls = sizeof(data->wls[0].d);
    uint16_t size_loadouts = sizeof(data->loadouts[0].d);
    uint16_t size_ships = sizeof(data->ships[0].d);
    uint16_t size_enemies = sizeof(data->enemies[0].d);
    uint16_t size_spawnpoints = sizeof(data->spawnpoints[0].d);
    uint16_t size_waypoints = sizeof(data->waypoints[0].d);
    uint16_t size_paths = sizeof(data->paths[0].d);
    uint16_t size_wavespawns = sizeof(data->wavespawns[0].d);
    uint16_t size_waves = sizeof(data->waves[0].d);

    fwrite(&size_blobs, sizeof(size_blobs), 1, f);
    fwrite(&size_images, sizeof(size_images), 1, f);
    fwrite(&size_sounds, sizeof(size_sounds), 1, f);
    fwrite(&size_levels, sizeof(size_levels), 1, f);
    fwrite(&size_regions, sizeof(size_regions), 1, f);
    fwrite(&size_frames, sizeof(size_frames), 1, f);
    fwrite(&size_anims, sizeof(size_anims), 1, f);
    fwrite(&size_bullets, sizeof(size_bullets), 1, f);
    fwrite(&size_bulletspawns, sizeof(size_bulletspawns), 1, f);
    fwrite(&size_wls, sizeof(size_wls), 1, f);
    fwrite(&size_loadouts, sizeof(size_loadouts), 1, f);
    fwrite(&size_ships, sizeof(size_ships), 1, f);
    fwrite(&size_enemies, sizeof(size_enemies), 1, f);
    fwrite(&size_spawnpoints, sizeof(size_spawnpoints), 1, f);
    fwrite(&size_waypoints, sizeof(size_waypoints), 1, f);
    fwrite(&size_paths, sizeof(size_paths), 1, f);
    fwrite(&size_wavespawns, sizeof(size_wavespawns), 1, f);
    fwrite(&size_waves, sizeof(size_waves), 1, f);

    for (size_t i = 0, n = num_blobs; i < n; ++i) {
        fwrite(&data->blobs[i].d, sizeof(data->blobs[i].d), 1, f);
    }

    for (size_t i = 0, n = num_images; i < n; ++i) {
        fwrite(&data->images[i].d, sizeof(data->images[i].d), 1, f);
    }

    for (size_t i = 0, n = num_sounds; i < n; ++i) {
        fwrite(&data->sounds[i].d, sizeof(data->sounds[i].d), 1, f);
    }

    for (size_t i = 0, n = num_levels; i < n; ++i) {
        fwrite(&data->levels[i].d, sizeof(data->levels[i].d), 1, f);
    }

    for (size_t i = 0, n = num_regions; i < n; ++i) {
        fwrite(&data->regions[i].d, sizeof(data->regions[i].d), 1, f);
    }

    for (size_t i = 0, n = num_frames; i < n; ++i) {
        fwrite(&data->frames[i].d, sizeof(data->frames[i].d), 1, f);
    }

    for (size_t i = 0, n = num_anims; i < n; ++i) {
        fwrite(&data->anims[i].d, sizeof(data->anims[i].d), 1, f);
    }

    for (size_t i = 0, n = num_bullets; i < n; ++i) {
        fwrite(&data->bullets[i].d, sizeof(data->bullets[i].d), 1, f);
    }

    for (size_t i = 0, n = num_bulletspawns; i < n; ++i) {
        fwrite(&data->bulletspawns[i].d, sizeof(data->bulletspawns[i].d), 1, f);
    }

    for (size_t i = 0, n = num_wls; i < n; ++i) {
        fwrite(&data->wls[i].d, sizeof(data->wls[i].d), 1, f);
    }

    for (size_t i = 0, n = num_loadouts; i < n; ++i) {
        fwrite(&data->loadouts[i].d, sizeof(data->loadouts[i].d), 1, f);
    }

    for (size_t i = 0, n = num_ships; i < n; ++i) {
        fwrite(&data->ships[i].d, sizeof(data->ships[i].d), 1, f);
    }

    for (size_t i = 0, n = num_enemies; i < n; ++i) {
        fwrite(&data->enemies[i].d, sizeof(data->enemies[i].d), 1, f);
    }

    for (size_t i = 0, n = num_spawnpoints; i < n; ++i) {
        fwrite(&data->spawnpoints[i].d, sizeof(data->spawnpoints[i].d), 1, f);
    }

    for (size_t i = 0, n = num_waypoints; i < n; ++i) {
        fwrite(&data->waypoints[i].d, sizeof(data->waypoints[i].d), 1, f);
    }

    for (size_t i = 0, n = num_paths; i < n; ++i) {
        fwrite(&data->paths[i].d, sizeof(data->paths[i].d), 1, f);
    }

    for (size_t i = 0, n = num_wavespawns; i < n; ++i) {
        fwrite(&data->wavespawns[i].d, sizeof(data->wavespawns[i].d), 1, f);
    }

    for (size_t i = 0, n = num_waves; i < n; ++i) {
        fwrite(&data->waves[i].d, sizeof(data->waves[i].d), 1, f);
    }

    fclose(f);
}

static void reindex_blobs( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->blobs); ++i) {
        struct gui_blob_data *b = &data->blobs[i];
        b->id = i;
    }
}

static void reindex_images( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->images); ++i) {
        struct gui_image_data *b = &data->images[i];
        b->id = i;
    }
}

static void reindex_sounds( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->sounds); ++i) {
        struct gui_sound_data *b = &data->sounds[i];
        b->id = i;
    }
}

static void reindex_ships( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->ships); ++i) {
        struct gui_ship_data *ship = &data->ships[i];
        ship->d.flydown_anim_ref = gui_anim_find_index(data, ship->d.flydown_anim_ref);
        ship->d.flyup_anim_ref = gui_anim_find_index(data, ship->d.flyup_anim_ref);
        ship->d.idle_anim_ref = gui_anim_find_index(data, ship->d.idle_anim_ref);
        ship->d.initial_loadout_ref = gui_loadout_find_index(data, ship->d.initial_loadout_ref);
        ship->d.maximum_loadout_ref = gui_loadout_find_index(data, ship->d.maximum_loadout_ref);
        ship->id = i;
    }
}

static void reindex_regions( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->regions); ++i) {
        struct gui_region_data *r = &data->regions[i];
        r->d.atlas_ref = gui_image_find_index(data, r->d.atlas_ref);
        r->id = i;
    }
}

static void reindex_frames( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->frames); ++i) {
        struct gui_frame_data *frm = &data->frames[i];
        frm->d.region_ref = gui_region_find_index(data, frm->d.region_ref);
        frm->id = i;
    }
}

static void reindex_anims( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->anims); ++i) {
        struct gui_anim_data *anm = &data->anims[i];
        anm->d.frames_start_ref = gui_frame_find_index(data, anm->d.frames_start_ref);
        anm->d.frames_end_ref = gui_frame_find_index(data, anm->d.frames_end_ref);
        anm->id = i;
    }
}

static void reindex_loadouts( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->loadouts); ++i) {
        struct gui_loadout_data *l = &data->loadouts[i];
        l->id = i;
    }
}

static void reindex_spawnpoints( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->spawnpoints); ++i) {
        struct gui_spawnpoint_data *l = &data->spawnpoints[i];
        l->id = i;
    }
}

static void reindex_waypoints( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->waypoints); ++i) {
        struct gui_waypoint_data *l = &data->waypoints[i];
        l->id = i;
    }
}

static void reindex_paths( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->paths); ++i) {
        struct gui_path_data *l = &data->paths[i];
        l->id = i;
    }
}

static void reindex_wls( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->wls); ++i) {
        struct gui_weaponlevelshot_data *item = &data->wls[i];
        item->d.start_ref = gui_bulletspawn_find_index(data, item->d.start_ref);
        item->d.end_ref = gui_bulletspawn_find_index(data, item->d.end_ref);
        item->d.sound.ref = gui_sound_find_index(data, item->d.sound.ref);
        item->id = i;
    }
}

static void reindex_bulletspawns( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->bulletspawns); ++i) {
        struct gui_bulletspawn_data *bs = &data->bulletspawns[i];
        bs->d.bullet_ref = gui_bullet_find_index(data, bs->d.bullet_ref);
        bs->id = i;
    }
}

static void reindex_bullets( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->bullets); ++i) {
        struct gui_bullet_data *b = &data->bullets[i];
        b->d.anim_ref = gui_anim_find_index(data, b->d.anim_ref);
        b->id = i;
    }
}

static void reindex_levels( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->levels); ++i) {
        struct gui_level_data *l = &data->levels[i];
        l->d.level_ref = gui_blob_find_index(data, l->d.level_ref);
        l->d.sprites_ref = gui_image_find_index(data, l->d.sprites_ref);
        l->d.tileset_ref = gui_image_find_index(data, l->d.tileset_ref);
        l->id = i;
    }
}

static void reindex_wavespawns( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->wavespawns); ++i) {
        struct gui_wavespawn_data *w = &data->wavespawns[i];
        w->d.enemy_ref = gui_enemy_find_index(data, w->d.enemy_ref);
        w->d.path_ref = gui_path_find_index(data, w->d.path_ref);
        w->d.spawnpoint_ref = gui_spawnpoint_find_index(data, w->d.spawnpoint_ref);
        w->id = i;
    }
}

static void reindex_enemies( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->enemies); ++i) {
        struct gui_enemy_data_v2 *e = &data->enemies[i];
        e->d.anim_ref = gui_anim_find_index(data, e->d.anim_ref);
        e->id = i;
    }
}

static void reindex_waves( struct gui_data *data ) {
    for (size_t i = 0; i < arrlenu(data->waves); ++i) {
        struct gui_wave_data *w = &data->waves[i];
        w->d.spawn_ref_from = gui_wavespawn_find_index(data, w->d.spawn_ref_from);
        w->d.spawn_ref_to = gui_wavespawn_find_index(data, w->d.spawn_ref_to);
        w->id = i;
    }
}

void gui_data_reindex( struct gui_data *data ) {
    // tables rueckwaerts durchgehen wegen dependencies
    // index fuer ref-wert finden und eigenen ref wert ersetzen
    // eigene id mit index ersetzen

    reindex_waves(data);
    reindex_wavespawns(data);
    reindex_enemies(data);
    reindex_ships(data);
    reindex_wls(data);
    reindex_bulletspawns(data);
    reindex_bullets(data);

    reindex_anims(data);
    reindex_frames(data);
    reindex_regions(data);

    reindex_levels(data);

    reindex_sounds(data);
    reindex_images(data);
    reindex_blobs(data);
    reindex_loadouts(data);
    reindex_paths(data);
    reindex_spawnpoints(data);
    reindex_waypoints(data);
}

#define DEFINE_FIND_FUNCTIONS(Name, Type, Array) \
    uint16_t gui_##Name##_find_index( struct gui_data *data, uint16_t id ) { \
        for (uint16_t i = 0; i < arrlenu(data->Array); ++i) { \
            if (data->Array[i].id == id) { \
                return i; \
            } \
        } \
\
        return 0xffff; \
    } \
\
    struct Type *gui_##Name##_find( struct gui_data *data, uint16_t id ) { \
        uint16_t idx = gui_##Name##_find_index(data, id); \
        return idx == 0xffff ? NULL : &data->Array[idx]; \
    }

DEFINE_FIND_FUNCTIONS(blob, gui_blob_data, blobs)
DEFINE_FIND_FUNCTIONS(image, gui_image_data, images)
DEFINE_FIND_FUNCTIONS(sound, gui_sound_data, sounds)
DEFINE_FIND_FUNCTIONS(region, gui_region_data, regions)
DEFINE_FIND_FUNCTIONS(frame, gui_frame_data, frames)
DEFINE_FIND_FUNCTIONS(anim, gui_anim_data, anims)
DEFINE_FIND_FUNCTIONS(bullet, gui_bullet_data, bullets)
DEFINE_FIND_FUNCTIONS(bulletspawn, gui_bulletspawn_data, bulletspawns)
DEFINE_FIND_FUNCTIONS(loadout, gui_loadout_data, loadouts)
DEFINE_FIND_FUNCTIONS(enemy, gui_enemy_data_v2, enemies)
DEFINE_FIND_FUNCTIONS(spawnpoint, gui_spawnpoint_data, spawnpoints)
DEFINE_FIND_FUNCTIONS(waypoint, gui_waypoint_data, waypoints)
DEFINE_FIND_FUNCTIONS(path, gui_path_data, paths)
DEFINE_FIND_FUNCTIONS(wavespawn, gui_wavespawn_data, wavespawns)
DEFINE_FIND_FUNCTIONS(wave, gui_wave_data, waves)

#endif
