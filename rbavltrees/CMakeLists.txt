cmake_minimum_required(VERSION 3.13)
project(rbavltrees)

set(CMAKE_CXX_STANDARD 14)

add_executable(rbavltrees main.cpp AvlTree.hpp TracingAvlTree.hpp TracingRbTree.hpp)