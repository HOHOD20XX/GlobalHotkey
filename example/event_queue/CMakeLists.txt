cmake_minimum_required(VERSION 3.25)

project(event_queue LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    event_work.cpp
    utility.cpp
    main.cpp
)
