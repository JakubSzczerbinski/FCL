
cmake_minimum_required (VERSION 2.8.11)
project (FCL)

include_directories(./src)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -ggdb -O0")

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.hpp")

add_executable(fcl ${SOURCES} ${HEADERS})