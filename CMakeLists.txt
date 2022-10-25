cmake_minimum_required(VERSION 3.10)

project(countdown_game)

set(CMAKE_CXX_STANDARD 20)

set(source_files
    src/solver.h
    src/solver.cpp
    src/fast_num.h
    src/fast_num.cpp
    src/node.h
    src/node.cpp
    src/optimizer.h
    src/optimizer.cpp
    src/printing.hpp
    src/utility.h
)
add_executable(conundrums src/conundrums.cpp ${source_files})