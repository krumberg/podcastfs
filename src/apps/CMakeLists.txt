cmake_minimum_required(VERSION 2.4)

project(apps)

include_directories(${libs_SOURCE_DIR})
include_directories(${apps_SOURCE_DIR})

include_directories( ${GLIB_INCLUDE_DIRS} ${XML_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} )

add_subdirectory(testxmlparsing)
add_subdirectory(testpodcastlist)
add_subdirectory(pcmount)
