cmake_minimum_required (VERSION 3.0)

project (AdPatres)

find_package(BOTAN REQUIRED)
include_directories(${BOTAN_INCLUDE_DIRS})
link_directories(${BOTAN_LIBRARIES})

add_library (output output.cpp output.h)
add_library (tail tail.cpp tail.h)
add_library (input Input.cpp Input.h)
add_library (transaction transaction.cpp transaction.h)
add_library (block block.cpp block.h)
add_library (blockchain blockchain.cpp blockchain.h)


add_subdirectory (tx_cmd)

target_link_libraries (transaction output input tail)
target_link_libraries (block transaction)
target_link_libraries (blockchain block)

