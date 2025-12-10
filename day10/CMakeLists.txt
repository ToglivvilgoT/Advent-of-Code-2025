cmake_minimum_required(VERSION 4.1)

project(aoc)

set(CMAKE_CXX_STANDARD 23)

add_executable(aoc solution.cpp ../lib.cpp)

target_include_directories(aoc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)