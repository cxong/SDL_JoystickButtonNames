# SDL_JoystickButtonNames
A small library to get joystick button names and colors, to be used with SDL2

SDL2's game controller API works by assuming an "ideal gamepad" - the Xbox 360 controller - and all button names will be translated to the same layout as that gamepad. That is, the bottom face button will always be called "A", and the right face button "B", even if you're not using the Xbox 360 controller.

What if you want to display button names that match the real controller's? What about the button colors too?

This library intends to provide this missing functionality, by providing a database of controller button names and colors, given an `SDL_Joystick` and an `SDL_GameControllerButton`.

The library looks up the controller's buttons using a combination of its GUID and its joystick name.
