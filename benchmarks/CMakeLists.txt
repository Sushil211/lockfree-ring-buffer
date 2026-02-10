# Link against Google Benchmark and our library
add_executable(bench_latency bench_latency.cpp)
target_link_libraries(bench_latency PRIVATE lfrb benchmark::benchmark)