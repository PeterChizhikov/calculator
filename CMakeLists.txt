cmake_minimum_required(VERSION 3.10)

project(Calculator)


include(FetchContent)

FetchContent_Declare(
    mathLib
    GIT_REPOSITORY https://github.com/PeterChizhikov/mathLib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(mathLib)
add_executable(calc main.cpp)
target_link_libraries(calc PRIVATE mathLib)

find_program(CLANG_FORMAT NAMES clang-format clang-format-18 clang-format-17 clang-format-16)
if(CLANG_FORMAT)
    set(SOURCE_FILES main.cpp)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting sources with clang-format"
    )
else()
    message(WARNING "clang-format not found. Target 'format' will not be available.")
endif()

find_program(CLANG_TIDY NAMES clang-tidy clang-tidy-18 clang-tidy-17 clang-tidy-16)
if(CLANG_TIDY)
    set(CLANG_TIDY_CHECKS "-checks=*,-fuchsia-*,-hicpp-*,-readability-magic-numbers")
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY} ${SOURCE_FILES} -- -I${CMAKE_CURRENT_SOURCE_DIR} -std=c++17
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running clang-tidy on sources"
    )
else()
    message(WARNING "clang-tidy not found. Target 'tidy' will not be available.")
endif()
