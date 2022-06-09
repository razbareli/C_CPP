cmake_minimum_required(VERSION 3.17)
project(ex4-unixraz C)

set(CMAKE_C_STANDARD 99)

add_executable(ex4-unixraz main.c vector.c vector.h hashmap.h hash_funcs.h hashmap.c pair.c pair.h test_suite.h test_suite.c test_pairs.h)