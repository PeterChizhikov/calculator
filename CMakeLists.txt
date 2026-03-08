cmake_minimum_required(VERSION 3.10)

project(Calculator)

include(FetchContent)

FetchContent_Declare(
    math_lib
    GIT_REPOSITORY 
    GIT_TAG main
)
FetchContent_MakeAvailable(mathLib)

add_executable(calc main.cpp)
