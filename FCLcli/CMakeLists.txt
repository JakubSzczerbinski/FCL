
include_directories(./)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")

add_executable(fcl_cli ${SOURCES} ${HEADERS})
target_link_libraries(fcl_cli fcl)
