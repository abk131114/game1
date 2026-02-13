#include "oxi.shader-system.h"
#include "oxi.io.h"
#include "oxi.private.h"

#include <kinc/graphics4/graphics.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/shader.h>
#include <kinc/graphics4/texture.h>
#include <kinc/graphics4/vertexstructure.h>
#include <kinc/image.h>

#include <kinc/window.h> // TODO (DK) remove me?

#include <stb_ds.h>

struct oxi_shaderconstant_raw {
// @:structInit class TShaderConstant {
// 	public var name: String;
// 	public var type: String;
// 	@:optional public var link: String;
// 	@:optional public var vec4: Float32Array;
// 	@:optional public var vec3: Float32Array;
// 	@:optional public var vec2: Float32Array;
// 	@:optional public var float: Null<FastFloat>;
// 	@:optional public var bool: Null<Bool>;
// 	@:optional public var int: Null<Int>;
// 	@:optional public var is_arm_parameter: Null<Bool>;
    const char *name;
    const char *type;
    const char *link;
};

struct oxi_textureunit_raw {
// @:structInit class TTextureUnit {
// 	public var name: String;
// 	@:optional public var is_image: Null<Bool>; // image2D
// 	@:optional public var link: String;
// 	@:optional public var addressing_u: String;
// 	@:optional public var addressing_v: String;
// 	@:optional public var filter_min: String;
// 	@:optional public var filter_mag: String;
// 	@:optional public var mipmap_filter: String;
// 	@:optional public var default_image_file: String;
// 	@:optional public var is_arm_parameter: Null<Bool>;
    const char *name;
};

struct oxi_vertexelement_raw {
// @:structInit class TVertexElement {
// 	public var name: String;
// 	public var data: String; // "float4", "short2norm"
    const char *name;
    const char *data;
};

struct oxi_shader_raw {
    // const char *name;
    const char *vertex_shader; // add len?
    const char *fragment_shader; // add len?

    struct oxi_shaderconstant_raw *constant;//[8];
    struct oxi_textureunit_raw *texture_unit;//[8];
    struct oxi_vertexelement_raw *vertex_element;//[8];

    // uint8_t num_constants;
    // uint8_t num_textureunits;
    // uint8_t num_vertex_elements;

    kinc_g4_cull_mode_t cull_mode;

	kinc_g4_blending_factor_t blend_source;
	kinc_g4_blending_factor_t blend_destination;
	kinc_g4_blending_operation_t blend_operation;
	kinc_g4_blending_factor_t alpha_blend_source;
	kinc_g4_blending_factor_t alpha_blend_destination;
	kinc_g4_blending_operation_t alpha_blend_operation;

    bool depth_write;
};

struct oxi_shader {
    struct oxi_shader_raw *raw;
    kinc_g4_vertex_structure_t structure;
    kinc_g4_pipeline_t pipeline;
    kinc_g4_shader_t vertex_shader;
    kinc_g4_shader_t fragment_shader;
    kinc_g4_constant_location_t *constant; // [8]
    kinc_g4_texture_unit_t *texture_unit; // [8]
};

// struct cached_shader {
//     const char *key;
//     struct oxi_shader *value;
// };

struct loading_shader_resources {
    oxi_id shader_idx;
    oxi_id vertex_shader_res;
    oxi_id fragment_shader_res;
};

struct oxi_shader_system {
	void *(*alloc)( size_t n, void *ctx );
	void (*free)( void *ptr, void *ctx );
    void *memctx;
    struct oxi_context *ctx;

	oxi_id next_id;

    struct oxi_shader **shader_list;
    // struct cached_fragment_shader *fragment_shaders;
    // struct cached_shader *shader_cache;
    struct loading_shader_resources *loading_list;

    struct oxi_shader *current_shader; // only set after oxi_shader_bind()
};

struct oxi_shader_system *oxi_shader_system_create( struct oxi_shader_system_init_args *args ) {
    struct oxi_shader_system *sys = args->alloc(sizeof(*sys), args->memctx);
    memset(sys, 0, sizeof(*sys));
    sys->alloc = args->alloc;
    sys->memctx = args->memctx;
    sys->ctx = args->ctx;
    return sys;
}

static enum kinc_g4_vertex_data parse_vertexdata( const char *type ) {
    if (strcmp(type, "float1") == 0) {
        return KINC_G4_VERTEX_DATA_F32_1X;
    } else if (strcmp(type, "float2") == 0) {
        return KINC_G4_VERTEX_DATA_F32_2X;
    } else if (strcmp(type, "float3") == 0) {
        return KINC_G4_VERTEX_DATA_F32_3X;
    } else if (strcmp(type, "float4") == 0) {
        return KINC_G4_VERTEX_DATA_F32_4X;
    } else if (strcmp(type, "ubyte4norm") == 0) {
        return KINC_G4_VERTEX_DATA_U8_4X_NORMALIZED;
    } else if (strcmp(type, "ushort2norm") == 0) {
        return KINC_G4_VERTEX_DATA_U16_2X_NORMALIZED;
    }

		// else if (data == "short2norm") return VertexData.Short2Norm;
		// else if (data == "short4norm") return VertexData.Short4Norm;

    oxi_err("[oxi.shader.parse_vertexdata] unknown vertexdata <%s>", type);
    return KINC_G4_VERTEX_DATA_F32_1X;
}

static void init_shader( struct oxi_shader_system *sys, oxi_id shader_idx, void *vs, size_t vss, void *fs, size_t fss ) {
    struct oxi_shader *shader = sys->shader_list[shader_idx];

    kinc_g4_shader_init(&shader->vertex_shader, vs, vss, KINC_G4_SHADER_TYPE_VERTEX);
    kinc_g4_shader_init(&shader->fragment_shader, fs, fss, KINC_G4_SHADER_TYPE_FRAGMENT);

    struct kinc_g4_pipeline *p = &shader->pipeline;
    struct oxi_shader_raw *rs = shader->raw;
    kinc_g4_pipeline_init(p);

    p->input_layout[0] = &shader->structure;
    p->input_layout[1] = NULL;
    p->vertex_shader = &shader->vertex_shader;
    p->fragment_shader = &shader->fragment_shader;
    p->depth_mode = KINC_G4_COMPARE_LESS;
    p->depth_write = rs->depth_write;
    p->cull_mode = KINC_G4_CULL_NOTHING; // rs->cull_mode;
	p->blend_source = rs->blend_source;
	p->blend_destination = rs->blend_destination;
	p->blend_operation = rs->blend_operation;
	p->alpha_blend_source = rs->alpha_blend_source;
	p->alpha_blend_destination = rs->alpha_blend_destination;
	p->alpha_blend_operation = rs->alpha_blend_operation;

    kinc_g4_pipeline_compile(p);

    unsigned num_constants = arrlenu(rs->constant);

    for (unsigned i = 0; i < num_constants; ++i) {
        kinc_g4_constant_location_t loc = kinc_g4_pipeline_get_constant_location(p, rs->constant[i].name);
        arrput(shader->constant, loc);
    }

    unsigned num_units = arrlen(rs->texture_unit);

    for (unsigned i = 0; i < num_units; ++i) {
        kinc_g4_texture_unit_t unit = kinc_g4_pipeline_get_texture_unit(p, rs->texture_unit[i].name);
        arrput(shader->texture_unit, unit);
    }
}

void oxi_shader_system_update( struct oxi_shader_system *sys ) {
    int i = arrlen(sys->loading_list) - 1;

    while (i >= 0) {
        struct loading_shader_resources *res = &sys->loading_list[i];
        struct oxi_blob_data *vs = oxi_io_get_blob_data(sys->ctx->sys.io, res->vertex_shader_res);
        struct oxi_blob_data *fs = oxi_io_get_blob_data(sys->ctx->sys.io, res->fragment_shader_res);

        if (vs != NULL && fs != NULL) {
            init_shader(sys, res->shader_idx, vs->bytes, vs->size, fs->bytes, fs->size);
            arrdel(sys->loading_list, i);
        }

        i -= 1;
    }
}

oxi_id oxi_shader_create( struct oxi_shader_system *sys, struct oxi_shader_raw *raw_shader ) {
    // TODO (DK) check cache?
    struct oxi_shader *shader = sys->alloc(sizeof(*shader), sys->memctx);
    memset(shader, 0, sizeof(*shader));
    arrput(sys->shader_list, shader);
    oxi_id idx = arrlenu(sys->shader_list) - 1;
    shader->raw = raw_shader;

    struct loading_shader_resources loading = {
        .shader_idx = idx,
        .vertex_shader_res = oxi_io_load_blob(sys->ctx->sys.io, raw_shader->vertex_shader, strlen(raw_shader->vertex_shader)),
        .fragment_shader_res = oxi_io_load_blob(sys->ctx->sys.io, raw_shader->fragment_shader, strlen(raw_shader->fragment_shader)),
    };

    kinc_g4_vertex_structure_init(&shader->structure);

    for (unsigned i = 0; i < arrlenu(shader->raw->vertex_element); ++i) {
        struct oxi_vertexelement_raw *elem = &shader->raw->vertex_element[i];
        kinc_g4_vertex_structure_add(&shader->structure, elem->name, parse_vertexdata(elem->data));
    }

    arrput(sys->loading_list, loading);
    return idx;
}

struct kinc_g4_vertex_structure * oxi_shader_get_structure( struct oxi_shader_system *sys, oxi_id idx ) {
    unsigned num_shaders = arrlenu(sys->shader_list);

    if (idx >= num_shaders) {
        oxi_fatal("[oxi.shader-system.oxi_shader_get_structure] shader index %u out of range %u", idx, num_shaders);
        return NULL;
    }

    struct oxi_shader *shader = sys->shader_list[idx];
    return &shader->structure;
}

struct oxi_shader_raw *oxi_shader_raw_create( struct oxi_shader_system *sys ) {
    struct oxi_shader_raw *raw = sys->alloc(sizeof(*raw), sys->memctx);
    memset(raw, 0, sizeof(*raw));
// TODO (DK) remove me and add accessor functions
    raw->depth_write = false; // true;
    raw->cull_mode = KINC_G4_CULL_NOTHING;
	raw->blend_source = KINC_G4_BLEND_ONE;
	raw->blend_destination = KINC_G4_BLEND_INV_SOURCE_ALPHA;
	raw->blend_operation = KINC_G4_BLENDOP_ADD;
	raw->alpha_blend_source = KINC_G4_BLEND_ONE;
	raw->alpha_blend_destination = KINC_G4_BLEND_INV_SOURCE_ALPHA;
	raw->alpha_blend_operation = KINC_G4_BLENDOP_ADD;

    return raw;
}

void oxi_shader_raw_set_vertex_shader( struct oxi_shader_raw *raw, const char *uri ) {
    raw->vertex_shader = uri; // TODO (DK) strdup?
}

void oxi_shader_raw_set_fragment_shader( struct oxi_shader_raw *raw, const char *uri ) {
    raw->fragment_shader = uri; // TODO (DK) strdup?
}

void oxi_shader_raw_add_vertex_element( struct oxi_shader_raw *raw, const char *name, const char *data ) {
    struct oxi_vertexelement_raw elem = {
        .name = name,
        .data = data,
    };

    arrput(raw->vertex_element, elem);
}

void oxi_shader_raw_add_constant_location( struct oxi_shader_raw *raw, const char *name, const char *link, const char *type ) {
    struct oxi_shaderconstant_raw c = {
        .name = name,
        .link = link,
        .type = type,
    };

    arrput(raw->constant, c);
}

void oxi_shader_raw_add_texture_unit( struct oxi_shader_raw *raw, const char *name ) {
    struct oxi_textureunit_raw u = {
        .name = name,
    };

    arrput(raw->texture_unit, u);
}

void oxi_shader_bind( struct oxi_shader_system *sys, oxi_id shader_id ) {
    struct oxi_shader *shader = sys->shader_list[shader_id];
    kinc_g4_set_pipeline(&shader->pipeline);
    sys->current_shader = shader;

    float screen_width = sys->ctx->cfg.screen.width;
    float screen_height = sys->ctx->cfg.screen.height;
    unsigned num_constants = arrlenu(shader->constant);

    for (unsigned i = 0; i < num_constants; ++i) {
        struct kinc_g4_constant_location loc = shader->constant[i];
        struct oxi_shaderconstant_raw *raw = &shader->raw->constant[i];

        if (strcmp(raw->type, "_worldViewProjectionMatrix") == 0) {
            float left = 0.0f;
            float right = screen_width;
            float top = 0.0f;
            float  bottom = screen_height;
            float zn = 0.1f;
            float zf = 1000.0f;

            float tx = -(right + left) / (right - left);
            float ty = -(top + bottom) / (top - bottom);
            float tz = -(zf + zn) / (zf - zn);

            kinc_matrix4x4_t mvp = { .m = {
                2.0f / (right - left),  0.0f,                   0.0f,               0.0f,
                0.0f,                   2.0f / (top - bottom),  0.0f,               0.0f,
                0.0f,                   0.0f,                   -2.0f / (zf - zn),  0.0f,
                tx,                     ty,                     tz,                 1.0f
            } };

            kinc_g4_set_matrix4(loc, &mvp);
        }
    }
}

void oxi_shader_use_texture( struct oxi_shader_system *sys, uint8_t slot, struct oxi_texture *tex ) {
    struct oxi_shader *shader = sys->current_shader;

    if (shader == NULL) {
        oxi_fatal("[oxi.shader-system.oxi_shader_use_texture] no current shader");
        return;
    }

    struct kinc_g4_texture_unit unit = shader->texture_unit[slot];
    kinc_g4_set_texture(unit, &tex->impl);

    // struct oxi_textureunit_raw *raw = &shader->raw->texture_unit[slot];
    // TODO (DK) actual setting
    kinc_g4_set_texture_minification_filter(unit, KINC_G4_TEXTURE_FILTER_POINT);
    kinc_g4_set_texture_magnification_filter(unit, KINC_G4_TEXTURE_FILTER_POINT);
    kinc_g4_set_texture_addressing(unit, KINC_G4_TEXTURE_DIRECTION_U, KINC_G4_TEXTURE_ADDRESSING_CLAMP);
    kinc_g4_set_texture_addressing(unit, KINC_G4_TEXTURE_DIRECTION_V, KINC_G4_TEXTURE_ADDRESSING_CLAMP);
}
