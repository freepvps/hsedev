project(app C)
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

include(../../../cmake/lib.cmake)

add_subdirectory(${SRC_ROOT}/utils utils)
add_executable(app main.c)
target_link_libraries(app utils)
