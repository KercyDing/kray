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

add_executable(
    kray
    src/main.cpp
)

target_compile_options(
    kray
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
