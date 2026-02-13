#pragma once

#include "oxi.core.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct oxi_geometry_system_config {
	void *(*alloc)( size_t n, void *ctx );
	void (*free)( void *ptr, void *ctx );
    void *memctx;
};

struct oxi_geometry_system *          oxi_geometry_system_create( struct oxi_geometry_system_config *cfg );

// struct oxi_id oxi_geo_create_vertex_buffer( size_t )

// create index buffer
// create vertex buffer

#if defined(__cplusplus)
}
#endif
