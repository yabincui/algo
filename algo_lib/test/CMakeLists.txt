
cmake_minimum_required (VERSION 2.6)

set (SOURCES geometry_test.cpp avl_tree_test.cpp skiplist_test.cpp astar_test.cpp)

add_compile_options(-std=c++11 -g)

add_executable(algo_test ${SOURCES})

target_include_directories(algo_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")


target_link_libraries(algo_test algo)
target_link_libraries(algo_test gtest gtest_main pthread)