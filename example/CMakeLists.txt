cmake_minimum_required(VERSION 3.25)

project(GlobalHotkeyExample LANGUAGES CXX)

if(NOT TARGET global_hotkey)
    find_package(global_hotkey REQUIRED)
endif()

option(GBHK_EXAMPLE_USE_HOOK "Build example using hook Global Hotkey Manager" OFF)
if(GBHK_EXAMPLE_USE_HOOK)
    message(STATUS "${GBHK_MSG_PREFIX} Build example using hook Global Hotkey Manager.")
endif()

if(GBHK_DISABLE_REGISTER AND NOT GBHK_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot build example using register Global Hotkey Manager with GBHK_DISABLE_REGISTER.")
elseif(GBHK_DISABLE_HOOK AND GBHK_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot build example using hook Global Hotkey Manager with GBHK_DISABLE_HOOK.")
endif()

link_libraries(global_hotkey::global_hotkey)
if(GBHK_EXAMPLE_USE_HOOK)
    add_compile_definitions(GLOBAL_HOTKEY_EXAMPLE_USE_HOOK)
    if(APPLE)
        # For AXIsProcessTrustedWithOptions.
        find_library(AX_LIBRARY ApplicationServices)
        if(AX_LIBRARY)
            message(STATUS "${GBHK_MSG_PREFIX} Found ApplicationServices: ${AX_LIBRARY}.")
            link_libraries(${AX_LIBRARY})
        else()
            message(STATUS "${GBHK_MSG_PREFIX} Not found ApplicationServices.")
        endif()
    endif()
endif()

option(GBHK_EXAMPLE_BUILD_BASE_EXAMPLE "Build base example" ON)
option(GBHK_EXAMPLE_BUILD_EVENT_QUEUE "Build event queue example" ON)

if(GBHK_EXAMPLE_BUILD_BASE_EXAMPLE)
    add_subdirectory(base_example)
endif()

if (GBHK_EXAMPLE_BUILD_EVENT_QUEUE)
    add_subdirectory(event_queue)
endif()
