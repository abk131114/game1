#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct oxi_init_args {
  struct {
    uint16_t width, height;
  } screen;
};

struct oxi_geometry_system;
struct oxi_io_system;
struct oxi_input_system;
struct oxi_shader_system;
struct oxi_spritebatcher;
struct oxi_texture_system;

struct oxi_context {
  struct oxi_init_args cfg;

  struct {
      // struct oxi_geometry_system *geometry;
      struct oxi_io_system *io;
      struct oxi_input_system *input;
      struct oxi_shader_system *shaders;
      struct oxi_texture_system *textures;
  } sys;
};

void oxi_init( struct oxi_context *ctx, struct oxi_init_args *args );

struct oxi_vec2i { int x, y; };
struct oxi_vec2u { unsigned x, y; };
struct oxi_vec2f { float x, y; };
struct oxi_vec3f { float x, y, z; };
struct oxi_vec4f { float x, y, z, w; };
struct oxi_recti { int x, y, w, h; };
struct oxi_rectf { float x, y, w, h; };

struct oxi_color {
    union {
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        } components;

        unsigned int rgba;
    } impl;
};

float oxi_random_0_1_f( void );
int   oxi_random_range_inc_i( int min, int max );
int32_t oxi_clamp_i32( int32_t v, int32_t min, int32_t max );

struct oxi_color oxi_rgba( unsigned int color );
struct oxi_color oxi_rgba_c( uint8_t r, uint8_t g, uint8_t b, uint8_t a );

typedef uint64_t oxi_id;

#define OXI_ASSERT(Expr) assert(Expr)

// https://stackoverflow.com/questions/47981/how-to-set-clear-and-toggle-a-single-bit

#define oxi_bit_set( Bits, N ) (Bits | 1U << N)
#define oxi_bit_clear( Bits, N ) (Bits & ~(1U << N))
#define oxi_bit_has( Bits, N ) ((Bits >> N) & (1U))
#define oxi_bit_toggle( Bits, N ) (Bits ^ (1U << N))

void oxi_trace( const char *message, ... );
void oxi_warn( const char *message, ... );
void oxi_err( const char *message, ... );
void oxi_fatal( const char *message, ... );

void *oxi_alloc( size_t bytes, void *ctx );
void  oxi_free( void *ptr, void *ctx );

struct oxi_config {
	struct {
		uint16_t width, height;
	} screen;
};

// expansion macro for enum value definition
#define OXI_ENUM_VALUE(Name, Assign) Name Assign,

// expansion macro for enum to string conversion
#define OXI_ENUM_CASE(Name, Assign) case Name: return #Name;

// expansion macro for string to enum conversion
#define OXI_ENUM_STRCMP(Name, Assign) if (strcmp(str, #Name) == 0) return Name;

/// declare the access function and define enum values
#define OXI_ENUM_DECLARE(EnumType, ValueList) \
  enum EnumType { \
    ValueList(OXI_ENUM_VALUE) \
  }; \
  const char *EnumType##_to_string(enum EnumType dummy); \
  enum EnumType parse_##EnumType##_string(const char *string);

/// define the access function names
#define OXI_ENUM_DEFINE(EnumType, ValueList) \
  const char *EnumType##_to_string(enum EnumType value) { \
    switch(value) { \
      ValueList(OXI_ENUM_CASE) \
      default: return ""; /* handle input error */ \
    } \
  }

#if defined(__cplusplus)
}
#endif
