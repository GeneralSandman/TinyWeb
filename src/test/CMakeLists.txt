#test/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

SET (SOURCE_DIR ../)

include_directories(${SOURCE_DIR})

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
    pthread
    )

# process_test
add_executable(process_test
    process_test.cc
    )

target_link_libraries(process_test
    tiny_core_model
    tiny_http_model
    tiny_struct_model
    tiny_base_model
    )

# slave_test
add_executable(slave_test
    slave_test.cc
    )

target_link_libraries(slave_test
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

# sharedmemory_test
add_executable(sharedmemory_test
    sharedmemory_test.cc
    )

target_link_libraries(sharedmemory_test
    tiny_struct_model
    tiny_base_model
    pthread
    )

# sync_test
add_executable(sync_test
    sync_test.cc
    )

target_link_libraries(sync_test
    tiny_struct_model
    tiny_base_model
    pthread
    )

# file_test
add_executable(file_test
    file_test.cc
    )

target_link_libraries(file_test
    tiny_http_model
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

# configer_test
add_executable(configer_test
    configer_test.cc
    )

target_link_libraries(configer_test
    tiny_base_model
    )

# main_test
add_executable(main_test
    main_test.cc
    )

target_link_libraries(main_test
    tiny_core_model
    tiny_http_model
    tiny_base_model
    )

# daemon_test
add_executable(daemon_test
    daemon_test.cc
    )

target_link_libraries(daemon_test
    tiny_core_model
    tiny_http_model
    tiny_struct_model
    tiny_base_model
    )

# client_test
add_executable(client_test
    client_test.cc
    )

target_link_libraries(client_test
    tiny_core_model
    tiny_http_model
    tiny_struct_model
    tiny_base_model
    )

# clientpool_test
add_executable(clientpool_test
    clientpool_test.cc
    )

target_link_libraries(clientpool_test
    tiny_core_model
    tiny_http_model
    tiny_struct_model
    tiny_base_model
    )

# fcgi_client_test
add_executable(fcgi_client_test
    fcgi_client_test.cc
    )

target_link_libraries(fcgi_client_test
    tiny_core_model
    tiny_http_model
    tiny_struct_model
    tiny_base_model
    )
