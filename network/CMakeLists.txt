cmake_minimum_required (VERSION 3.0)

project (AdPatres)

find_package (Boost REQUIRED system)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
  set (Boost_USE_STATIC_LIBS OFF)
  set (Boost_USE_MULTITHREADED ON)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
endif (Boost_FOUND)

include_directories (messages)
link_directories(/usr/local/lib/)

add_subdirectory (messages)

add_library (server server.cpp server.hpp)
add_library (connection connection.cpp connection.hpp)
target_link_libraries (server pthread ${EXTRA_LIBS} connection message version getblocks inv block_msg tx)
