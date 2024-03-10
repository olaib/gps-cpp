cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
include_directories(includes)
include_directories(src)

add_executable(${PROJECT_NAME} main.cpp)

