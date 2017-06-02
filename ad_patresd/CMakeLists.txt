cmake_minimum_required (VERSION 3.0)

project (ad_patresd)

find_package (Boost REQUIRED system)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
  set (Boost_USE_STATIC_LIBS OFF)
  set (Boost_USE_MULTITHREADED ON)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
endif (Boost_FOUND)

set(HEADERS ad_patresd.hpp)
set(SOURCES main.cpp ad_patresd.cpp)

add_executable(ad_patresd ${SOURCES} ${HEADERS})
