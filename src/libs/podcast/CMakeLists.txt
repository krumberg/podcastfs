cmake_minimum_required(VERSION 2.4)

project(podcast)

find_package(PkgConfig)
pkg_check_modules(GLIB glib-2.0)
pkg_check_modules(XML xml2)
pkg_check_modules(CURL libcurl)

add_library(podcast
  podcastlist.c
  podcast.c
  podcasttrack.c
)

include_directories(${GLIB_INCLUDE_DIRS} ${XML_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=gnu99")
target_link_libraries(podcast debuglog ${GLIB_LIBRARIES} ${XML_LIBRARIES} ${CURL_LIBRARIES})

