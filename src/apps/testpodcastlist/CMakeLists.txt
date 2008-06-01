cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(testpodcastlist)

add_executable(testpodcastlist
  main.c
)

target_link_libraries(testpodcastlist podcast ${GLIB_LIBRARIES} ${XML_LIBRARIES} ${CURL_LIBRARIES})

