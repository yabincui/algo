
cmake_minimum_required (VERSION 2.6)

set (SOURCES partial_persistence_fat_node.cpp)

add_compile_options(-std=c++11 -g)

add_executable(fat_node_test ${SOURCES})

target_include_directories(fat_node_test PUBLIC)


target_link_libraries(fat_node_test gtest_main gtest pthread)
