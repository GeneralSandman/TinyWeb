#test/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

SET (SOURCE_DIR ../)

include_directories(${SOURCE_DIR})

add_subdirectory("${SOURCE_DIR}/http_parser" http_parser.out)
add_subdirectory("${SOURCE_DIR}/tiny_base" tiny_base.out)
add_subdirectory("${SOURCE_DIR}/tiny_core" tiny_core.out)
add_subdirectory("${SOURCE_DIR}/tiny_http" tiny_http.out)
add_subdirectory("${SOURCE_DIR}/tiny_struct" tiny_struct.out)


# processpool_test
add_executable(processpool_test
    processpool_test.cc
    )

target_link_libraries(processpool_test
    tiny_core_model
    tiny_http_model
    tiny_struct_model
    tiny_base_model
    )

# memorypool_test
add_executable(memorypool_test
    memorypool_test.cc
    )

target_link_libraries(memorypool_test
    tiny_core_model
    tiny_http_model
    tiny_struct_model
    tiny_base_model
    )

# sdstr_test
add_executable(sdstr_test
    sdstr_test.cc
    )

target_link_libraries(sdstr_test
    tiny_base_model
    tiny_struct_model
    )

# gzip_test
add_executable(gzip_test
    gzip_test.cc
    )

target_link_libraries(gzip_test
    z
    )

