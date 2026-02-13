#pragma once

#include "oxi.core.h"
#include <kinc/audio1/sound.h>
#include <kinc/image.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct oxi_io_system;

struct oxi_blob_data {
	void *bytes;
	size_t size;
};

struct oxi_image_data {
	kinc_image_t image;
	bool readable;
};

struct oxi_sound_data {
	struct kinc_a1_sound *a1;
	// size_t size;
	// unsigned int channels;
	// int sample_rate;
	// float *samples;
	// float length;
	// uint8_t *compressed_samples;
};

#define OXI_IO_STR_ARG(Str) Str, strlen(Str)

struct oxi_io_system *  oxi_io_system_create( void *(*alloc)( size_t, void * ), void *ctx );
oxi_id                  oxi_io_load_blob( struct oxi_io_system *sys, const char *uri, int uri_len );
oxi_id                  oxi_io_load_image( struct oxi_io_system *sys, const char *uri, int uri_len );
oxi_id                  oxi_io_load_sound( struct oxi_io_system *sys, const char *uri, int uri_len );
bool                    oxi_io_is_loaded( struct oxi_io_system *sys, oxi_id id );

struct oxi_blob_data *  oxi_io_find_blob_data( struct oxi_io_system *sys, oxi_id id );
struct oxi_blob_data *  oxi_io_get_blob_data( struct oxi_io_system *sys, oxi_id id );

struct oxi_image_data * oxi_io_find_image_data( struct oxi_io_system *sys, oxi_id id );
struct oxi_image_data * oxi_io_get_image_data( struct oxi_io_system *sys, oxi_id id );

struct oxi_sound_data * oxi_io_find_sound_data( struct oxi_io_system *sys, oxi_id id );
struct oxi_sound_data * oxi_io_get_sound_data( struct oxi_io_system *sys, oxi_id id );

#if defined(__cplusplus)
}
#endif
