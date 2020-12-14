project(utils C)
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

include(../cmake/lib.cmake)

file(
    GLOB_RECURSE
    SOURCES
    ./*.c
)

add_library(utils ${SOURCES})
target_link_libraries(utils)
target_include_directories(utils PUBLIC ${SRC_ROOT})
