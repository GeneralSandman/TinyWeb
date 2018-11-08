#http_parser/mytest/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

add_library(tiny_struct_model STATIC
    dict_t.cc
    list_t.cc
    rbtree_t.cc
    string_t.cc
)
