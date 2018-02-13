
include_directories(./)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

add_executable(fcl_tests EvaluatorTests.cpp)#${SOURCES} ${HEADERS})
target_link_libraries(fcl_tests fcl gtest gtest_main)

add_custom_target(run_tests COMMAND "./fcl_tests"
                  DEPENDS fcl_tests)
