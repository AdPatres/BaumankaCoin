cmake_minimum_required (VERSION 3.0)

project (ad_patresd)

find_package (Boost REQUIRED system serialization)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
  set (Boost_USE_STATIC_LIBS OFF)
  set (Boost_USE_MULTITHREADED ON)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
endif (Boost_FOUND)

set(HEADERS ad_patresd.hpp peer.hpp server.hpp message.hpp)
set(SOURCES main.cpp ad_patresd.cpp peer.cpp server.cpp message.cpp)

add_executable(ad_patresd ${SOURCES} ${HEADERS})
target_link_libraries (ad_patresd ${EXTRA_LIBS})
