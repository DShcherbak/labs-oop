project(lab_benchmark)

add_executable(lab_benchmark benchmark.cpp BenchmarkUtils.hpp)

target_link_libraries(lab_benchmark PRIVATE benchmark::benchmark ${LIB_NAME})

add_subdirectory(googletest)
add_subdirectory(benchmark)
