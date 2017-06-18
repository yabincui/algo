
cmake_minimum_required(VERSION 2.6)

project(AlgoLib)

add_compile_options(-std=c++11 -g)

set(SOURCES geometry.h geometry.cpp avl_tree.h)

add_library(algo STATIC ${SOURCES})

add_subdirectory(test)