#main CMakeLists.txt

cmake_minimum_required(VERSION 2.8)
project (TinyWeb)

set (TINYWEB_VERSION "\"TnyWeb/0.0.8\"")
set (SHARED_MEMORY_ANON 1)
set (SHARED_MEMORY_DEVZERO 0)
set (SHARED_MEMORY_SYSVSHM 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/TinyWebConfig.h.in"
    "${PROJECT_SOURCE_DIR}/TinyWebConfig.h"
  )

add_definitions("-Wall -std=c++11")

SET (SOURCE_DIR ./)

include_directories(${SOURCE_DIR})

add_subdirectory("${SOURCE_DIR}/tiny_base" tiny_base.out)
add_subdirectory("${SOURCE_DIR}/tiny_core" tiny_core.out)
add_subdirectory("${SOURCE_DIR}/tiny_http" tiny_http.out)
add_subdirectory("${SOURCE_DIR}/tiny_struct" tiny_struct.out)


add_executable(TinyWeb
    main.cc)

target_link_libraries(TinyWeb
    tiny_http_model
    tiny_core_model
    tiny_struct_model
    tiny_base_model
    )
