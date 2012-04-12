cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(integration-tests)

add_executable(integration-tests
  main.cpp
  podcast-integration-tests.cpp
)

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${XML_INCLUDE_DIRS}/..)

include_directories(${CMAKE_SOURCE_DIR}/libs)

target_link_libraries(
  integration-tests
  podcast
  gtest
)
