cmake_minimum_required (VERSION 3.0)

project (AdPatres)

find_package (Boost REQUIRED system)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
  set (Boost_USE_STATIC_LIBS OFF)
  set (Boost_USE_MULTITHREADED ON)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
endif (Boost_FOUND)

include_directories (blockchain)
include_directories (messages)
include_directories (/usr/local/include/botan-2)
link_directories(/usr/local/lib/)

add_library (server server.cpp server.hpp)
add_library (fpid fpid.cpp fpid.hpp)
add_library (connection connection.cpp connection.hpp)
add_library (miner miner.cpp miner.hpp)

add_subdirectory (messages)
add_subdirectory (blockchain)

add_executable (adpatresd main.cpp)
target_link_libraries (adpatresd ${EXTRA_LIBS} pthread server fpid connection version getblocks inv wallet block_msg tx miner botan-2)
