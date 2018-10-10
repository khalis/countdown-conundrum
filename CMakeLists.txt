cmake_minimum_required(VERSION 3.10)

project(countdown_game)

set(CMAKE_CXX_STANDARD 17)

add_executable(countdown_test test.cpp)

add_executable(countdown_conundrum numbers.cpp solver.cpp utility.h)
target_link_libraries(countdown_conundrum fmt)

add_executable(countdown_numbers_test numbers_test.cpp)
target_link_libraries(countdown_numbers_test fmt)

add_executable(countdown_template_generation template_generation.cpp)
target_link_libraries(countdown_template_generation fmt)