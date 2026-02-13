#include "oxi.spritebatcher.h"
#include "oxi.shader-system.h"
#include "oxi.texture-system.h"

#include <kinc/graphics4/graphics.h>
#include <kinc/graphics4/indexbuffer.h>
#include <kinc/graphics4/texture.h>
#include <kinc/graphics4/vertexbuffer.h>
#include <kinc/graphics4/vertexstructure.h>

// struct sprite {
//     struct oxi_texture *tex;
//     float x, y, z, w, h, sx, sy, sw, sh;
// };

struct oxi_spritebatcher {
    struct kinc_g4_vertex_buffer vertex_buffer;
    struct kinc_g4_index_buffer index_buffer;
    uint16_t max_sprites, num_sprites, start;

    struct oxi_context *ctx;
    oxi_id shader;

    struct oxi_texture *last_tex;

    float *vertices;
};

size_t oxi_spritebatcher_alloc_size( uint16_t max_sprites ) {
    size_t n = sizeof(struct oxi_spritebatcher); // + max_sprites * sizeof(struct sprite);
    return n;
}

void oxi_spritebatcher_init( struct oxi_spritebatcher *b, struct oxi_context *ctx, uint16_t max_sprites ) {
    b->max_sprites = max_sprites;
    b->num_sprites = 0;
    b->start = 0;
    b->last_tex = NULL;
    b->ctx = ctx;

    // TODO (DK) load from some file
    struct oxi_shader_raw *raw_shader = oxi_shader_raw_create(ctx->sys.shaders);
    oxi_shader_raw_set_vertex_shader(raw_shader, "oxi-sprite-batcher-tint.vert");
    oxi_shader_raw_set_fragment_shader(raw_shader, "oxi-sprite-batcher-tint.frag");
    oxi_shader_raw_add_vertex_element(raw_shader, "v_pos", "float3");
    oxi_shader_raw_add_vertex_element(raw_shader, "v_uv", "float2");
    oxi_shader_raw_add_vertex_element(raw_shader, "v_col", "ubyte4norm");
    oxi_shader_raw_add_constant_location(raw_shader, "u_mvp", "mat4", "_worldViewProjectionMatrix");
    // oxi_shader_raw_add_constant_location(raw_shader, "u_modulate", "vec4", "_modulate_color");
    oxi_shader_raw_add_texture_unit(raw_shader, "u_sampler");

    // raw_shader->depth_write = true;
    // raw_shader->cull_mode = KINC_G4_CULL_NOTHING;
	// raw_shader->blend_source = KINC_G4_BLEND_ONE;
	// raw_shader->blend_destination = KINC_G4_BLEND_INV_SOURCE_ALPHA;
	// raw_shader->blend_operation = KINC_G4_BLENDOP_ADD;
	// raw_shader->alpha_blend_source = KINC_G4_BLEND_ONE;
	// raw_shader->alpha_blend_destination = KINC_G4_BLEND_INV_SOURCE_ALPHA;
	// raw_shader->alpha_blend_operation = KINC_G4_BLENDOP_ADD;

    b->shader = oxi_shader_create(b->ctx->sys.shaders, raw_shader);

    kinc_g4_vertex_buffer_init(&b->vertex_buffer, b->max_sprites * 4, oxi_shader_get_structure(b->ctx->sys.shaders, b->shader),  KINC_G4_USAGE_DYNAMIC, 0);
//     // batch->vertices = kinc_g4_vertex_buffer_lock(&batch->vertex_buffer, batch->start, 0);
    b->vertices = kinc_g4_vertex_buffer_lock_all(&b->vertex_buffer);

    if (max_sprites * 6 >= UINT16_MAX) {
        oxi_fatal("[oxi.spritebatcher.oxi_spritebatcher_init] can not fit indices into 16bit");
        return;
    }

    kinc_g4_index_buffer_init(&b->index_buffer, b->max_sprites * 6, KINC_G4_INDEX_BUFFER_FORMAT_16BIT, KINC_G4_USAGE_STATIC);

    uint16_t *indices = kinc_g4_index_buffer_lock_all(&b->index_buffer);

    for (uint16_t i = 0; i < max_sprites; ++i) {
        indices[i * 3 * 2 + 0] = i * 4 + 0;
        indices[i * 3 * 2 + 1] = i * 4 + 1;
        indices[i * 3 * 2 + 2] = i * 4 + 2;
        indices[i * 3 * 2 + 3] = i * 4 + 0;
        indices[i * 3 * 2 + 4] = i * 4 + 2;
        indices[i * 3 * 2 + 5] = i * 4 + 3;
    }

    kinc_g4_index_buffer_unlock_all(&b->index_buffer);
}

void oxi_spritebatcher_begin( struct oxi_spritebatcher *b ) {
}

void oxi_spritebatcher_flush( struct oxi_spritebatcher *b, int clear ) {
    if (b->num_sprites == 0) {
        return;
    }

    if (b->last_tex == NULL) {
        return;
    }

    kinc_g4_vertex_buffer_unlock(&b->vertex_buffer, b->num_sprites * 4);

    oxi_shader_bind(b->ctx->sys.shaders, b->shader);
    oxi_shader_use_texture(b->ctx->sys.shaders, 0, b->last_tex);
    kinc_g4_set_vertex_buffer(&b->vertex_buffer);
    kinc_g4_set_index_buffer(&b->index_buffer);
    kinc_g4_draw_indexed_vertices_from_to(b->start * 6, b->num_sprites * 6);

    // if (clear > 0 || b->start + b->num_sprites >= b->max_sprites) {
        b->start = 0;
        b->num_sprites = 0;
    // }

    // b->vertices = kinc_g4_vertex_buffer_lock(&b->vertex_buffer, b->start * 4, 0);
    b->vertices = kinc_g4_vertex_buffer_lock_all(&b->vertex_buffer);
}

void oxi_spritebatcher_end( struct oxi_spritebatcher *b ) {
    oxi_spritebatcher_flush(b, 1);
}

void oxi_spritebatcher_add_px_tint(
    struct oxi_spritebatcher *b,
    struct oxi_texture *tex,
    uint16_t sx, uint16_t sy, uint16_t sw, uint16_t sh,
    float x, float y, float z, float w, float h,
    struct oxi_color color
) {
    float tw = oxi_texture_width(tex);
    float th = oxi_texture_height(tex);
    oxi_spritebatcher_add_uv_tint(b, tex, (float)sx / tw, (float)sy / th, (float)sw / tw, (float)sh / th, x, y, z, w, h, color);
}

void oxi_spritebatcher_add_uv_tint(
    struct oxi_spritebatcher *b,
    struct oxi_texture *tex,
    float sx, float sy, float sw, float sh,
    float x, float y, float z, float w, float h,
    struct oxi_color color
) {
    if (b->start + b->num_sprites + 1 >= b->max_sprites) {
        oxi_spritebatcher_flush(b, 0);
    }

    if (b->last_tex != tex) {
        oxi_spritebatcher_flush(b, 0);
    }

    b->last_tex = tex;

    int num_vertex_elements = 6;
    int idx = (b->start + b->num_sprites) * (num_vertex_elements * 4);
    float *verts = b->vertices;

    // TODO (DK) use SIMD

    // bottom left
    verts[idx++] = x;
    verts[idx++] = y + h;
    verts[idx++] = z;
    verts[idx++] = sx;
    verts[idx++] = sy + sh;
    verts[idx++] = *(float *)&color;

    // top left
    verts[idx++] = x;
    verts[idx++] = y;
    verts[idx++] = z;
    verts[idx++] = sx;
    verts[idx++] = sy;
    verts[idx++] = *(float *)&color;

    // top right
    verts[idx++] = x + w;
    verts[idx++] = y;
    verts[idx++] = z;
    verts[idx++] = sx + sw;
    verts[idx++] = sy;
    verts[idx++] = *(float *)&color;

    // bottom right
    verts[idx++] = x + w;
    verts[idx++] = y + h;
    verts[idx++] = z;
    verts[idx++] = sx + sw;
    verts[idx++] = sy + sh;
    verts[idx++] = *(float *)&color;

    b->num_sprites += 1;
}
