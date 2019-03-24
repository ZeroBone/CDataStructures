cmake_minimum_required(VERSION 3.13)
project(1_2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(1_2 main.cpp queue.c++ queue.h)