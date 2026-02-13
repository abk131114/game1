#include "oxi.core.h"

#include "oxi.geometry-system.h"
#include "oxi.input.h"
#include "oxi.io.h"
#include "oxi.shader-system.h"
#include "oxi.texture-system.h"

#include <kinc/audio1/audio.h>
#include <kinc/audio1/sound.h>
#include <kinc/audio1/soundstream.h>

#include <kinc/log.h>
#include <kinc/math/random.h>
#include <kinc/system.h>

#include <stdlib.h>

void oxi_init( struct oxi_context *ctx, struct oxi_init_args *args ) {
	kinc_random_init(42); // TODO (DK) real seed
	kinc_a1_init();

	ctx->cfg = *args;

    // g->sys.geometry = oxi_geometry_system_create(&(struct oxi_geometry_system_config){
    //     .alloc = oxi_alloc,
    //     .memctx = ctx,
    // });

    ctx->sys.input = oxi_alloc(oxi_input_system_alloc_size(), NULL);
    oxi_input_system_init(ctx->sys.input);

    ctx->sys.io = oxi_io_system_create(oxi_alloc, ctx);

    ctx->sys.shaders = oxi_shader_system_create(&(struct oxi_shader_system_init_args){
        .alloc = oxi_alloc,
        .memctx = ctx,
        .ctx = ctx,
    });

    ctx->sys.textures = oxi_texture_system_create(&(struct oxi_texture_system_config){
        .alloc = oxi_alloc,
        .memctx = ctx,
        // .io = g->sys.io,
    });
}

void oxi_trace( const char *format, ... ) {
	va_list args;
	va_start(args, format);
	kinc_log_args(KINC_LOG_LEVEL_INFO, format, args);
	va_end(args);
}

void oxi_warn( const char *format, ... ) {
	va_list args;
	va_start(args, format);
	kinc_log_args(KINC_LOG_LEVEL_WARNING, format, args);
	va_end(args);
}

void oxi_err( const char *format, ... ) {
	va_list args;
	va_start(args, format);
	kinc_log_args(KINC_LOG_LEVEL_ERROR, format, args);
	va_end(args);

    kinc_debug_break();
}

void oxi_fatal( const char *format, ... ) {
	va_list args;
	va_start(args, format);
	kinc_log_args(KINC_LOG_LEVEL_INFO, format, args);
	va_end(args);

    kinc_debug_break();
}

void *oxi_alloc( size_t bytes, void *ctx ) {
	(void) ctx;
	return malloc(bytes);
}

void  oxi_free( void *ptr, void *ctx ) {
	(void) ctx;
	free(ptr);
}

float oxi_random_0_1_f( void ) {
    return (float)kinc_random_get() / (float)(INT64_MAX / 1.0f);
}

int oxi_random_range_inc_i( int min, int max ) {
    return kinc_random_get_in(min, max - 1);
}

int32_t oxi_clamp_i32( int32_t v, int32_t min, int32_t max ) {
	return v < min ? min : v > max ? max : v;
}

struct oxi_color oxi_rgba( unsigned int color ) {
    return (struct oxi_color){ .impl = {.rgba = color } };
}

struct oxi_color oxi_rgba_c( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) {
    return (struct oxi_color){ .impl = { .components = { .r = r, .g = g, .b = b, .a = a }}};
}
