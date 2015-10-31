include_directories(${SDL2_INCLUDE_DIR})

add_library(SDL_joystickbuttonnames STATIC
  SDL_joystickbuttonnames.c SDL_joystickbuttonnames.h)
target_link_libraries(SDL_joystickbuttonnames ${SDL2_LIBRARY})
