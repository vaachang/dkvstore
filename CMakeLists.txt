cmake_minimum_required(VERSION 3.29)

project(dkvstore
        VERSION 0.1.0
        DESCRIPTION "Distributed Key-Value Store")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

include(CTest)

add_subdirectory(src)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
