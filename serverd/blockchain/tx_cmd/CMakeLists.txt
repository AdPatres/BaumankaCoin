cmake_minimum_required (VERSION 3.0)

project (AdPatres)

include_directories (/usr/local/include/botan-2)

add_library (command command.cpp command.h)
add_library (cmd_input addInput.cpp removeInput.cpp addInput.h removeInput.h)
add_library (cmd_tail addTail.cpp addTail.h removeTail.cpp removeTail.h)
add_library (cleaner cleaner.cpp cleaner.h)
add_library (receiver receiver.cpp receiver.h)
add_library (signer signer.cpp signer.h)

target_link_libraries (cmd_input command)
target_link_libraries (cmd_tail command)
target_link_libraries (cleaner command)
target_link_libraries (signer command)
target_link_libraries (receiver cmd_input cmd_tail cleaner signer)
