cmake_minimum_required(VERSION 3.29)

project(dkvstore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(dkvstore dkvtest.cpp)
