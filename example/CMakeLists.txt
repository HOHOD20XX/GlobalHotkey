cmake_minimum_required(VERSION 3.25)

project(global_hotkey_examples LANGUAGES CXX)

option(GBHK_EXAMPLE_USE_HOOK "Build example using hook Global Hotkey Manager" OFF)
if(GBHK_EXAMPLE_USE_HOOK)
    message(STATUS "${GBHK_MSG_PREFIX} Build example using hook Global Hotkey Manager.")
endif()

if(GBHK_DISABLE_REGISTER AND NOT GBHK_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot build example using register Global Hotkey Manager with GBHK_DISABLE_REGISTER.")
elseif(GBHK_DISABLE_HOOK AND GBHK_EXAMPLE_USE_HOOK)
    message(FATAL_ERROR "${GBHK_MSG_PREFIX} Cannot build example using hook Global Hotkey Manager with GBHK_DISABLE_HOOK.")
endif()

option(GBHK_EXAMPLE_BUILD_SIMPLE "Build simple example" ON)
option(GBHK_EXAMPLE_BUILD_EVENT_QUEUE "Build event queue example" ON)

if(GBHK_EXAMPLE_BUILD_SIMPLE)
    add_subdirectory(simple)
endif()

if(GBHK_EXAMPLE_BUILD_EVENT_QUEUE)
    add_subdirectory(event_queue)
endif()
