cmake_minimum_required (VERSION 2.8.0)
project (FCL)
cmake_minimum_required(VERSION 3.0.0)

execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR} --build missing -s compiler.libcxx=libstdc++11)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

cmake_policy(SET CMP0015 NEW)

find_package (Boost 1.58 REQUIRED)

include_directories(
       ./
       ${CONAN_INCLUDE_DIRS}
       ${Boost_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -ggdb -O0 -pthread")

add_subdirectory(FCLcli)
add_subdirectory(FCLws)
add_subdirectory(libFCL)
add_subdirectory(test)
