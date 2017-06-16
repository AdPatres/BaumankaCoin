cmake_minimum_required (VERSION 3.0)

project (AdPatres)

include_directories (/usr/local/include/botan-2)

add_library (version version.cpp version.hpp)
add_library (net_addr net_addr.cpp net_addr.hpp)
add_library (message message.cpp message.hpp)
add_library (getblocks getblocks.cpp getblocks.hpp)
add_library (inv inv.cpp inv.hpp)
add_library (block_msg block.cpp block.hpp)

target_link_libraries (version net_addr message)
target_link_libraries (getblocks message)
target_link_libraries (inv message)
target_link_libraries (block_msg message)
