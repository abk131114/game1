#include "tupni.h"

#include <kinc/input/keyboard.h> // TODO (DK) wrap in oxi?
#include <kinc/log.h> // TODO (DK) remove me
#include <kinc/system.h>

#include <math.h>
#include <string.h>

#define bit_set( Bits, N ) (Bits | 1U << N)
#define bit_clear( Bits, N ) (Bits & ~(1U << N))
#define bit_has( Bits, N ) ((Bits >> N) & (1U))
#define bit_toggle( Bits, N ) (Bits ^ (1U << N))

enum keystate_flag {
    keystate_flag_was_pressed_this_frame,
    keystate_flag_state_changed,
    keystate_flag_is_down,
};

void tupni_trace( const char *format, ... ) {
	va_list args;
	va_start(args, format);
	kinc_log_args(KINC_LOG_LEVEL_INFO, format, args);
	va_end(args);
}

void tupni_err( const char *format, ... ) {
	va_list args;
	va_start(args, format);
	kinc_log_args(KINC_LOG_LEVEL_ERROR, format, args);
	va_end(args);

    kinc_debug_break();
}

void tupni_init( struct tupni_context *ctx ) {
	memset(ctx, 0, sizeof(*ctx));

    tupni_ref_name_key(ctx, 0, KINC_KEY_W, "Keyboard/w");
    tupni_ref_name_key(ctx, 1, KINC_KEY_S, "Keyboard/s");
    tupni_ref_name_key(ctx, 2, KINC_KEY_A, "Keyboard/a");
    tupni_ref_name_key(ctx, 3, KINC_KEY_D, "Keyboard/d");
    tupni_ref_name_key(ctx, 4, KINC_KEY_SPACE, "Keyboard/space");
    tupni_ref_name_key(ctx, 5, KINC_KEY_CONTROL, "Keyboard/control");

    tupni_ref_name_gamepad_axis(ctx, 6, 0, 0, 1, "<Gamepad>/leftStick/up");
    tupni_ref_name_gamepad_axis(ctx, 7, 0, 0, 0, "<Gamepad>/leftStick/down");
    tupni_ref_name_gamepad_axis(ctx, 8, 0, 1, 1, "<Gamepad>/leftStick/left");
    tupni_ref_name_gamepad_axis(ctx, 9, 0, 1, 0, "<Gamepad>/leftStick/right");

    tupni_ref_name_gamepad_button(ctx, 10, 0, 0, "<Gamepad>/buttonEast");
    tupni_ref_name_gamepad_button(ctx, 11, 0, 1, "<Gamepad>/buttonNorth");
    tupni_ref_name_gamepad_button(ctx, 12, 0, 2, "<Gamepad>/buttonSouth");
    tupni_ref_name_gamepad_button(ctx, 13, 0, 3, "<Gamepad>/buttonWest");

    tupni_ref_name_gamepad_axis(ctx, 14, 1, 0, 1, "<Gamepad>/rightStick/up");
    tupni_ref_name_gamepad_axis(ctx, 15, 1, 0, 0, "<Gamepad>/rightStick/down");
    tupni_ref_name_gamepad_axis(ctx, 16, 1, 1, 1, "<Gamepad>/rightStick/left");
    tupni_ref_name_gamepad_axis(ctx, 17, 1, 1, 0, "<Gamepad>/rightStick/right");

    tupni_ref_name_gamepad_button(ctx, 18, 1, 0, "<Gamepad>/buttonEast");
    tupni_ref_name_gamepad_button(ctx, 19, 1, 1, "<Gamepad>/buttonNorth");
    tupni_ref_name_gamepad_button(ctx, 20, 1, 2, "<Gamepad>/buttonSouth");
    tupni_ref_name_gamepad_button(ctx, 21, 1, 3, "<Gamepad>/buttonWest");

    tupni_control_define(ctx, 0, 0);
    tupni_control_define(ctx, 1, 1);
    tupni_control_define(ctx, 2, 2);
    tupni_control_define(ctx, 3, 3);
    tupni_control_define(ctx, 4, 4);
    tupni_control_define(ctx, 5, 5);
    tupni_control_define(ctx, 6, 6);
    tupni_control_define(ctx, 7, 7);
    tupni_control_define(ctx, 8, 8);
    tupni_control_define(ctx, 9, 9);

    tupni_control_define(ctx, 10, 10);
    tupni_control_define(ctx, 11, 11);
    tupni_control_define(ctx, 12, 12);
    tupni_control_define(ctx, 13, 13);
    tupni_control_define(ctx, 14, 14);
    tupni_control_define(ctx, 15, 15);
    tupni_control_define(ctx, 16, 16);
    tupni_control_define(ctx, 17, 17);
    tupni_control_define(ctx, 18, 18);
    tupni_control_define(ctx, 19, 19);

    tupni_control_define(ctx, 20, 20);
    tupni_control_define(ctx, 21, 21);
}

void tupni_action_set_action_kind( struct tupni_context *ctx, uint8_t map, tupni_action_id action, enum tupni_action_kind kind ) {
	ctx->maps[map].actions[action].kind = kind;
}

void tupni_action_set_control_kind( struct tupni_context *ctx, uint8_t map, tupni_action_id action, enum tupni_control_kind kind ) {
	ctx->maps[map].actions[action].control_kind = kind;
}

void tupni_action_enable( struct tupni_context *ctx, uint8_t map, tupni_action_id action ) {
	ctx->maps[map].actions[action].enabled = 1;
}

void tupni_action_disable( struct tupni_context *ctx, uint8_t map, tupni_action_id action ) {
	ctx->maps[map].actions[action].enabled = 0;
}

void tupni_action_set_enabled( struct tupni_context *ctx, uint8_t map, tupni_action_id action, int enabled ) {
	ctx->maps[map].actions[action].enabled = enabled;
}

void tupni_ref_name_key( struct tupni_context *ctx, uint8_t ref, uint8_t code, const char *name ) {
	struct tupni_ref *r = &ctx->refs[ref];
	r->kind = tupni_ref_kind_key;
	r->impl.key.code = code;
	r->name = name; // TODO (DK) strdup?
}

void tupni_ref_name_gamepad_axis( struct tupni_context *ctx, uint8_t ref, uint8_t gamepad, uint8_t axis, uint8_t negative, const char *name ) {
	struct tupni_ref *r = &ctx->refs[ref];
	r->kind = tupni_ref_kind_gamepad_axis;
	r->impl.gp_axis.gamepad = gamepad;
	r->impl.gp_axis.axis = axis;
	r->impl.gp_axis.negative = negative;
	r->name = name; // TODO (DK) strdup?
}

void tupni_ref_name_gamepad_button( struct tupni_context *ctx, uint8_t ref, uint8_t gamepad, uint8_t button, const char *name ) {
	struct tupni_ref *r = &ctx->refs[ref];
	r->kind = tupni_ref_kind_gamepad_button;
	r->impl.gp_button.gamepad = gamepad;
	r->impl.gp_button.button = button;
	r->name = name; // TODO (DK) strdup?
}

void tupni_control_define( struct tupni_context *ctx, uint8_t control, uint8_t ref ) {
	ctx->controls[control].ref = ref;
}

void tupni_action_bind_button( struct tupni_context *ctx, uint8_t map, uint8_t action, uint8_t binding, uint8_t control ) {
	ctx->maps[map].actions[action].impl.button.binding[binding] = (struct tupni_button_binding) {
		.active = 1,
		.control = control,
	};
}

void tupni_action_bind_composite_axis1d( struct tupni_context *ctx, uint8_t map, uint8_t action, uint8_t binding, uint8_t negative, uint8_t positive, enum tupni_axis1d_priority priority ) {
	ctx->maps[map].actions[action].impl.axis1d.binding[binding] = (struct tupni_axis1d_binding) {
		.active = 1,
		.negative = negative,
		.positive = positive,
		.priority = priority,
	};
}

void tupni_action_bind_composite_vector2d( struct tupni_context *ctx, uint8_t map, uint8_t action, uint8_t binding, uint8_t up, uint8_t down, uint8_t left, uint8_t right, enum tupni_vector2d_mode mode ) {
	ctx->maps[map].actions[action].impl.vector2d.binding[binding] = (struct tupni_vector2d_binding) {
		.active = 1,
		.up = up,
		.down = down,
		.left = left,
		.right = right,
		.mode = mode,
	};
}

void tupni_action_set_button_start_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_button_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.button.start = (struct tupni_button_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_action_set_button_trigger_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_button_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.button.trigger = (struct tupni_button_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_action_set_button_end_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_button_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.button.end = (struct tupni_button_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_action_set_axis1d_start_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_axis1d_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.axis1d.start = (struct tupni_axis1d_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_action_set_axis1d_trigger_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_axis1d_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.axis1d.trigger = (struct tupni_axis1d_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_action_set_axis1d_end_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_axis1d_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.axis1d.end = (struct tupni_axis1d_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_action_set_vector2d_start_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_vector2d_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.vector2d.start = (struct tupni_vector2d_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_action_set_vector2d_trigger_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_vector2d_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.vector2d.trigger = (struct tupni_vector2d_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_action_set_vector2d_end_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_vector2d_action_fn fn, void *userdata ) {
	ctx->maps[map].actions[action].impl.vector2d.end = (struct tupni_vector2d_callback) {
		.fn = fn,
		.ctx = userdata,
	};
}

void tupni_actionmap_set_active_index( struct tupni_context *ctx, uint8_t map ) {
	for (int i = 0; i < sizeof(ctx->maps) / sizeof(ctx->maps[0]); ++i) {
		ctx->maps[i].enabled = 0;
	}

	ctx->maps[map].enabled = 1;
}

void tupni_actionmap_enable( struct tupni_context *ctx, uint8_t map ) {
	ctx->maps[map].enabled = 1;
}

void tupni_actionmap_disable( struct tupni_context *ctx, uint8_t map ) {
	ctx->maps[map].enabled = 0;
}

void tupni_actionmap_set_enabled( struct tupni_context *ctx, uint8_t map, int enabled ) {
	ctx->maps[map].enabled = enabled;
}

static void process_button( struct tupni_context *ctx, struct tupni_map *m, struct tupni_action *action ) {
	for (int binding = 0; binding < TUPNI_MAX_BINDINGS; ++binding) {
		struct tupni_button_binding *b = &action->impl.button.binding[binding];
		uint8_t ref_index = ctx->controls[b->control].ref;
		struct tupni_ref *ref = &ctx->refs[ref_index];

		if (!b->active) {
			return;
		}

		switch (ref->kind) {
			case tupni_ref_kind_key: {
				uint8_t key_state = ctx->state.keys[ref->impl.key.code];

				if (bit_has(key_state, keystate_flag_was_pressed_this_frame)) {
					if (action->impl.button.start.fn != NULL) {
						tupni_trace("start action: %s", ref->name);
						action->impl.button.start.fn(action->impl.button.start.ctx);
					}
				}

				if (bit_has(key_state, keystate_flag_is_down)) {
					if (action->impl.button.trigger.fn != NULL) {
						tupni_trace("trigger action: %s", ref->name);
						action->impl.button.trigger.fn(action->impl.button.trigger.ctx);
					}
				}

				if (bit_has(key_state, keystate_flag_state_changed) && !bit_has(key_state, keystate_flag_is_down)) {
					if (action->impl.button.end.fn != NULL) {
						tupni_trace("end action: %s", ref->name);
						action->impl.button.end.fn(action->impl.button.end.ctx);
					}
				}
			} break;

			case tupni_ref_kind_gamepad_axis: {
			} break;

			case tupni_ref_kind_gamepad_button: {
			} break;

			default: {
				tupni_trace("unhandled tupni_ref_kind", ref->kind);
			} break;
		}
	}
}

static float get_control_value( struct tupni_context *ctx, uint8_t idx ) {
	struct tupni_control *ctrl = &ctx->controls[idx];
	struct tupni_ref *ref = &ctx->refs[ctrl->ref];

	switch (ref->kind) {
    	case tupni_ref_kind_key: {
			return ctx->state.keys[ref->impl.key.code] ? 1.0f : 0.0f;
		} break;

		case tupni_ref_kind_gamepad_axis: {
			int pad = ref->impl.gp_axis.gamepad;
			int axis = ref->impl.gp_axis.axis;
			float value = ctx->state.gpaxis[pad][axis];

			if (ref->impl.gp_axis.negative == 1) {
				return value < 0 ? -value : 0;
			} else {
				return value > 0 ? value : 0;
			}
		} break;

    	case tupni_ref_kind_gamepad_button: {
			int pad = ref->impl.gp_axis.gamepad;
			int button = ref->impl.gp_button.button;
			return ctx->state.gpbutton[pad][button];
		} break;

		default: {
			tupni_trace("unhandled tupni_ref_kind", ref->kind);
			return 0.0f;
		} break;
	}
}

static void process_vector2d_passthrough( struct tupni_context *ctx, struct tupni_map *m, struct tupni_action *action ) {
	// any change triggers a callback

	for (int binding = 0; binding < TUPNI_MAX_BINDINGS; ++binding) {
		struct tupni_vector2d_binding *b = &action->impl.vector2d.binding[binding];

		if (!b->active) {
			return;
		}

		float up = get_control_value(ctx, b->up);
		float down = get_control_value(ctx, b->down);
		float left = get_control_value(ctx, b->left);
		float right = get_control_value(ctx, b->right);
		float h = -left + right, v = -up + down;

		switch (b->mode) {
			case tupni_vector2d_mode_digital_normalized: {
				// make a diamon shape
			} break;

			case tupni_vector2d_mode_digital: {
				// send -1..0..1
			} break;

			case tupni_vector2d_mode_analog: {
				// make a circle
			} break;
		}

		action->impl.vector2d.trigger.fn(h, v, action->impl.vector2d.trigger.ctx);
	}
}

static void process_vector2d_value( struct tupni_context *ctx, struct tupni_map *m, struct tupni_action *action ) {
	// only the most actuated control triggers actions
	return process_vector2d_passthrough(ctx, m, action);
}

static void update_action( struct tupni_context *ctx, struct tupni_map *map, int idx ) {
	struct tupni_action *action = &map->actions[idx];

	if (!action->enabled) {
		return;
	}

	switch (action->kind) {
		case tupni_action_kind_button: {
			switch (action->control_kind) {
				case tupni_control_kind_axis1d: {
					// ignored
				} break;

				case tupni_control_kind_button: {
					process_button(ctx, map, action);
				} break;

				case tupni_control_kind_vector2d: {
					// ignored
				} break;

				default: {
					tupni_trace("unhandled tupni_control_kind (%i)", action->control_kind);
				} break;
			}
		} break;

		case tupni_action_kind_passthrough: {
			switch (action->control_kind) {
				// case tupni_control_kind_button: {
				// 	process_button(ctx, map, action);
				// } break;

				// case tupi_action_composite_vector1d: {
				// 	process_vector1d(ctx, map, action, idx);
				// } break;

				case tupni_control_kind_vector2d: {
					process_vector2d_passthrough(ctx, map, action);
				} break;

				default: {
					tupni_trace("unhandled tupni_control_kind (%i)", action->control_kind);
				} break;
			}
		} break;

		case tupni_action_kind_value: {
			switch (action->control_kind) {
				// case tupni_control_kind_button: {
				// 	process_button(ctx, map, action);
				// } break;

				// case tupi_action_composite_vector1d: {
				// 	process_vector1d(ctx, map, action, idx);
				// } break;

				case tupni_control_kind_vector2d: {
					process_vector2d_value(ctx, map, action);
				} break;

				default: {
					tupni_trace("unhandled tupni_control_kind (%i)", action->control_kind);
				} break;
			}
		} break;

		default: {
			tupni_trace("unhandled tupni_action_kind (%i)", action->kind);
		} break;
	}
}

static void update_map( struct tupni_context *ctx, int idx ) {
	struct tupni_map *map = &ctx->maps[idx];

	if (!map->enabled) {
		return;
	}

	for (int i = 0; i < sizeof(map->actions) / sizeof(map->actions[0]); ++i) {
		update_action(ctx, map, i);
	}
}

static void press_key( uint8_t *key ) {
    if (bit_has(*key, keystate_flag_state_changed)) {
        *key = bit_clear(*key, keystate_flag_was_pressed_this_frame);
    } else {
        *key = bit_set(*key, keystate_flag_was_pressed_this_frame);
    }

    *key = bit_set(*key, keystate_flag_state_changed);
    *key = bit_set(*key, keystate_flag_is_down);
}

static void release_key( uint8_t *key ) {
    *key = bit_clear(*key, keystate_flag_is_down);
}

static void reset_key( uint8_t *key ) {
    if (!bit_has(*key, keystate_flag_is_down)) {
        *key = bit_clear(*key, keystate_flag_state_changed);
        *key = bit_clear(*key, keystate_flag_was_pressed_this_frame);
    } else if (bit_has(*key, keystate_flag_state_changed)) {
        *key = bit_clear(*key, keystate_flag_was_pressed_this_frame);
    }
}

void tupni_update( struct tupni_context *ctx ) {
	for (int i = 0; i < sizeof(ctx->maps) / sizeof(ctx->maps[0]); ++i) {
		update_map(ctx, i);
	}

	for (int i = 0; i < TUPNI_MAX_KEYS; ++i) {
		reset_key(&ctx->state.keys[i]);
	}
}

void tupni_reset( struct tupni_context *ctx ) {
}

void tupni_inject_key_press( struct tupni_context *ctx, int code ) {
	press_key(&ctx->state.keys[code]);
}

void tupni_inject_key_release( struct tupni_context *ctx, int code ) {
    release_key(&ctx->state.keys[code]);
}

void tupni_inject_gamepad_axis( struct tupni_context *ctx, int gamepad, int axis, float value ) {
	ctx->state.gpaxis[gamepad][axis] = value;
}

void tupni_inject_gamepad_button( struct tupni_context *ctx, int gamepad, int button, float value ) {
	ctx->state.gpbutton[gamepad][button] = value;
}
