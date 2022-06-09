cmake_minimum_required(VERSION 3.17)
project(ex6)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex6 main.cpp vl_vector.h helpers.h vl_string.h)
add_executable(ex6_presubmit presubmit.cpp vl_vector.h helpers.h vl_string.h)
add_executable(ex6_tests vl_vector.h ex6_tests.cpp)
add_executable(ex6_b_tests vl_string.h ex6_bonus_tests.cpp)