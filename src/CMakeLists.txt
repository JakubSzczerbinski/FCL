
include_directories(./)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(_fcl ${SOURCES} ${HEADERS})

add_executable(fcl main.cpp)
target_link_libraries(fcl _fcl)