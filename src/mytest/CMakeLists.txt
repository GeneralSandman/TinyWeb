#http_parser/mytest/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

project (TinyWeb)

set (TINYWEB_VERSION "\"TinyWeb/0.0.10\"")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/TinyWebConfig.h.in"
    "${PROJECT_SOURCE_DIR}/TinyWebConfig.h"
  )

add_definitions("-Wall -std=c++11")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

SET (SOURCE_DIR ../)

include_directories(${SOURCE_DIR})

add_subdirectory("${SOURCE_DIR}/http_parser" http_parser.out)
add_subdirectory("${SOURCE_DIR}/tiny_base" tiny_base.out)
add_subdirectory("${SOURCE_DIR}/tiny_core" tiny_core.out)
add_subdirectory("${SOURCE_DIR}/tiny_http" tiny_http.out)
add_subdirectory("${SOURCE_DIR}/tiny_struct" tiny_struct.out)


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

# responser_test
add_executable(responser_test
    responser_test.cc
    )

target_link_libraries(responser_test
    http_parser_model
    tiny_struct_model
    )

# model_file_test
add_executable(model_file_test
    model_file_test.cc
    )

target_link_libraries(model_file_test
    http_parser_model
    )

# httpTime_test
add_executable(httpTime_test
    httpTime_test.cc
    )

target_link_libraries(httpTime_test
    )



