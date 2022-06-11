cmake_minimum_required(VERSION 3.20)
project(TiktokGen)

set(CMAKE_CXX_STANDARD 14)

include_directories(modules/imgui)
add_library(IMGUI modules/imgui)
add_subdirectory(modules)
add_executable(TiktokGen main.cpp)
