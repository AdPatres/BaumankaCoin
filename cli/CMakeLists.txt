cmake_minimum_required (VERSION 3.0)

project (AdPatres)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall")

add_executable(ad_patres main.cpp)
target_link_libraries (ad_patres)
