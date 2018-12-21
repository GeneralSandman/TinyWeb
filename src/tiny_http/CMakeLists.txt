#tiny_http/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

add_library(tiny_http_model STATIC
  cache.cc
  http_factory.cc
  http_protocol.cc
  proxyer.cc
  http.cc
  http_model_file.cc
  http_model_gzip.cc
  http_model_fcgi.cc
  http_parser.cc
  http_responser.cc
  http_time.cc
  str_t.cc
)
