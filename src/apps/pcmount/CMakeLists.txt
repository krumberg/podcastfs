cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(pcmount)

find_package(PkgConfig)
pkg_check_modules(FUSE fuse)

add_executable(pcmount
  main.c
  fuse.c
)

include_directories( ${FUSE_INCLUDE_DIRS} )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64 -fnested-functions")
target_link_libraries(pcmount podcast ${FUSE_LIBRARIES} ${GLIB_LIBRARIES} ${XML_LIBRARIES} ${CURL_LIBRARIES})

