cmake_minimum_required(VERSION 3.12)

project(GlobalHotkeyExample LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT TARGET global_hotkey)
    find_package(global_hotkey REQUIRED)
endif()

option(BUILD_EXAMPLE_USE_HOOK "Use hook global hotkey to build example" OFF)

add_executable(example1 example1.cpp)

if (GBHK_DISABLE_REGISTER AND NOT BUILD_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "Cannot build example using register global hotkey with GBHK_DISABLE_REGISTER")
elseif (GBHK_DISABLE_HOOK AND BUILD_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "Cannot build example using hook global hotkey with GBHK_DISABLE_HOOK")
endif()

if (BUILD_EXAMPLE_USE_HOOK)
    target_compile_definitions(example1 PRIVATE GLOBAL_HOTKEY_EXAMPLE_USE_HOOK)
endif()

target_link_libraries(example1 PRIVATE global_hotkey::global_hotkey)
