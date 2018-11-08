#http_parser/mytest/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

project (TinyWeb)

add_definitions("-Wall -std=c++11")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

SET (SOURCE_DIR ../)

include_directories(${SOURCE_DIR})

add_subdirectory("${SOURCE_DIR}/http_parser" http_parser.out)


# parseBody
add_executable(parseBody
    parseBody_test.cc
    )

target_link_libraries(parseBody
    http_parser_model
    )

# parseHeader
add_executable(parseHeader
    parseHeader_test.cc
    )

target_link_libraries(parseHeader
    http_parser_model
    )

# parseHeaderMeaning
add_executable(parseHeaderMeaning
    parseHeaderMeaning_test.cc
    )

target_link_libraries(parseHeaderMeaning
    http_parser_model
    )

# parseHost
add_executable(parseHost
    parseHost_test.cc
    )

target_link_libraries(parseHost
    http_parser_model
    )

# parseRequestLine
add_executable(parseRequestLine
    parseRequestLine_test.cc
    )

target_link_libraries(parseRequestLine
    http_parser_model
    )

# parseResponseLine
add_executable(parseResponseLine
    parseResponseLine_test.cc
    )

target_link_libraries(parseResponseLine
    http_parser_model
    )

# parseUrl
add_executable(parseUrl
    parseUrl_test.cc
    )

target_link_libraries(parseUrl
    http_parser_model
    )

