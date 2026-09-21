cmake_minimum_required(VERSION 3.28)

project(kray VERSION 0.1.0 LANGUAGES CXX)

find_package(SDL3 CONFIG REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PROJECT_MODULES CONFIGURE_DEPENDS src/*.cppm)

add_executable(kray ${PROJECT_SOURCES})

target_link_libraries(kray PRIVATE SDL3::SDL3)

target_sources(
    kray
    PRIVATE
    FILE_SET CXX_MODULES
    FILES ${PROJECT_MODULES}
)

set_target_properties(kray PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
target_compile_options(kray PRIVATE -Wall -Wextra -Wpedantic)
