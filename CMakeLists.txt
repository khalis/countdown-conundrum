cmake_minimum_required(VERSION 3.10)

project(countdown_game)

find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(countdown_conundrum numbers.cpp solver.cpp utility.h)
target_link_libraries(countdown_conundrum fmt::fmt)

add_executable(countdown_numbers_test numbers_test.cpp)
target_link_libraries(countdown_numbers_test fmt::fmt)

add_executable(countdown_template_generation template_generation.cpp)
target_link_libraries(countdown_template_generation fmt::fmt)

