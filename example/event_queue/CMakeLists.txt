cmake_minimum_required(VERSION 3.25)

project(GBHK_Example_EventQueue LANGUAGES CXX)

add_executable(event_queue
    event_work.cpp
    utility.cpp
    main.cpp)
