#pragma once

#include "oxi.core.h"

#if defined(__cplusplus)
extern "C" {
#endif

enum oxi_bindconstant_kind {
    OXI_BINDCONSTANT_VEC4,
    OXI_BINDCONSTANT_VEC3,
    OXI_BINDCONSTANT_VEC2,
    OXI_BINDCONSTANT_FLOAT,
    OXI_BINDCONSTANT_BOOL,
    OXI_BINDCONSTANT_INT,
};

struct oxi_io_system;
struct oxi_shader_system;
struct oxi_shader_raw;
struct oxi_texture;
struct kinc_image;

struct oxi_shader_system_init_args {
    void *(*alloc)( size_t, void * );
    void *memctx;
    struct oxi_context *ctx;
};

struct oxi_shader_system *          oxi_shader_system_create( struct oxi_shader_system_init_args *cfg );
void                                oxi_shader_system_update( struct oxi_shader_system *sys );

// void                                oxi_shader_system_update_uniform( struct oxi_shader_system *sys, const char *link, );

oxi_id                              oxi_shader_create( struct oxi_shader_system *sys, struct oxi_shader_raw *raw );
struct kinc_g4_vertex_structure *   oxi_shader_get_structure( struct oxi_shader_system *sys, oxi_id idx );
void                                oxi_shader_bind( struct oxi_shader_system *sys, oxi_id shader_id );
void                                oxi_shader_use_texture( struct oxi_shader_system *sys, uint8_t slot, struct oxi_texture *tex );

struct oxi_shader_raw *             oxi_shader_raw_create( struct oxi_shader_system *sys );
void                                oxi_shader_raw_set_vertex_shader( struct oxi_shader_raw *raw, const char *uri );
void                                oxi_shader_raw_set_fragment_shader( struct oxi_shader_raw *raw, const char *uri );
void                                oxi_shader_raw_add_vertex_element( struct oxi_shader_raw *raw, const char *name, const char *data );
void                                oxi_shader_raw_add_constant_location( struct oxi_shader_raw *raw, const char *name, const char *type, const char *link );
void                                oxi_shader_raw_add_texture_unit( struct oxi_shader_raw *raw, const char *name );

#if defined(__cplusplus)
}
#endif
