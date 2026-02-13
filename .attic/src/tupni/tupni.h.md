// https://docs.unity3d.com/Packages/com.unity.inputsystem@1.5/manual/PlayerInput.html
// https://gamedevbeginner.com/input-in-unity-made-easy-complete-guide-to-the-new-system/#input_system


// control
 // is a button, for example [] SQUARE on the playstation
// controls form a hierarchy; the root is called a "device"
// Example: <Gamepad>/leftStick/x means "X Control on left stick of gamepad".

// synthetic controls:

// An InputBinding represents a connection between an Action and one or more Controls identified by a Control path. An Action can have an arbitrary number of Bindings pointed at it. Multiple Bindings can reference the same Control.




// struct InputControl {
//     int dummy;
// };

// struct ButtonControl {
//     struct InputControl base;
// };

// struct AxisControl {
//     struct InputControl base;
// };

// struct Vector2Control {
//     struct InputControl/*<vector2>*/ base;
//     struct AxisControl x, y;
// };

// // can be used as vector2d as well
// struct DpadControl {
//     struct Vector2Control base;
//     struct ButtonControl up, down, left, right;
// };

// struct StickControl {
//     struct Vector2Control base;
//     struct ButtonControl up, down, left, right;
// };
