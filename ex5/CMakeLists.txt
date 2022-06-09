cmake_minimum_required(VERSION 3.17)
project(ex5-unixraz)
set(CMAKE_CXX_STANDARD 14)

add_executable(ex5-unixraz main.cpp Activation.h Dense.h Digit.h Matrix.h MlpNetwork.h Matrix.cpp Activation.cpp Dense.cpp MlpNetwork.cpp)
