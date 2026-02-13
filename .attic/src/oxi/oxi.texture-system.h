#pragma once

#include "oxi.core.h"

struct kinc_image;
struct oxi_texture;

#if defined(__cplusplus)
extern "C" {
#endif

struct oxi_texture_system_config {
	void *(*alloc)( size_t n, void *ctx );
	void (*free)( void *ptr, void *ctx );
  void *memctx;
};

struct oxi_texture_system * oxi_texture_system_create( struct oxi_texture_system_config *cfg );
struct oxi_texture *    		oxi_texture_from_image( struct oxi_texture_system *sys, struct kinc_image *img );
int                     		oxi_texture_width( struct oxi_texture *t );
int                     		oxi_texture_height( struct oxi_texture *t );

// struct oxi_id oxi_geo_create_vertex_buffer( size_t )

// create index buffer
// create vertex buffer

#if defined(__cplusplus)
}
#endif
