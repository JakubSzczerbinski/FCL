
find_package(PkgConfig)
pkg_search_module(BENCHMARK REQUIRED benchmark)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

add_executable(benchmarks ${SOURCES} ${HEADERS})
target_link_libraries(benchmarks _fcl ${BENCHMARK_LIBRARIES})
add_custom_target(benchmarks-run COMMAND "./benchmarks"
                  DEPENDS benchmarks)

