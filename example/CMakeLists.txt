cmake_minimum_required(VERSION 3.25)

project(GlobalHotkeyExample LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT TARGET global_hotkey)
    find_package(global_hotkey REQUIRED)
endif()

option(GBHK_EXAMPLE_USE_HOOK "Build example using hook Global Hotkey Manager" OFF)
if(GBHK_EXAMPLE_USE_HOOK)
    message(STATUS "${GBHK_MSG_PREFIX} Build example using hook Global Hotkey Manager")
endif()

if(GBHK_DISABLE_REGISTER AND NOT GBHK_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot build example using register Global Hotkey Manager with GBHK_DISABLE_REGISTER")
elseif(GBHK_DISABLE_HOOK AND GBHK_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot build example using hook Global Hotkey Manager with GBHK_DISABLE_HOOK")
endif()

add_executable(example1 example1.cpp)
target_link_libraries(example1 PRIVATE global_hotkey::global_hotkey)

if(GBHK_EXAMPLE_USE_HOOK)
    target_compile_definitions(example1 PRIVATE GLOBAL_HOTKEY_EXAMPLE_USE_HOOK)

    if(APPLE)
        # For AXIsProcessTrustedWithOptions.
        find_library(AX_LIBRARY ApplicationServices)
        if(AX_LIBRARY)
            message(STATUS "${GBHK_MSG_PREFIX} Found ApplicationServices: ${AX_LIBRARY}")
            target_link_libraries(example1 PRIVATE ${AX_LIBRARY})
        else()
            message(STATUS "${GBHK_MSG_PREFIX} Not found ApplicationServices")
        endif()
    endif()
endif()
