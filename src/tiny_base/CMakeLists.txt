#tiny_base/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

add_library(tiny_base_model STATIC
  api.cc
  buffer.cc
  condition.cc
  configer.cc
  exception.cc
  list_timer.cc
  log.cc
  file.cc
  memorypool.cc
  messagequeue.cc
  semaphore.cc
  sharedmemory.cc
  signalmanager.cc
)
