cmake_minimum_required(VERSION 3.10)

project(countdown_game)

set(CMAKE_CXX_STANDARD 20)

set(source_files
    conundrums.cpp
    solver.h
    solver.cpp
    fast_num.h
    fast_num.cpp
    node.h
    node.cpp
    optimizer.h
    optimizer.cpp
    printing.hpp
    utility.h
)
add_executable(conundrums ${source_files})