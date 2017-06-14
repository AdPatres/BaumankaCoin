cmake_minimum_required (VERSION 3.0)

project (AdPatres)

include_directories (/usr/local/include/botan-2)

add_library (tail tail.cpp tail.h)
add_library (output output.cpp output.h)
add_library (block block.cpp block.h)
add_library (blockchain blockchain.cpp blockchain.h)
add_library (transaction transaction.cpp transaction.h)
add_library (wallet wallet.cpp wallet.h)
add_library (input Input.cpp Input.h)

target_link_libraries (transaction output input tail)
target_link_libraries (block transaction)
target_link_libraries (blockchain block)
target_link_libraries (wallet blockchain)
