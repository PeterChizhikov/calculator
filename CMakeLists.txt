cmake_minimum_required(VERSION 3.10)

project(Calculator)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CTest)

set(COMPILE_COMMANDS_LINK "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json")
set(COMPILE_COMMANDS_FILE "${CMAKE_BINARY_DIR}/compile_commands.json")
if(UNIX)
    if(EXISTS "${COMPILE_COMMANDS_LINK}")
        if(NOT IS_SYMLINK "${COMPILE_COMMANDS_LINK}")
            file(REMOVE "${COMPILE_COMMANDS_LINK}")
        endif()
    endif()
    if(NOT EXISTS "${COMPILE_COMMANDS_LINK}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${COMPILE_COMMANDS_FILE}" "${COMPILE_COMMANDS_LINK}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
endif()

find_package(PostgreSQL REQUIRED)

set(HEADER_FILES
    include/Calculator.h
    include/Checker.h
    include/Parser.h 
    include/Printer.h
    include/Runner.h
    include/MathInfo.h
    include/Logger.h
    include/DatabaseProcessor.h
    include/CalculatorException.h
)

set(SOURCE_FILES 
    src/main.cpp
    src/Calculator.cpp
    src/Checker.cpp
    src/Parser.cpp
    src/Printer.cpp
    src/Runner.cpp
    src/Logger.cpp
    src/DatabaseProcessor.cpp
    src/CalculatorException.cpp
)

set(TEST_SOURCE_FILES
    tests/CalculatorTests.cpp
    tests/ServiceClientTests.cpp
)

set(TEST_APP_SOURCE_FILES
    src/Calculator.cpp
    src/Checker.cpp
    src/Logger.cpp
    src/Parser.cpp
    src/Printer.cpp
    src/Runner.cpp
    src/DatabaseProcessor.cpp
    src/CalculatorException.cpp
)

set(SERVICE_SOURCE_FILES
    src/service_main.cpp
    src/Calculator.cpp
    src/Checker.cpp
    src/Logger.cpp
    src/DatabaseProcessor.cpp
    src/CalculatorException.cpp
    src/CalculationProcessor.cpp
    src/CalculatorServer.cpp
)

include(FetchContent)

FetchContent_Declare(
    mathLib
    GIT_REPOSITORY https://github.com/PeterChizhikov/mathLib.git
    GIT_TAG main
)

FetchContent_Declare(
    json 
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_Declare(
    googletest 
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)

FetchContent_Declare(
  cppzmq
  GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
  GIT_TAG v4.10.0
)

set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPPZMQ_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CPPZMQ_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(cppzmq)
FetchContent_MakeAvailable(mathLib)
FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(spdlog)

add_subdirectory(cache)
add_executable(calc ${SOURCE_FILES})

target_include_directories(calc PRIVATE include)
target_link_libraries(calc
    PRIVATE
    mathLib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    PostgreSQL::PostgreSQL
    cache_lib
)

install(TARGETS calc RUNTIME DESTINATION bin)

add_executable(calc_service ${SERVICE_SOURCE_FILES})

target_include_directories(calc_service PRIVATE include)

target_link_libraries(calc_service
    PRIVATE
    mathLib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    PostgreSQL::PostgreSQL
    cache_lib
    cppzmq
)
install(TARGETS calc_service RUNTIME DESTINATION bin)



if(BUILD_TESTING)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    add_executable(calc_tests
        ${TEST_SOURCE_FILES}
        ${TEST_APP_SOURCE_FILES}
            src/CalculationProcessor.cpp
            src/CalculatorServer.cpp
    )
    find_package(PostgreSQL REQUIRED)
    target_include_directories(calc_tests PRIVATE include)
    target_link_libraries(calc_tests
        PRIVATE
        mathLib
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        GTest::gtest_main
        PostgreSQL::PostgreSQL
        cache_lib
        cppzmq
    )
    gtest_discover_tests(calc_tests)
endif()

find_program(CLANG_FORMAT NAMES clang-format clang-format-18 clang-format-17 clang-format-16)
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES} ${HEADER_FILES} ${TEST_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting sources with clang-format"
    )
else()
    message(WARNING "clang-format not found. Target 'format' will not be available.")
endif()

find_program(CLANG_TIDY NAMES clang-tidy clang-tidy-18 clang-tidy-17 clang-tidy-16)
if(CLANG_TIDY)
    set(TIDY_SOURCES "")
    foreach(F ${SOURCE_FILES})
        list(APPEND TIDY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${F})
    endforeach()
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY} ${TIDY_SOURCES}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running clang-tidy on sources"
    )
else()
    message(WARNING "clang-tidy not found. Target 'tidy' will not be available.")
endif()

if(CLANG_FORMAT AND CLANG_TIDY)
    add_dependencies(calc format tidy)
    if(BUILD_TESTING)
        add_dependencies(calc_tests format)
    endif()
endif()


install(FILES packaging/calculator.service
    DESTINATION lib/systemd/system
)

install(FILES packaging/calculator.env
    DESTINATION etc/calculator
)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "calculator-service")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "petr.chizhikov@bk.ru")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "student")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpq5, libzmq5")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_SOURCE_DIR}/packaging/postinst;${CMAKE_CURRENT_SOURCE_DIR}/packaging/prerm"
)

include(CPack)