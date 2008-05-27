cmake_minimum_required(VERSION 2.4)

project(apps)

include_directories(${libs_SOURCE_DIR})
include_directories(${apps_SOURCE_DIR})

add_subdirectory(pcmount)

