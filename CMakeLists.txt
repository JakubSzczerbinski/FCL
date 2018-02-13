
cmake_minimum_required (VERSION 2.8.0)
project (FCL)

execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR} --build gtest OUTPUT_QUIET)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

cmake_policy(SET CMP0015 NEW)

find_package (Boost 1.58 REQUIRED)

include_directories(
	src
	${CONAN_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)
# link_directories(lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -ggdb -O0 -pthread")

add_subdirectory(src)
add_subdirectory(test)
