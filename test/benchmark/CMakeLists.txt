
find_package(PkgConfig)
pkg_search_module(BENCHMARK benchmark)

if(${BENCHMARK_FOUND})
	file(GLOB SOURCES "*.cpp")
	file(GLOB HEADERS "*.hpp")

	add_executable(benchmarks ${SOURCES} ${HEADERS})
	target_link_libraries(benchmarks fcl ${BENCHMARK_LIBRARIES})
	add_custom_target(benchmarks-run COMMAND "./benchmarks"
	                  DEPENDS benchmarks)
endif(${BENCHMARK_FOUND})


