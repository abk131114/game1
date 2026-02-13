#include "oxi.texture-system.h"
#include "oxi.private.h"

struct oxi_texture_system {
	void *(*alloc)( size_t n, void *ctx );
	void (*free)( void *ptr, void *ctx );
    void *memctx;
};

struct oxi_texture_system *oxi_texture_system_create( struct oxi_texture_system_config *cfg ) {
    struct oxi_texture_system *sys = cfg->alloc(sizeof(*sys), cfg->memctx);
    memset(sys, 0, sizeof(*sys));
    sys->alloc = cfg->alloc;
    sys->memctx = cfg->memctx;
    return sys;
}

struct oxi_texture *oxi_texture_from_image( struct oxi_texture_system *sys, struct kinc_image *img ) {
    OXI_ASSERT(img != NULL && "img is NULL");

    if (img == NULL) {
        return NULL;
    }

    struct oxi_texture *tex = sys->alloc(sizeof(*tex), sys->memctx);
    kinc_g4_texture_init_from_image(&tex->impl, img);
    return tex;
}

int oxi_texture_width( struct oxi_texture *t ) {
    return t->impl.tex_width;
}

int oxi_texture_height( struct oxi_texture *t ) {
    return t->impl.tex_height;
}
