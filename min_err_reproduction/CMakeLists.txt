cmake_minimum_required(VERSION 3.25)

project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(TARGET_NAME test)

add_executable(${TARGET_NAME} main.cpp)

find_library(COREFOUNDATION_LIBRARY CoreFoundation)
target_link_libraries(${TARGET_NAME} PRIVATE ${COREFOUNDATION_LIBRARY})

find_library(CARBON_LIBRARY Carbon)
target_link_libraries(${TARGET_NAME} PRIVATE ${CARBON_LIBRARY})
