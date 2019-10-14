cmake_minimum_required(VERSION 3.15)
project(ReadMusic C)

set(CMAKE_C_STANDARD 99)

add_executable(ReadMusic main.c fileHeader.h fileHeader.c)