cmake_minimum_required(VERSION 2.4)
# cmake multi-version policy 
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(podcast)

find_package(PkgConfig)
pkg_check_modules(GLIB glib-2.0)
pkg_check_modules(XML xml2)

add_library(podcast
  podcastlist.c
  podcast.c
  podcasttrack.c
  util.c
)

include_directories(${GLIB_INCLUDE_DIRS} ${XML_INCLUDE_DIRS})
target_link_libraries(podcast debuglog urlfetch ${GLIB_LIBRARIES} ${XML_LIBRARIES})
