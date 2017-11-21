
include_directories(./)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")

add_library(_fcl ${SOURCES} ${HEADERS})
