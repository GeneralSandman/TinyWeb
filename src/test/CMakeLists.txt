#test/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

add_subdirectory(../tiny_base) 
add_subdirectory(../tiny_core) 
add_subdirectory(../tiny_http) 
add_subdirectory(../tiny_struct) 

add_executable(Main)

target_link_libraries(Main
    tiny_base_model
    tiny_core_model
    tiny_http_model
    tiny_http_struct)
