#main CMakeLists.txt

cmake_minimum_required(VERSION 2.8)
project (TinyWeb)

set (TINYWEB_VERSION "\"TnyWeb/0.0.10\"")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/TinyWebConfig.h.in"
    "${PROJECT_SOURCE_DIR}/TinyWebConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")


add_subdirectory(tiny_base) 
add_subdirectory(tiny_core) 
add_subdirectory(tiny_http) 
add_subdirectory(tiny_struct) 

add_executable(Main
    main.cc)

target_link_libraries(Main
    tiny_base_model
    tiny_core_model
    tiny_http_model
    tiny_http_struct)
