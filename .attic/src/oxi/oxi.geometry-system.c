#include "oxi.geometry-system.h"

#include <kinc/graphics4/indexbuffer.h>
#include <kinc/graphics4/vertexbuffer.h>

struct oxi_geometry_system {
	void *(*alloc)( size_t n, void *ctx );
	void (*free)( void *ptr, void *ctx );
    void *memctx;
};

struct oxi_geometry_system *oxi_geometry_system_create( struct oxi_geometry_system_config *cfg ) {
    struct oxi_geometry_system *sys = cfg->alloc(sizeof(*sys), cfg->memctx);
    memset(sys, 0, sizeof(*sys));
    sys->alloc = cfg->alloc;
    sys->memctx = cfg->memctx;
    return sys;
}
