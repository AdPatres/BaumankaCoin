cmake_minimum_required (VERSION 3.7)

project (AEgida)
add_executable(main.exe main.cpp)

find_package (OpenSSL REQUIRED)
if (OPENSSL_FOUND)
  include_directories (${OPENSSL_INCLUDE_DIR})
  set (EXTRA_LIBS ${EXTRA_LIBS} ${OPENSSL_LIBRARIES})
endif (OPENSSL_FOUND)

find_package (Boost REQUIRED)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
  set (Boost_USE_STATIC_LIBS OFF)
  set (Boost_USE_MULTITHREADED ON)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
endif (Boost_FOUND)

target_link_libraries (main.exe ${EXTRA_LIBS})
