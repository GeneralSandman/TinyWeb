#tiny_core/CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

add_library(tiny_core_model STATIC
  accepter.cc
  channel.cc
  client.cc
  connection.cc
  connector.cc
  currentthread.cc
  defer.cc
  epoller.cc
  eventloop.cc
  factory.cc
  master.cc
  netaddress.cc
  process.cc
  processpool.cc
  protocol.cc
  server.cc
  slave.cc
  socket.cc
  socketpair.cc
  status.cc
  thread.cc
  threadpool.cc
  time.cc
  timer.cc
  timerqueue.cc
)
