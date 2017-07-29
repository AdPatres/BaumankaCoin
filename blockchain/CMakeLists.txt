cmake_minimum_required (VERSION 3.0)

project (AdPatres)

include_directories (/usr/local/include/botan-2)
link_directories(/usr/local/lib/)

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

