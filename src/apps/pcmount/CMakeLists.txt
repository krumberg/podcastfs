cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(pcmount)

find_package(PkgConfig)
pkg_check_modules(GLIB glib-2.0)
pkg_check_modules(FUSE fuse)
pkg_check_modules(XML xml2)
pkg_check_modules(CURL libcurl)

add_executable(pcmount
  main.c
  fuse.c
)

include_directories( ${FUSE_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS} ${XML_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=gnu99 -D_FILE_OFFSET_BITS=64")
target_link_libraries(pcmount podcast ${FUSE_LIBRARIES} ${GLIB_LIBRARIES} ${XML_LIBRARIES} ${CURL_LIBRARIES})

