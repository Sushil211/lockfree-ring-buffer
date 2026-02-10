cmake_minimum_required(VERSION 3.15)
project(lockfree_ring_buffer VERSION 1.0.0 LANGUAGES CXX)

# --- 1. Project Options ---
option(LFRB_BUILD_TESTS "Build the test suite" ON)
option(LFRB_BUILD_BENCHMARKS "Build benchmarks" ON)
option(LFRB_BUILD_EXAMPLES "Build example programs" ON)

# --- 2. Compiler Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Apply flags only if this is the top-level project to avoid polluting consumers
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(MSVC)
        add_compile_options(/O2 /arch:AVX2 /W4 /WX)
    else()
        # -O3: Max optimization
        # -march=native: Use instructions specific to this CPU (AVX2/AVX-512)
        # -Werror: Treat warnings as errors (strict discipline)
        add_compile_options(-O3 -march=native -Wall -Wextra -Werror)
    endif()
endif()

# --- 3. The Library (Header Only) ---
add_library(lfrb INTERFACE)
add_library(lfrb::lfrb ALIAS lfrb)

target_include_directories(lfrb INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# --- 4. Dependencies (FetchContent) ---
include(FetchContent)

# --- 5. Subdirectories ---
if(LFRB_BUILD_TESTS)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # Prevent GTest from overriding parent compiler settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    enable_testing()
    add_subdirectory(tests)
endif()

if(LFRB_BUILD_BENCHMARKS)
    FetchContent_Declare(
        benchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(benchmark)
    
    add_subdirectory(benchmarks)
endif()

if(LFRB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()