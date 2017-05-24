
cmake_minimum_required (VERSION 2.8.11)
project (FCL)

include_directories(src)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -ggdb -O0")

add_subdirectory(src)
add_subdirectory(test)

# enable_testing()
# add_test(NAME fclTests COMMAND fcl_tests)