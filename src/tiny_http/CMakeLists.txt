#tiny_http/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

add_library(tiny_http_model STATIC
  cache.cc
  http.cc
  http_factory.cc
  http_protocol.cc
  parser.cc
  proxyer.cc
  responser.cc
)
