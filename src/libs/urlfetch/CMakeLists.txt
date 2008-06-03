cmake_minimum_required(VERSION 2.4)

project(urlfetch)

find_package(PkgConfig)
pkg_check_modules(CURL libcurl)

include_directories(${CURL_INCLUDE_DIRS})

add_library(urlfetch
  urlfetch.c
)

target_link_libraries(urlfetch ${CURL_LIBRARIES})

