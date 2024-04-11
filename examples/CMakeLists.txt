cmake_minimum_required(VERSION 3.1)
project(SDL_joystickbuttonnames_examples)
cmake_policy(SET CMP0054 NEW)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-std=c99)
elseif(MSVC)
else()
  add_definitions(-std=gnu99)
endif()

find_package(SDL2 REQUIRED)

include_directories(.. ${SDL2_INCLUDE_DIRS})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/lib)

add_executable(find_all_joy_buttons find_all_joy_buttons.c)
target_link_libraries(find_all_joy_buttons SDL_joystickbuttonnames)

add_executable(minimal minimal.c)
target_link_libraries(minimal SDL_joystickbuttonnames)
