cmake_minimum_required(VERSION 3.25)

project(GBHK_Example_EventQueue LANGUAGES CXX)

file(GLOB_RECURSE SRC *.cpp)
add_executable(event_queue ${SRC})
