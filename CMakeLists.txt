
cmake_minimum_required (VERSION 2.8.0)
project (FCL)

cmake_policy(SET CMP0015 NEW)

find_package (Boost 1.55 REQUIRED)

include_directories(
	src
	include
	${Boost_INCLUDE_DIRS}
)

MESSAGE( STATUS "BOOST_INCLUDE_DIRS:         " ${Boost_INCLUDE_DIRS} )

link_directories(lib)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -ggdb -O0 -pthread")

add_subdirectory(src)
add_subdirectory(test)