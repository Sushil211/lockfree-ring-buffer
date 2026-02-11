#pragma once

#include <cstddef>
#include <cstdint>

namespace lfrb {
// The L1 cache line size for most modern architectures (x86_64, Apple Silicon).
// We align our atomic cursors to this boundary to prevent "False Sharing" 
// (where writes to one variable invalidate the cache line of an adjacent variable).

#ifdef __cpp_lib_hardware_interference_size
    // Use the standard library's value if available (C++17+)
    constexpr size_t kCacheLineSize = __builtin_hardware_destructive_interference_size;
#else
    constexpr size_t kCacheLineSize = 64;
#endif

// Helper to calculate the next power of 2.
// This allows us to use bitwise AND (&) masks instead of the expensive modulo (%) operator
// for wrapping the ring buffer indices.
constexpr size_t next_power_of_two(size_t x) {
    if(x == 0) {
        return 1;
    }
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return ++x;
}
} // namespace lfrb