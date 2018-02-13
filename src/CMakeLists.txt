
include_directories(./)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")

add_library(fcl ${SOURCES} ${HEADERS})
# set_target_properties(fcl PROPERTIES COMPILE_FLAGS "-fPIC")
