cmake_minimum_required (VERSION 3.0)

project (AdPatres)

set (MESSAGE_HEADERS ${MESSAGE_HEADERS} message.hpp)
set (MESSAGE_HEADERS ${MESSAGE_HEADERS} net_addr.hpp)
set (MESSAGE_HEADERS ${MESSAGE_HEADERS} version.hpp)

add_library(messages message.cpp version.cpp net_addr.cpp ${MESSAGE_HEADERS})
