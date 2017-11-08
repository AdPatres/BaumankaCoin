cmake_minimum_required (VERSION 3.0)

project (AdPatres)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/" ${CMAKE_MODULE_PATH})
find_package(BOTAN REQUIRED)
include_directories(${BOTAN_INCLUDE_DIRS})
link_directories(${BOTAN_LIBRARIES})

include_directories (blockchain network)

add_subdirectory (blockchain)
add_subdirectory (network)

add_library (wallet wallet.cpp wallet.h)
add_library (miner miner.cpp miner.hpp)

add_executable (adpatres main.cpp)
target_link_libraries (wallet botan-2 blockchain receiver)
target_link_libraries (adpatres wallet server miner)
