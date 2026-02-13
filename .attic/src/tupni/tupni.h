#ifndef __tupni_h_20240412__
#define __tupni_h_20240412__

#pragma once

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

// https://docs.unity3d.com/Packages/com.unity.inputsystem@1.5/manual/PlayerInput.html
// https://gamedevbeginner.com/input-in-unity-made-easy-complete-guide-to-the-new-system/#input_system

#define TUPNI_MAX_KEYS 256
#define TUPNI_MAX_GAMEPADS 4
#define TUPNI_MAX_GAMEPAD_AXIS 12
#define TUPNI_MAX_GAMEPAD_BUTTONS 16
#define TUPNI_MAX_CONTROLS 64
#define TUPNI_MAX_REFS 64
#define TUPNI_MAX_ACTIONS 16
#define TUPNI_MAX_BINDINGS 2
#define TUPNI_MAX_MAPS 2
#define TUPNI_MAX_AXIS 8

// control
 // is a button, for example [] SQUARE on the playstation
// controls form a hierarchy; the root is called a "device"
// Example: <Gamepad>/leftStick/x means "X Control on left stick of gamepad".
// synthetic controls: virtual
// An InputBinding represents a connection between an Action and one or more Controls identified by a Control path. An Action can have an arbitrary number of Bindings pointed at it. Multiple Bindings can reference the same Control.

typedef uint8_t tupni_axis_id;
typedef uint8_t tupni_action_id;

// https://docs.unity3d.com/Packages/com.unity.inputsystem@1.5/manual/Actions.html#action-types
enum tupni_action_kind {
    // This is the default Action type. Use this for any inputs which should track continuous changes to the state of a Control.
    // Value type actions continuously monitor all the Controls which are bound to the Action, and then choose the one which is the most actuated to be the Control driving the Action, and report the values from that Control in callbacks, triggered whenever the value changes. If a different bound Control actuated more, then that Control becomes the Control driving the Action, and the Action starts reporting values from that Control. This process is called conflict resolution. This is useful if you want to allow different Controls to control an Action in the game, but only take input from one Control at the same time.
    // When the Action initially enables, it performs an initial state check of all bound Controls. If any of them is actuated, the Action then triggers a callback with the current value.
    tupni_action_kind_value,

    // This is very similar to Value, but Button type Actions can only be bound to ButtonControl Controls, and don't perform an initial state check like Value Actions do (see the Value section above). Use this for inputs that trigger an Action once every time they are pressed. The initial state check is usually not useful in such cases, because it can trigger actions if the button is still held down from a previous press when the Action was enabled.
    tupni_action_kind_button,

    // Pass-Through Actions bypass the conflict resolution process described above for Value Actions and don't use the concept of a specific Control driving the Action. Instead, any change to any bound Control triggers a callback with that Control's value. This is useful if you want to process all input from a set of Controls.
    tupni_action_kind_passthrough,
};

typedef void (*tupni_button_action_fn)( void * );

struct tupni_button_callback {
    tupni_button_action_fn fn;
    void *ctx;
};

typedef void (*tupni_axis1d_action_fn)( float, void * );

struct tupni_axis1d_callback {
    tupni_axis1d_action_fn fn;
    void *ctx;
};

typedef void (*tupni_vector2d_action_fn)( float, float, void * );

struct tupni_vector2d_callback {
    tupni_vector2d_action_fn fn;
    void *ctx;
};

struct tupni_button_binding {
    uint8_t active : 1;
    uint8_t control;
};

// enum tupni_action_phase {
//     disabled,
//     waiting,
//     started,
//     performed,
//     canceled,
// };

enum tupni_axis1d_priority {
    // If both positive and negative are actuated, the sides cancel each other out and the result is 0.
    tupni_axis1d_priority_neither,

    // If both positive and negative are actuated, the value of positive wins and negative is ignored.
    tupni_axis1d_priority_positive,

    // If both positive and negative are actuated, the value of negative wins and positive is ignored.
    tupni_axis1d_priority_negative,
};

struct tupni_axis1d_binding {
    uint8_t                         active : 1;
    enum tupni_axis1d_priority      priority : 2;
    uint8_t                         negative, positive; // index into composite table?
};

enum tupni_vector2d_mode {
    // If this is set to Mode.DigitalNormalized, inputs are treated as buttons (off if below defaultButtonPressPoint and on if equal to or greater). Each input is 0 or 1 depending on whether the button is pressed or not. The vector resulting from the up/down/left/right parts is normalized. The result is a diamond-shaped 2D input range.
    tupni_vector2d_mode_digital_normalized, // default

    // If this is set to Mode.Digital, the behavior is essentially the same as Mode.DigitalNormalized except that the resulting vector is not normalized.
    tupni_vector2d_mode_digital,

    // Finally, if this is set to Mode.Analog, inputs are treated as analog (i.e. full floating-point values) and, other than down and left being inverted, values will be passed through as is.
    tupni_vector2d_mode_analog,
};

struct tupni_vector2d_binding {
    uint8_t                             active : 1;
    enum tupni_vector2d_mode            mode : 3;
    uint8_t                             up, down, left, right; // index into composite table?
};

// TODO (DK) rename binding kind?
enum tupni_control_kind {
    tupni_control_kind_button = 1,
    tupni_control_kind_axis1d,
    tupni_control_kind_vector2d,
};

struct tupni_action {
    enum tupni_action_kind      kind : 2;
    enum tupni_control_kind     control_kind : 2;
    uint8_t                     enabled : 1;

    union {
        struct {
            struct tupni_button_binding binding[TUPNI_MAX_BINDINGS];
            struct tupni_button_callback start, trigger, end;
        } button;

        struct {
            struct tupni_axis1d_binding binding[TUPNI_MAX_BINDINGS];
            struct tupni_axis1d_callback start, trigger, end;
        } axis1d;

        struct {
            struct tupni_vector2d_binding binding[TUPNI_MAX_BINDINGS];
            struct tupni_vector2d_callback start, trigger, end;
        } vector2d;
    } impl;
};

struct tupni_key_ref {
    uint8_t code;
};

struct tupni_gamepad_axis_ref {
    uint8_t gamepad : 4;
    uint8_t negative : 1;
    uint8_t axis;
};

struct tupni_gamepad_button_ref {
    uint8_t gamepad;
    uint8_t button;
};

enum tupni_ref_kind {
    tupni_ref_kind_key = 1,
    tupni_ref_kind_gamepad_axis,
    tupni_ref_kind_gamepad_button,
};

struct tupni_ref {
    enum tupni_ref_kind kind : 4;
    const char *name;

    union {
        struct tupni_key_ref key;
        struct tupni_gamepad_axis_ref gp_axis;
        struct tupni_gamepad_button_ref gp_button;
    } impl;
};

struct tupni_control {
    uint8_t ref;
};

struct tupni_map {
    struct tupni_action     actions[TUPNI_MAX_ACTIONS]; // TODO (DK) have the actions in the user code instead?
    uint8_t                 enabled : 1;
};

struct tupni_state {
    uint8_t     keys[TUPNI_MAX_KEYS];
    float       gpaxis[TUPNI_MAX_GAMEPADS][TUPNI_MAX_GAMEPAD_AXIS]; // TODO (DK) normalized uint8?
    float       gpbutton[TUPNI_MAX_GAMEPADS][TUPNI_MAX_GAMEPAD_BUTTONS]; // TODO (DK) normalized uint8?
};

struct tupni_context {
    struct tupni_ref        refs[TUPNI_MAX_REFS];
    struct tupni_control    controls[TUPNI_MAX_CONTROLS];
    struct tupni_state      state;
    struct tupni_map        maps[TUPNI_MAX_MAPS];
};

void tupni_init( struct tupni_context *ctx );
void tupni_update( struct tupni_context *ctx );
void tupni_reset( struct tupni_context *ctx );

void tupni_action_set_action_kind( struct tupni_context *ctx, uint8_t map, tupni_action_id action, enum tupni_action_kind kind );
void tupni_action_set_control_kind( struct tupni_context *ctx, uint8_t map, tupni_action_id action, enum tupni_control_kind kind );
void tupni_action_enable( struct tupni_context *ctx, uint8_t map, tupni_action_id action );
void tupni_action_disable( struct tupni_context *ctx, uint8_t map, tupni_action_id action );
void tupni_action_set_enabled( struct tupni_context *ctx, uint8_t map, tupni_action_id action, int enabled );

void tupni_ref_name_key( struct tupni_context *ctx, uint8_t ref, uint8_t code, const char *name );
void tupni_ref_name_gamepad_axis( struct tupni_context *ctx, uint8_t ref, uint8_t gamepad, uint8_t axis, uint8_t negative, const char *name );
void tupni_ref_name_gamepad_button( struct tupni_context *ctx, uint8_t ref, uint8_t gamepad, uint8_t button, const char *name );

void tupni_control_define( struct tupni_context *ctx, uint8_t control, uint8_t ref );

void tupni_action_bind_button( struct tupni_context *ctx, uint8_t map, uint8_t action, uint8_t binding, uint8_t control );
void tupni_action_bind_composite_axis1d( struct tupni_context *ctx, uint8_t map, uint8_t action, uint8_t binding, uint8_t negative, uint8_t positive, enum tupni_axis1d_priority priority );
void tupni_action_bind_composite_vector2d( struct tupni_context *ctx, uint8_t map, uint8_t action, uint8_t binding, uint8_t up, uint8_t down, uint8_t left, uint8_t right, enum tupni_vector2d_mode mode );

void tupni_action_set_button_start_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_button_action_fn fn, void *userdata );
void tupni_action_set_button_trigger_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_button_action_fn fn, void *userdata );
void tupni_action_set_button_end_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_button_action_fn fn, void *userdata );

void tupni_action_set_axis1d_start_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_axis1d_action_fn fn, void *userdata );
void tupni_action_set_axis1d_trigger_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_axis1d_action_fn fn, void *userdata );
void tupni_action_set_axis1d_end_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_axis1d_action_fn fn, void *userdata );

void tupni_action_set_vector2d_start_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_vector2d_action_fn fn, void *userdata );
void tupni_action_set_vector2d_trigger_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_vector2d_action_fn fn, void *userdata );
void tupni_action_set_vector2d_end_callback( struct tupni_context *ctx, uint8_t map, tupni_action_id action, tupni_vector2d_action_fn fn, void *userdata );

void tupni_actionmap_enable( struct tupni_context *ctx, uint8_t map );
void tupni_actionmap_disable( struct tupni_context *ctx, uint8_t map );
void tupni_actionmap_set_enabled( struct tupni_context *ctx, uint8_t map, int enabled );
void tupni_actionmap_set_active_index( struct tupni_context *ctx, uint8_t idx );

void tupni_inject_key_press( struct tupni_context *ctx, int code );
void tupni_inject_key_release( struct tupni_context *ctx, int code );
void tupni_inject_gamepad_axis( struct tupni_context *ctx, int gamepad, int axis, float value );
void tupni_inject_gamepad_button( struct tupni_context *ctx, int gamepad, int button, float value );

#if defined(__cplusplus)
}
#endif

#endif
