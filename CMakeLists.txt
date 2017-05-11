cmake_minimum_required (VERSION 3.7)

project (AdPatres)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")

find_package (OpenSSL REQUIRED)
if (OPENSSL_FOUND)
  include_directories (${OPENSSL_INCLUDE_DIR})
  set (EXTRA_LIBS ${EXTRA_LIBS} ${OPENSSL_LIBRARIES})
endif (OPENSSL_FOUND)

find_package (Boost REQUIRED system)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
  set (Boost_USE_STATIC_LIBS OFF)
  set (Boost_USE_MULTITHREADED ON)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
endif (Boost_FOUND)

include_directories(network)

add_executable(ad_patres main.cpp)
add_subdirectory(network)
target_link_libraries (ad_patres ${EXTRA_LIBS} network)
