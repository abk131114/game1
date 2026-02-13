#pragma once

#include "oxi.core.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct oxi_spritebatcher;
struct oxi_texture;

size_t  oxi_spritebatcher_alloc_size( uint16_t max_sprites );
void    oxi_spritebatcher_init( struct oxi_spritebatcher *b, struct oxi_context *ctx, uint16_t max_sprites );
void    oxi_spritebatcher_begin( struct oxi_spritebatcher *b );

void    oxi_spritebatcher_add_px_tint(
    struct oxi_spritebatcher *b, struct oxi_texture *image,
    uint16_t sx, uint16_t sy, uint16_t sw, uint16_t sh,
    float x, float y, float z, float w, float h,
    struct oxi_color color
);

void    oxi_spritebatcher_add_uv_tint(
    struct oxi_spritebatcher *b, struct oxi_texture *image,
    float sx, float sy, float sw, float sh,
    float x, float y, float z, float w, float h,
    struct oxi_color color
);

void    oxi_spritebatcher_flush( struct oxi_spritebatcher *b, int clear );
void    oxi_spritebatcher_end( struct oxi_spritebatcher *b );

#if defined(__cplusplus)
}
#endif
