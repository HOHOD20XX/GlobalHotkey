cmake_minimum_required(VERSION 3.25)

project(GlobalHotkeyExample LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT TARGET global_hotkey)
    find_package(global_hotkey REQUIRED)
endif()

option(GBHK_EXAMPLE_USE_HOOK "Build example using hook global hotkey manager" OFF)
if (GBHK_EXAMPLE_USE_HOOK)
    message(STATUS "${GBHK_MSG_PREFIX} Build example using hook global hotkey manager")
endif()

add_executable(example1 example1.cpp)

if (GBHK_DISABLE_REGISTER AND NOT GBHK_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot build example using register global hotkey manager with GBHK_DISABLE_REGISTER")
elseif (GBHK_DISABLE_HOOK AND GBHK_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot build example using hook global hotkey manager with GBHK_DISABLE_HOOK")
endif()

if (GBHK_EXAMPLE_USE_HOOK)
    target_compile_definitions(example1 PRIVATE GLOBAL_HOTKEY_EXAMPLE_USE_HOOK)
endif()

target_link_libraries(example1 PRIVATE global_hotkey::global_hotkey)
