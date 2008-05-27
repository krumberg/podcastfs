cmake_minimum_required(VERSION 2.4)

project(debuglog)

add_library(debuglog
  debuglog.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=gnu99")
target_link_libraries(debuglog})

