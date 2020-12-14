project(app C)
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

include(../../cmake/lib.cmake)

add_executable(app main.c sample.c)
