cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(testseekanddownload)

find_package(PkgConfig)
pkg_check_modules(CURL libcurl)

include_directories(${CURL_INCLUDE_DIRS})

add_executable(testseekanddownload
  main.c
)

target_link_libraries(testseekanddownload ${CURL_LIBRARIES})

