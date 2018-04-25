
include_directories(./)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")

add_executable(fcl_ws ${SOURCES} ${HEADERS})
link_directories(${CONAN_LIB_DIRS_JSONCPP})
target_link_libraries(fcl_ws boost_system fcl jsoncpp)
