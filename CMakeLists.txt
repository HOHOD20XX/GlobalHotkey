cmake_minimum_required(VERSION 3.25)

project(GlobalHotkey LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(GBHK_MSG_PREFIX "[Global Hotkey]")

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
option(GBHK_DISABLE_REGISTER "Disable register Global Hotkey Manager" OFF)
if(GBHK_DISABLE_REGISTER)
    message(STATUS "${GBHK_MSG_PREFIX} Disable register Global Hotkey Manager")
endif()

# Disable hook hotkey manager option.
option(GBHK_DISABLE_HOOK "Disable hook Global Hotkey Manager" OFF)
if(GBHK_DISABLE_HOOK)
    message(STATUS "${GBHK_MSG_PREFIX} Disable hook Global Hotkey Manager")
endif()

# Build example option.
option(GBHK_BUILD_EXAMPLE "Build example" ${GBHK_MASTER_PROJECT})

if(GBHK_DISABLE_HOOK AND GBHK_DISABLE_REGISTER)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot set Both GBHK_DISABLE_HOOK and GBHK_DISABLE_REGISTER to ON")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE SRC
     src/global_hotkey_manager/*.cpp
     src/key/key.cpp
     src/key_combination/key_combination.cpp)

if(WIN32)
    file(GLOB_RECURSE SRC ${SRC} src/key/*win*.cpp)
elseif(APPLE)
    file(GLOB_RECURSE SRC ${SRC} src/key/*mac*.cpp)
elseif(LINUX)
    if(NOT GBHK_DISABLE_REGISTER)
        file(GLOB_RECURSE SRC ${SRC} src/key/*x11*.cpp)
    endif()
    if(NOT GBHK_DISABLE_HOOK)
        file(GLOB_RECURSE SRC ${SRC} src/key/*linux*.cpp)
    endif()
endif()

if(NOT GBHK_DISABLE_REGISTER)
    set(SRC ${SRC}
         src/register_global_hotkey_manager/register_global_hotkey_manager.cpp
         src/register_global_hotkey_manager/register_ghm_private.cpp)
    if(WIN32)
        file(GLOB_RECURSE SRC ${SRC} src/register_global_hotkey_manager/*win*.cpp)
    elseif(APPLE)
        file(GLOB_RECURSE SRC ${SRC} src/register_global_hotkey_manager/*mac*.cpp)
    elseif(LINUX)
        file(GLOB_RECURSE SRC ${SRC} src/register_global_hotkey_manager/*x11*.cpp)
     endif()
endif()

if(NOT GBHK_DISABLE_HOOK)
    file(GLOB_RECURSE SRC ${SRC} src/hook_global_hotkey_manager/*.cpp)
    set(SRC ${SRC}
         src/keyboard_hook_manager/keyboard_hook_manager.cpp
         src/keyboard_hook_manager/kbhm_private.cpp)
    if(WIN32)
        file(GLOB_RECURSE SRC ${SRC} src/keyboard_hook_manager/*win*.cpp)
    elseif(APPLE)
        file(GLOB_RECURSE SRC ${SRC} src/keyboard_hook_manager/*mac*.cpp)
    elseif(LINUX)
        file(GLOB_RECURSE SRC ${SRC} src/keyboard_hook_manager/*linux*.cpp)
     endif()
endif()

set(TARGET_NAME global_hotkey)

if(GBHK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${SRC})
    target_compile_definitions(${TARGET_NAME} PUBLIC GLOBAL_HOTKEY_BUILD_SHARED)
else()
    add_library(${TARGET_NAME} STATIC ${SRC})
endif()

if(GBHK_DISABLE_REGISTER)
    target_compile_definitions(${TARGET_NAME} PUBLIC GLOBAL_HOTKEY_DISABLE_REGISTER)
endif()

if(GBHK_DISABLE_HOOK)
    target_compile_definitions(${TARGET_NAME} PUBLIC GLOBAL_HOTKEY_DISABLE_HOOK)
endif()

if(WIN32)
    message(STATUS "${GBHK_MSG_PREFIX} Windows platform detected")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
endif()

if(APPLE)
    message(STATUS "${GBHK_MSG_PREFIX} Apple platform detected")

    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    if(COREFOUNDATION_LIBRARY)
        message(STATUS "${GBHK_MSG_PREFIX} Found CoreFoundation: ${COREFOUNDATION_LIBRARY}")
        target_link_libraries(${TARGET_NAME} PRIVATE ${COREFOUNDATION_LIBRARY})
    else()
        message(FATAL_ERROR "${GBHK_MSG_PREFIX} Not found CoreFoundation")
    endif()

    find_library(CARBON_LIBRARY Carbon)
    if(CARBON_LIBRARY)
        message(STATUS "{GBHK_MSG_PREFIX} Found Carbon: ${CARBON_LIBRARY}")
        target_link_libraries(${TARGET_NAME} PRIVATE ${CARBON_LIBRARY})
    else()
        message(FATAL_ERROR "{GBHK_MSG_PREFIX} Not found Carbon")
    endif()

    if(NOT GBHK_DISABLE_HOOK)
        find_library(COREGRAPHICS_LIBRARY CoreGraphics)
        if(COREGRAPHICS_LIBRARY)
            message(STATUS "${GBHK_MSG_PREFIX} Found CoreGraphics: ${COREGRAPHICS_LIBRARY}")
            target_link_libraries(${TARGET_NAME} PRIVATE ${COREGRAPHICS_LIBRARY})
        else()
            message(FATAL_ERROR "${GBHK_MSG_PREFIX} Not found CoreGraphics")
        endif()
    endif()
endif()

if(LINUX)
    message(STATUS "${GBHK_MSG_PREFIX} Linux platform detected")

    if(NOT GBHK_DISABLE_REGISTER)
        find_path(X11_INCLUDE_DIR X11/Xlib.h PATHS usr/include)
        find_library(X11_LIBRARY NAMES X11 PATHS /usr/lib /usr/lib/x86_64-linux-gnu)
        if(X11_INCLUDE_DIR AND X11_LIBRARY)
            message(STATUS "${GBHK_MSG_PREFIX} Found X11: ${X11_LIBRARY}")
            include_directories(${X11_INCLUDE_DIR})
            target_link_libraries(${TARGET_NAME} PRIVATE ${X11_LIBRARY})
        else()
            message(FATAL_ERROR "${GBHK_MSG_PREFIX} Not found X11")
        endif()
    endif()
endif()

find_package(Threads REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads)

set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX d)

add_library(global_hotkey::global_hotkey ALIAS ${TARGET_NAME})

if(GBHK_BUILD_EXAMPLE)
    message(STATUS "${GBHK_MSG_PREFIX} Build example")
    add_subdirectory(example)
endif()
