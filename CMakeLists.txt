cmake_minimum_required(VERSION 3.5.1)

project(HUFFMAN)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -O3 -flto")

add_subdirectory(lib)

add_executable(main main.cpp lib/huffman.cpp lib/tree.cpp)