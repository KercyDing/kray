cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER clang++)
endif()

project(kray
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE KRAY_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.cc
    src/*.h
    src/*.hpp
    include/*.h
    include/*.hpp
)

add_executable(kray ${KRAY_SOURCES})

target_include_directories(kray PRIVATE include)

target_compile_options(
    kray
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

find_program(CLANG_FORMAT NAMES clang-format)

if(CLANG_FORMAT)
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT} -i ${KRAY_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formatting sources with clang-format"
        VERBATIM
    )
else()
    add_custom_target(
        format
        COMMAND ${CMAKE_COMMAND} -E echo "clang-format not found in PATH"
        COMMAND ${CMAKE_COMMAND} -E false
        VERBATIM
    )
endif()
