cmake_minimum_required (VERSION 3.0)

project (AdPatres)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall")

find_package (Boost REQUIRED system)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
  set (Boost_USE_STATIC_LIBS OFF)
  set (Boost_USE_MULTITHREADED ON)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
endif (Boost_FOUND)

add_executable(ad_patres main.cpp)
target_link_libraries (ad_patres ${EXTRA_LIBS})
