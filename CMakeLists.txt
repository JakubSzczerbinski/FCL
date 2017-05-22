
cmake_minimum_required (VERSION 2.8.11)
project (FCL)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++14)

file(GLOB SOURCES "src/*.cpp")

add_executable(fcl ${SOURCES})