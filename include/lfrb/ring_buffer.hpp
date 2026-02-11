#pragma once

#include <atomic>
#include <vector>
#include <optional>
#include <span>
#include <bit>
#include <cstddef>
#include <stdexcept>

#include "defs.hpp"

namespace lfrb {
/**
 * @brief A Single-Producer, Single-Consumer (SPSC) lock-free ring buffer.
 *
 * This implementation uses C++20 atomics with acquire-release semantics to ensure
 * thread safety without locks. It is designed for high-throughput, low-latency
 * communication between two threads.
 *
 * @tparam T The type of element to be stored in the buffer.
 */

template <typename T>
class RingBuffer {
public:
    /**
     * @brief Constructs the ring buffer with a given minimum capacity.
     * @param capacity The desired minimum capacity. The actual capacity will be
     *                 rounded up to the next power of two. Must be at least 1.
     */

    explicit RingBuffer(size_t capacity)
        : capacity_(next_power_of_two(capacity)),
        mask_(capacity_ - 1),
        buffer_(capacity_) {
        // The actual buffer size is capacity_, but one slot is always
        // kept empty to distinguish between full and empty states.
        // Therefore, the usable capacity is capacity_ - 1.
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be at least 1");
        }
    }

    ~RingBuffer() = default;

    // This class manages thread-sensitive atomic state, so it's non-copyable and non-movable.
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;
    RingBuffer(RingBuffer&&) = delete;
    RingBuffer& operator=(RingBuffer&&) = delete;

    // Pushes an item into the buffer by copying it.
    bool push(const T& item) {}

    // Pushes an item into the buffer by moving it.
    bool push(T&& item) {}

    // Pops an item from the buffer.
    bool pop(T& item) {}

    /**
     * @brief Returns the number of items that can be stored in the buffer.
     * @return The usable capacity of the buffer.
     */
    size_t capacity() const noexcept {
        return capacity_;
    }

    // An estimate of the current number of items in the buffer.
    size_t size_approx() const {}

private:
    // To prevent "false sharing", data modified by the producer (head_) and data
    // modified by the consumer (tail_) must not reside on the same cache line.
    alignas(lfrb::kCacheLineSize) std::atomic<size_t> head_{0};
    alignas(lfrb::kCacheLineSize) std::atomic<size_t> tail_{0};

    // These are read-only after construction and can be shared without issue.
    const size_t capacity_; // Power-of-2 size of the underlying buffer
    const size_t mask_; // (capacity_ - 1) for fast modulo

    // Padded to ensure the std::vector control block doesn't share a cache
    // line with the tail_ atomic. The actual buffer data is on the heap.
    alignas(lfrb::kCacheLineSize) std::vector<T> buffer_;
};
}
