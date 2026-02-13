#include "oxi.io.h"

#include <kinc/audio1/sound.h>
#include <kinc/io/filereader.h>
#include <kinc/threads/event.h>
#include <kinc/threads/mutex.h>
#include <kinc/threads/thread.h>
#include <stb_ds.h>
#include <inttypes.h>

#define OXI_IO_MAX_PATH 255

enum file_kind {
	FILE_KIND_BLOB,
	FILE_KIND_IMAGE,
	FILE_KIND_SOUND
};

union file_data {
	struct oxi_blob_data blob;
	struct oxi_image_data image;
	struct oxi_sound_data sound;
};

struct file_reference {
	char name[OXI_IO_MAX_PATH + 1];
	oxi_id id;
	union file_data data;
	enum file_kind kind;
	bool error;
};

struct oxi_io_system {
	void *(*alloc)( size_t n, void *ctx );
	void (*free)( void *ptr, void *ctx );
	void *mem_ctx;

	oxi_id next_id;

	kinc_thread_t thread;
	kinc_event_t event;
	kinc_mutex_t loading_mutex;
	kinc_mutex_t loaded_mutex;

	struct file_reference *loading_files;
	struct file_reference *loaded_files;
};

static bool string_ends_with(char *str, const char *end) {
	size_t str_len = strlen(str);
	size_t end_len = strlen(end);

	if (end_len > str_len) {
		return false;
	}

	return strcmp(&str[str_len - end_len], end) == 0;
}

static void loader_thread_loop( void *param ) {
	struct oxi_io_system *sys = param;

	while (true) {
		kinc_event_wait(&sys->event);

		bool has_next = false;
		struct file_reference next;

		kinc_mutex_lock(&sys->loading_mutex);

		if (arrlen(sys->loading_files) > 0) {
			has_next = true;
			next = arrpop(sys->loading_files);
		} else {
			kinc_event_reset(&sys->event);
		}

		kinc_mutex_unlock(&sys->loading_mutex);

		while (has_next) {
			switch (next.kind) {
				case FILE_KIND_BLOB: {
                    oxi_trace("[oxi.io.loader_thread_loop] processing blob '%s'", next.name);

					kinc_file_reader_t reader;

					if (kinc_file_reader_open(&reader, next.name, KINC_FILE_TYPE_ASSET)) {
						next.data.blob.size = kinc_file_reader_size(&reader);
						next.data.blob.bytes = sys->alloc(next.data.blob.size + 1, sys->mem_ctx); // ecs_os_alloca(next.data.blob.size);
						kinc_file_reader_read(&reader, next.data.blob.bytes, next.data.blob.size);
						((char *)next.data.blob.bytes)[next.data.blob.size] = 0;
						kinc_file_reader_close(&reader);
					} else {
						next.error = true;
					}
				} break;

				case FILE_KIND_IMAGE: {
					oxi_trace("[oxi.io.loader_thread_loop] processing image '%s'", next.name);

					kinc_image_t image = { 0 };
					size_t size = kinc_image_size_from_file(next.name);

					if (size > 0) {
						void *data = sys->alloc(size, sys->mem_ctx);

						if (kinc_image_init_from_file(&image, data, next.name) != 0) {
							next.data.image.image = image;
						} else {
							oxi_free(data, NULL);
							next.error = true;
						}
					} else {
						next.error = true;
					}
				} break;

				case FILE_KIND_SOUND: {
					oxi_trace("[oxi.io.loader_thread_loop] processing sound '%s'", next.name);

					memset(&next.data.sound, 0, sizeof(next.data.sound));
					next.data.sound.a1 = kinc_a1_sound_create(next.name);

					// if (string_ends_with(next.name, ".ogg")) {
					// 	kinc_file_reader_t reader;

					// 	if (kinc_file_reader_open(&reader, next.name, KINC_FILE_TYPE_ASSET)) {
					// 		next.data.sound.size = kinc_file_reader_size(&reader);
					// 		next.data.sound.compressed_samples = sys->alloc(next.data.sound.size, sys->mem_ctx);
					// 		kinc_file_reader_read(&reader, next.data.sound.compressed_samples, kinc_file_reader_size(&reader));
					// 		kinc_file_reader_close(&reader);
					// 	} else {
					// 		next.error = true;
					// 	}
					// } else {
					// 	kinc_a1_sound_t *sound = kinc_a1_sound_create(next.name);
					// 	next.data.sound.size = sound->size * 2;
					// 	next.data.sound.samples = sys->alloc(sizeof(float) * next.data.sound.size, sys->mem_ctx);

					// 	for (int i = 0; i < sound->size; ++i) {
					// 		next.data.sound.samples[i * 2 + 0] = (float)(sound->left[i] / 32767.0);
					// 		next.data.sound.samples[i * 2 + 1] = (float)(sound->right[i] / 32767.0);
					// 	}

					// 	next.data.sound.channels = sound->channel_count;
					// 	next.data.sound.sample_rate = sound->samples_per_second;
					// 	next.data.sound.length = (sound->size / (sound->bits_per_sample / 8) / sound->channel_count) / (float)sound->samples_per_second;
					// 	kinc_a1_sound_destroy(sound);
					// }
				} break;

				default: {
					oxi_warn("[oxi.io.loader_thread_loop] unhandled asset type '%i'", next.kind);
				} break;
			}

			kinc_mutex_lock(&sys->loaded_mutex);
			arrput(sys->loaded_files, next);
			kinc_mutex_unlock(&sys->loaded_mutex);

			has_next = false;
			kinc_mutex_lock(&sys->loading_mutex);

			if (arrlen(sys->loading_files) > 0) {
				has_next = true;
				next = arrpop(sys->loading_files);
			} else {
				kinc_event_reset(&sys->event);
			}

			kinc_mutex_unlock(&sys->loading_mutex);
		}
	}
}

// static struct file_reference get_loaded_file( struct oxi_io_system *sys ) {
// 	kinc_mutex_lock(&sys->loaded_mutex);

// 	if (arrlen(sys->loaded_files) > 0) {
// 		struct file_reference file = arrpop(sys->loaded_files);
// 		kinc_mutex_unlock(&sys->loaded_mutex);
// 		return file;
// 	} else {
// 		kinc_mutex_unlock(&sys->loaded_mutex);
// 		struct file_reference file = { 0 };
// 		return file;
// 	}
// }

// static void step( struct oxi_io_system *sys ) {
//     struct file_reference ref = get_loaded_file(sys);

//     if (ref.id != 0 && !ref.error) {
// 		switch (ref.kind) {
// 			case FILE_KIND_BLOB: {
// 				// ecs_set(it->world, ref.entity_id, oxi_BlobContent, { .data = ref.data.blob.bytes, .size = ref.data.blob.size });
// 				// ecs_add(it->world, ref.entity_id, oxi_BlobLoadingFinished);
// 			} break;

// 			case FILE_KIND_IMAGE: {
// 				// ecs_set(it->world, ref.entity_id, oxi_ImageContent, { .image = ref.data.image.image });
// 				// kinc_g4_texture_init_from_image(&ip->texture.texture, &ref.data.image.image);
// 				// kinc_image_destroy(&ref.data.image.image);
// 			} break;

// 			case FILE_KIND_SOUND: {
// 			} break;
// 		}
//     }
// }

static void init( struct oxi_io_system *sys ) {
	kinc_mutex_init(&sys->loaded_mutex);
	kinc_mutex_init(&sys->loading_mutex);
	kinc_event_init(&sys->event, false);
	kinc_thread_init(&sys->thread, loader_thread_loop, sys);
}

struct oxi_io_system *oxi_io_system_create( void *(*alloc)( size_t, void * ), void *ctx ) {
	size_t n = sizeof(struct oxi_io_system);
	struct oxi_io_system *sys = alloc(n, ctx);
	memset(sys, 0, sizeof(*sys));
	sys->alloc = alloc;
	sys->mem_ctx = ctx;
	init(sys);
	return sys;
}

oxi_id oxi_io_load_blob( struct oxi_io_system *sys, const char *uri, int uri_len ) {
    OXI_ASSERT(strlen(uri) <= OXI_IO_MAX_PATH);

	struct file_reference file = { 0 };
	snprintf(file.name, OXI_IO_MAX_PATH, "%.*s", uri_len, uri);
	file.id = sys->next_id += 1;
	file.kind = FILE_KIND_BLOB;

	oxi_trace("[oxi.io.oxi_io_load_blob] queue '%.*s' id=%"PRIu64, uri_len, uri, file.id);

	kinc_mutex_lock(&sys->loading_mutex);
	arrput(sys->loading_files, file);
	kinc_event_signal(&sys->event);
	kinc_mutex_unlock(&sys->loading_mutex);

	return file.id;
}

oxi_id oxi_io_load_image( struct oxi_io_system *sys, const char *uri, int uri_len ) {
    OXI_ASSERT(strlen(uri) <= OXI_IO_MAX_PATH);

	struct file_reference file = { 0 };
	snprintf(file.name, OXI_IO_MAX_PATH, "%.*s", uri_len, uri);
	file.id = sys->next_id += 1;
	file.kind = FILE_KIND_IMAGE;

	oxi_trace("[oxi.io.oxi_io_load_image] queue '%.*s' id=%"PRIu64, uri_len, uri, file.id);

	kinc_mutex_lock(&sys->loading_mutex);
	arrput(sys->loading_files, file);
	kinc_event_signal(&sys->event);
	kinc_mutex_unlock(&sys->loading_mutex);
	return file.id;
}

oxi_id oxi_io_load_sound( struct oxi_io_system *sys, const char *uri, int uri_len ) {
    OXI_ASSERT(strlen(uri) <= OXI_IO_MAX_PATH);

	struct file_reference file = { 0 };
	snprintf(file.name, OXI_IO_MAX_PATH, "%.*s", uri_len, uri);
	file.id = sys->next_id += 1;
	file.kind = FILE_KIND_SOUND;

	oxi_trace("[oxi.io.oxi_io_load_sound] queue '%.*s' id=%"PRIu64, uri_len, uri, file.id);

	kinc_mutex_lock(&sys->loading_mutex);
	arrput(sys->loading_files, file);
	kinc_event_signal(&sys->event);
	kinc_mutex_unlock(&sys->loading_mutex);

	return file.id;
}

bool oxi_io_is_loaded( struct oxi_io_system *sys, oxi_id id ) {
	// TODO (DK) are mutexes required here?
	for (unsigned i = 0; i < arrlenu(sys->loaded_files); ++i) {
		if (sys->loaded_files[i].id == id) {
			return true;
		}
	}

	return false;
}

struct oxi_blob_data *oxi_io_get_blob_data( struct oxi_io_system *sys, oxi_id id ) {
	// TODO (DK) are mutexes required here?
	for (unsigned i = 0; i < arrlenu(sys->loaded_files); ++i) {
		struct file_reference *ref = &sys->loaded_files[i];

		if (ref->id == id) {
			if (ref->kind != FILE_KIND_BLOB) {
				oxi_warn("[oxi.io.oxi_io_get_blob_data] id %"PRIu64" is not a blob", id);
				return NULL;
			}

			if (ref->error) {
				oxi_warn("[oxi.io.oxi_io_get_blob_data] id %"PRIu64" failed to load", id);
				return NULL;
			}

			return &ref->data.blob;
		}
	}

	return NULL;
}

struct oxi_image_data *oxi_io_find_image_data( struct oxi_io_system *sys, oxi_id id ) {
	for (unsigned i = 0; i < arrlenu(sys->loaded_files); ++i) {
		struct file_reference *ref = &sys->loaded_files[i];

		if (ref->id == id) {
			if (ref->kind != FILE_KIND_IMAGE) {
				return NULL;
			}

			if (ref->error) {
				return NULL;
			}

			return &ref->data.image;
		}
	}

	return NULL;
}

struct oxi_image_data *oxi_io_get_image_data( struct oxi_io_system *sys, oxi_id id ) {
	for (unsigned i = 0; i < arrlenu(sys->loaded_files); ++i) {
		struct file_reference *ref = &sys->loaded_files[i];

		if (ref->id == id) {
			if (ref->kind != FILE_KIND_IMAGE) {
				oxi_warn("[oxi.io.oxi_io_get_image_data] id %"PRIu64" is not an image", id);
				OXI_ASSERT(false);
				return NULL;
			}

			if (ref->error) {
				oxi_warn("[oxi.io.oxi_io_get_image_data] id %"PRIu64" failed to load", id);
				OXI_ASSERT(false);
				return NULL;
			}

			return &ref->data.image;
		}
	}

	OXI_ASSERT(!"image is not contained in loaded_files[]");
	return NULL;
}

struct oxi_sound_data *oxi_io_get_sound_data( struct oxi_io_system *sys, oxi_id id ) {
	for (unsigned i = 0; i < arrlenu(sys->loaded_files); ++i) {
		struct file_reference *ref = &sys->loaded_files[i];

		if (ref->id == id) {
			if (ref->kind != FILE_KIND_SOUND) {
				oxi_warn("[oxi.io.oxi_io_get_image_data] id %"PRIu64" is not a sound", id);
				return NULL;
			}

			if (ref->error) {
				oxi_warn("[oxi.io.oxi_io_get_image_data] id %"PRIu64" failed to load", id);
				return NULL;
			}

			return &ref->data.sound;
		}
	}

	return NULL;
}
