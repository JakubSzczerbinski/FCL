
cmake_minimum_required (VERSION 2.8.11)
project (FCL)

set(CXX_FLAGS ${CXX_FLAGS} -std=c++14 -Wall -Wextra)

file(GLOB SOURCES "src/*.cpp")

add_executable(fcl ${SOURCES})