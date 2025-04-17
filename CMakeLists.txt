cmake_minimum_required(VERSION 3.12)

project(GlobalHotkey LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)

if (NOT DEFINED GBHK_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(GBHK_MASTER_PROJECT ON)
    else ()
        set(GBHK_MASTER_PROJECT OFF)
    endif ()
endif ()

if (WIN32)
    set(GBHK_WINDOWS ON)
else()
    set(GBHK_WINDOWS OFF)
endif()

# Build shared library option.
option(GBHK_BUILD_SHARED "Build shared library" OFF)

# Disable register hotkey option.
option(GBHK_DISABLE_REGISTER "Disable register global hotkey" OFF)

# Disable hook hotkey option.
option(GBHK_DISABLE_HOOK "Disable hook global hotkey" !GBHK_WINDOWS)

if (NOT GBHK_WINDOWS AND NOT GBHK_DISABLE_HOOK)
    message(FATAL_ERROR "Hook global hotkey is only supported on Windows")
endif()

# Build example option.
option(GBHK_BUILD_EXAMPLE "Build example" ${GBHK_MASTER_PROJECT})

# Only windows is supported for now.
# Maybe be removed in the future.
if (NOT WIN32)
    message(FATAL_ERROR "GlobalHotkey only supports Windows")
endif()

if (GBHK_DISABLE_HOOK AND GBHK_DISABLE_REGISTER)
    message(FATAL_ERROR "Cannot set Both GBHK_DISABLE_HOOK and GBHK_DISABLE_REGISTER to ON")
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
