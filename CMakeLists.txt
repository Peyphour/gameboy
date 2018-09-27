cmake_minimum_required(VERSION 3.10)
project(gameboy C)

set(CMAKE_C_STANDARD 11)

add_executable(gameboy main.c main.h core.c core.h ops.h ops.c)