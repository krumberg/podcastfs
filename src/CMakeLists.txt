cmake_minimum_required(VERSION 2.4)

project(cpodcastfs)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TOOLCHAIN_C_FLAGS}")

add_subdirectory(libs)

# When building using MacPorts the link path is not added automatically
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L ${GLIB_LIBRARY_DIRS}")

add_subdirectory(apps)

