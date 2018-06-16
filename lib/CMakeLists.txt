cmake_minimum_required(VERSION 3.5.1)
project(lib)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES huffman.cpp
        tree.cpp)

set(TEST	tests/huffman_testing.cpp
        tests/gtest/gtest-all.cc
        tests/gtest/gtest.h
        tests/gtest/gtest_main.cc)

set(HEADERS headers/tree.h
        headers/huffman.h)

add_library(lib ${SOURCES} ${HEADERS})

install (TARGETS lib DESTINATION lib)

install (FILES ${HEADERS} DESTINATION include/lib)

add_executable(testing ${SOURCES} ${HEADERS} ${TEST})

target_link_libraries(testing -lpthread)