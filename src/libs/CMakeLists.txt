cmake_minimum_required(VERSION 2.4)

project(libs)

include_directories(${libs_SOURCE_DIR})

add_subdirectory(urlfetch)
add_subdirectory(debuglog)
add_subdirectory(podcast)
