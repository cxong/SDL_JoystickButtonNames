# SDL_JoystickButtonNames
[![Build Status](https://github.com/cxong/SDL_JoystickButtonNames/actions/workflows/build.yml/badge.svg)](https://github.com/cxong/SDL_JoystickButtonNames/actions/workflows/build.yml)

A small library to get joystick button names and colors, to be used with SDL2

SDL2's game controller API works by assuming an "ideal gamepad" - the Xbox 360 controller - and all button names will be translated to the same layout as that gamepad. That is, the bottom face button will always be called "A", and the right face button "B", even if you're not using the Xbox 360 controller.

What if you want to display button names that match the real controller's? What about the button colors too?

This library intends to provide this missing functionality, by providing a database of controller button names and colors, given an `SDL_Joystick` and an `SDL_GameControllerButton`.

The library looks up the controller's buttons using a combination of its GUID and its joystick name.

## How it works

Given the following code:

    SDL_Joystick *joy;
    char name[256];
    Uint8 r, g, b;
    SDLJBN_GetButtonNameAndColor(joy, SDL_CONTROLLER_BUTTON_A, name, &r, &g, &b);
    printf("Button A is %s with color rgb(%d,%d,%d)\n", name, r, g, b);

If you have an Xbox 360 controller connected, this will print:

> Button A is A with color rgb(96,160,0)

But if you have a Logitech Dual Action, which looks like this:

![](https://raw.githubusercontent.com/cxong/SDL_JoystickButtonNames/master/docs/lda.png)

Then the code will print:

> Button A is 2 with color rgb(244,244,244)

## Running the examples

- Clone this repo
- Use CMake to configure and compile the examples:

      cd SDL_JoystickButtonNames/examples
      cmake .
      make
- Make sure you have joysticks plugged in, and run the example

      ./find_all_joy_buttons
- You should now have a file called `out.html`, which when you open up you'll see diagrams like this:

    ![](https://raw.githubusercontent.com/cxong/SDL_JoystickButtonNames/master/docs/out_screen.png)

## Getting started

- Include the library in your SDL2 + CMake project, or if you are using a different build system, include `SDL_joystickbuttonnames.c` and `SDL_joystickbuttonnames.h` in your project
  - This includes `db.h`, which is generated from `db.h.cmake`
- In your code:
  - Call `SDLJBN_Init()` during startup
  - (Recommended) take a copy of `gamecontrollerbuttondb.txt` and include it in your game's data directory, and load it during runtime using `SDLJBN_AddMappingsFromFile()`
  - Once you have detected and opened `SDL_Joystick` devices, get button names and colors using `SDLJBN_GetButtonNameAndColor()`
  - You can also get axis names and colors using `SDLJBN_GetAxisNameAndColor()`
- Check out `examples/minimal.c` for a minimal code example

## This project needs your help
If you have a joystick that is not included in this project's database (`gamecontrollerbuttondb.txt`), please let us know! Include your OS, the joystick's SDL2 GUID, and a picture of the joystick, so it can be added to the database.
