#http_parser/mytest/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

project (TinyWeb)

add_definitions("-Wall -std=c++11")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

SET (SOURCE_DIR ../)

include_directories(${SOURCE_DIR})

add_subdirectory("${SOURCE_DIR}/http_parser" http_parser.out)

add_executable(parseUrl
    parseUrl_test.cc)

target_link_libraries(parseUrl
    http_parser_model
    )
