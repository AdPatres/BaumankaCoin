cmake_minimum_required (VERSION 3.0)

project (AdPatres)

set (MESSAGE_HEADERS ${MESSAGE_HEADERS} message.hpp)
set (MESSAGE_HEADERS ${MESSAGE_HEADERS} version.hpp)

add_library(messages version.cpp version.hpp)
