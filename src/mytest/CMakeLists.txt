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

add_subdirectory("${SOURCE_DIR}/tiny_base" tiny_base.out)
add_subdirectory("${SOURCE_DIR}/tiny_core" tiny_core.out)
add_subdirectory("${SOURCE_DIR}/tiny_http" tiny_http.out)
add_subdirectory("${SOURCE_DIR}/tiny_struct" tiny_struct.out)


# all_test
add_executable(all_test
    all_test.cc
    )

target_link_libraries(all_test
    tiny_http_model
    )

# modelFile_test
add_executable(modelFile_test
    modelFile_test.cc
    )

target_link_libraries(modelFile_test
    tiny_http_model
    tiny_base_model
    )

# modelGzip_test
add_executable(modelGzip_test
    modelGzip_test.cc
    )

target_link_libraries(modelGzip_test
    tiny_http_model
    tiny_base_model
    tiny_struct_model
    z
    )

# modelGzip_test
add_executable(modelChunked_test
    modelChunked_test.cc
    )

target_link_libraries(modelGzip_test
    tiny_http_model
    tiny_base_model
    )

# parseBody
add_executable(parseBody
    parseBody_test.cc
    )

target_link_libraries(parseBody
    tiny_http_model
    )

# parseHeader
add_executable(parseHeader
    parseHeader_test.cc
    )

target_link_libraries(parseHeader
    tiny_http_model
    )

# parseHeaderMeaning
add_executable(parseHeaderMeaning
    parseHeaderMeaning_test.cc
    )

target_link_libraries(parseHeaderMeaning
    tiny_http_model
    )

# parseHost
add_executable(parseHost
    parseHost_test.cc
    )

target_link_libraries(parseHost
    tiny_http_model
    )

# parseHttpTime_test
add_executable(parseHttpTime_test
    parseHttpTime_test.cc
    )

target_link_libraries(parseHttpTime_test
    tiny_http_model
    tiny_struct_model
    )

# parseRequestLine
add_executable(parseRequestLine
    parseRequestLine_test.cc
    )

target_link_libraries(parseRequestLine
    tiny_http_model
    )

# parseResponseLine
add_executable(parseResponseLine
    parseResponseLine_test.cc
    )

target_link_libraries(parseResponseLine
    tiny_http_model
    )

# parseUrl
add_executable(parseUrl
    parseUrl_test.cc
    )

target_link_libraries(parseUrl
    tiny_http_model
    )

# responser_test
add_executable(responser_test
    responser_test.cc
    )

target_link_libraries(responser_test
    tiny_http_model
    tiny_struct_model
    tiny_base_model
    )

# chain_test
add_executable(chain_test
    chain_test.cc
    )

target_link_libraries(chain_test
    tiny_http_model
    )

