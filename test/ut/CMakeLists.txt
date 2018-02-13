
include_directories(./)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

add_executable(fcl_tests ${SOURCES} ${HEADERS})
target_link_libraries(fcl_tests fcl gtest)

add_custom_target(run_tests COMMAND "${CMAKE_BINARY_DIR}/bin/fcl_tests"
                  DEPENDS fcl_tests)
