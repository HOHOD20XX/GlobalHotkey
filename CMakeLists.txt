cmake_minimum_required(VERSION 3.25)

project(GlobalHotkey LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE SRC
     src/global_hotkey_manager/*.cpp
     src/hook_ghm/*.cpp
     src/key_combination/*.cpp
     src/utility/*.cpp
     src/key/key.cpp
     src/keyboard_hook/keyboard_hook.cpp
     src/keyboard_hook/keyboard_hook_private.cpp
     src/register_ghm/register_ghm.cpp
     src/register_ghm/register_ghm_private.cpp)

if(WIN32)
    file(GLOB_RECURSE SRC ${SRC} src/*win*.cpp)
elseif(APPLE)
    file(GLOB_RECURSE SRC ${SRC} src/*mac*.cpp)
elseif(LINUX)
    file(GLOB_RECURSE SRC ${SRC} src/*linux*.cpp)
    file(GLOB_RECURSE SRC ${SRC} src/*x11*.cpp)
endif()

set(GBHK_MSG_PREFIX "[GlobalHotkey]")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} In-source builds are not allowed. Please make a new directory (e.g. build) and run CMake from there.")
endif()

if(NOT DEFINED GBHK_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(GBHK_MASTER_PROJECT ON)
    else()
        set(GBHK_MASTER_PROJECT OFF)
    endif()
endif()

# Build shared library option.
option(GBHK_BUILD_SHARED "Build shared library" OFF)
if(GBHK_BUILD_SHARED)
    message(STATUS "${GBHK_MSG_PREFIX} Build shared library")
else()
    message(STATUS "${GBHK_MSG_PREFIX} Build static library")
endif()

# Disable register hotkey manager option.
option(GBHK_DISABLE_REGISTER "Disable register global hotkey manager" OFF)
if(GBHK_DISABLE_REGISTER)
    message(STATUS "${GBHK_MSG_PREFIX} Disable register global hotkey manager")
endif()

# Disable hook hotkey manager option.
option(GBHK_DISABLE_HOOK "Disable hook global hotkey manager" OFF)
if(GBHK_DISABLE_HOOK)
    message(STATUS "${GBHK_MSG_PREFIX} Disable hook global hotkey manager")
endif()

# Build example option.
option(GBHK_BUILD_EXAMPLE "Build example" ${GBHK_MASTER_PROJECT})
if(GBHK_BUILD_EXAMPLE)
    message(STATUS "${GBHK_MSG_PREFIX} Build example")
else()
    message(STATUS "${GBHK_MSG_PREFIX} Not build example")
endif()

if(GBHK_DISABLE_HOOK AND GBHK_DISABLE_REGISTER)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot set Both GBHK_DISABLE_HOOK and GBHK_DISABLE_REGISTER to ON")
endif()

find_package(Threads REQUIRED)

if(GBHK_BUILD_SHARED)
    add_library(global_hotkey SHARED ${SRC})
    target_compile_definitions(global_hotkey PUBLIC GLOBAL_HOTKEY_BUILD_SHARED)
else()
    add_library(global_hotkey STATIC ${SRC})
endif()

if(GBHK_DISABLE_REGISTER)
    target_compile_definitions(global_hotkey PUBLIC GLOBAL_HOTKEY_DISABLE_REGISTER)
endif()

if(GBHK_DISABLE_HOOK)
    target_compile_definitions(global_hotkey PUBLIC GLOBAL_HOTKEY_DISABLE_HOOK)
endif()

target_link_libraries(global_hotkey PRIVATE Threads::Threads)

if(WIN32)
    message(STATUS "${GBHK_MSG_PREFIX} Windows platform detected")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
endif()

if(LINUX)
    message(STATUS "${GBHK_MSG_PREFIX} Linux platform detected")
    find_path(LIBEVDEV_INCLUDE_DIR libevdev/libevdev.h PATHS /usr/include /usr/include/libevdev-1.0)
    find_library(LIBEVDEV_LIBRARY NAMES evdev PATHS /usr/lib /usr/lib/x86_64-linux-gnu)
    if(LIBEVDEV_INCLUDE_DIR AND LIBEVDEV_LIBRARY)
        message(STATUS "${GBHK_MSG_PREFIX} Found libevdev: ${LIBEVDEV_LIBRARY}")
        include_directories(${LIBEVDEV_INCLUDE_DIR})
        target_link_libraries(global_hotkey PRIVATE ${LIBEVDEV_LIBRARY})
    else()
        message(FATAL_ERROR "${GBHK_MSG_PREFIX} Not found libevdev")
    endif()
endif()

set_target_properties(global_hotkey PROPERTIES DEBUG_POSTFIX d)

add_library(global_hotkey::global_hotkey ALIAS global_hotkey)

if(GBHK_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
