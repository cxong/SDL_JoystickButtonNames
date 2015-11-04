cmake_minimum_required(VERSION 2.8)
project(SDL_joystickbuttonnames_examples)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)

include_directories(.. ${SDL2_INCLUDE_DIR})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/lib)

# libSDLMain comes with libSDL if needed on certain platforms
if(NOT SDLMAIN_LIBRARY)
  set(SDLMAIN_LIBRARY "")
endif()

add_executable(find_all_joy_buttons find_all_joy_buttons.c)
target_link_libraries(find_all_joy_buttons SDL_joystickbuttonnames)

add_executable(minimal minimal.c)
target_link_libraries(minimal SDL_joystickbuttonnames)
