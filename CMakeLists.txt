
cmake_minimum_required (VERSION 2.8.0)
project (FCL)

include_directories(src)
include_directories(include)

link_directories(lib)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -ggdb -O0 -pthread")

add_subdirectory(src)
add_subdirectory(test)