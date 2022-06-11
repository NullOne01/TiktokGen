cmake_minimum_required(VERSION 3.20)
project(TiktokGen)

set(CMAKE_CXX_STANDARD 14)

include_directories(modules)
add_subdirectory(modules)
add_executable(TiktokGen main.cpp)
