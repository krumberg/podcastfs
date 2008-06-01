cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(testdownloadxml)

find_package(PkgConfig)
pkg_check_modules(GIO gio-2.0)

include_directories(${GIO_INCLUDE_DIRS})

add_executable(testdownloadxml
  main.c
)

target_link_libraries(testdownloadxml podcast ${GIO_LIBRARIES} ${GLIB_LIBRARIES} ${XML_LIBRARIES} ${CURL_LIBRARIES})

