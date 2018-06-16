cmake_minimum_required(VERSION 3.10)

project(HUFFMAN)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O3")

add_subdirectory(lib)

add_executable(main main.cpp lib/huffman.cpp lib/tree.cpp)