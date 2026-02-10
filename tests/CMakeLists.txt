# Link against GTest and our library
add_executable(test_correctness test_correctness.cpp)
target_link_libraries(test_correctness PRIVATE lfrb GTest::gtest_main)

add_executable(test_concurrency test_concurrency.cpp)
target_link_libraries(test_concurrency PRIVATE lfrb GTest::gtest_main)

# Register tests so 'ctest' works
include(GoogleTest)
gtest_discover_tests(test_correctness)
gtest_discover_tests(test_concurrency)