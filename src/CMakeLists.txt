cmake_minimum_required(VERSION 2.4)

project(cpodcastfs)

add_subdirectory(libs)

if (NOT "${GLIB_LIBRARY_DIRS}" STREQUAL "")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L ${GLIB_LIBRARY_DIRS}")
endif()

add_subdirectory(apps)

