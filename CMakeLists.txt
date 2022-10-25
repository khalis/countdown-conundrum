cmake_minimum_required(VERSION 3.10)

project(countdown_game)

set(CMAKE_CXX_STANDARD 20)

add_library(node src/node.cpp)
target_include_directories(node PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/include)

add_library(fast_num src/fast_num.cpp)
target_include_directories(fast_num PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/include)
target_link_libraries(fast_num node)

add_library(optimizer src/optimizer.cpp)
target_include_directories(optimizer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/include)
target_link_libraries(optimizer node)

add_library(solver src/solver.cpp)
target_include_directories(solver PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/include)

add_executable(conundrums src/conundrums.cpp)
target_link_libraries(conundrums fast_num solver optimizer)