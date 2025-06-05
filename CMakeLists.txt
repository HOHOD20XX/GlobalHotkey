cmake_minimum_required(VERSION 3.12)

project(GlobalHotkey LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)

set(GBHK_MSG_PREFIX "[GlobalHotkey]")

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} In-source builds are not allowed. Please make a new directory (e.g. build) and run CMake from there.")
endif()

if (NOT DEFINED GBHK_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(GBHK_MASTER_PROJECT ON)
    else ()
        set(GBHK_MASTER_PROJECT OFF)
    endif ()
endif ()

if (WIN32)
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
endif()

# Build shared library option.
option(GBHK_BUILD_SHARED "Build shared library" OFF)
if (GBHK_BUILD_SHARED)
    message(STATUS "${GBHK_MSG_PREFIX} Building shared library")
else()
    message(STATUS "${GBHK_MSG_PREFIX} Building static library")
endif()

# Disable register hotkey option.
option(GBHK_DISABLE_REGISTER "Disable register global hotkey" OFF)
if (GBHK_DISABLE_REGISTER)
    message(STATUS "${GBHK_MSG_PREFIX} Disabling register global hotkey")
endif()

# Disable hook hotkey option.
option(GBHK_DISABLE_HOOK "Disable hook global hotkey" !WIN32)
if (GBHK_DISABLE_HOOK)
    message(STATUS "${GBHK_MSG_PREFIX} Disabling hook global hotkey")
endif()

if (NOT WIN32 AND NOT GBHK_DISABLE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Hook global hotkey is only supported on Windows")
endif()

# Build example option.
option(GBHK_BUILD_EXAMPLE "Build example" ${GBHK_MASTER_PROJECT})
if (GBHK_BUILD_EXAMPLE)
    message(STATUS "${GBHK_MSG_PREFIX} Building example")
else()
    message(STATUS "${GBHK_MSG_PREFIX} Not building example")
endif()

# Only Windows is supported for now.
# Maybe be removed in the future.
if (NOT WIN32)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} GlobalHotkey only supports Windows")
endif()

if (GBHK_DISABLE_HOOK AND GBHK_DISABLE_REGISTER)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot set Both GBHK_DISABLE_HOOK and GBHK_DISABLE_REGISTER to ON")
endif()

find_package(Threads REQUIRED)

if (GBHK_BUILD_SHARED)
    add_library(global_hotkey SHARED ${SRC})
    target_compile_definitions(global_hotkey PUBLIC GLOBAL_HOTKEY_BUILD_SHARED)
else()
    add_library(global_hotkey STATIC ${SRC})
endif()

if (GBHK_DISABLE_REGISTER)
    target_compile_definitions(global_hotkey PUBLIC GLOBAL_HOTKEY_NOREGISTER)
endif()

if (GBHK_DISABLE_HOOK)
    target_compile_definitions(global_hotkey PUBLIC GLOBAL_HOTKEY_NOHOOK)
endif()

target_link_libraries(global_hotkey PUBLIC Threads::Threads)

set_target_properties(global_hotkey PROPERTIES DEBUG_POSTFIX d)

add_library(global_hotkey::global_hotkey ALIAS global_hotkey)

if (GBHK_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
