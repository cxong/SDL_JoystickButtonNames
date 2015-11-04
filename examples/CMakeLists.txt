cmake_minimum_required(VERSION 2.8)
project(SDL_joystickbuttonnames_examples)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-std=c99)
elseif(MSVC)
else()
  add_definitions(-std=gnu99)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(PkgConfig)
pkg_search_module(SDL2 ../sdl2)
if(NOT "${SDL2_FOUND}")
  find_package(SDL2 REQUIRED)
endif()

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
