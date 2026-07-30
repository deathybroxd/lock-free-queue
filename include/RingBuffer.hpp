/* RingBuffer.hpp 
templated lock free queue
*/

#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <atomic>
#include <thread>
#include <array>
#include <new>
#include <optional>

// T = type, N = buffer capacity
template <typename T, size_t N>
class RingBuffer {
public:
    // constructor + rule of five not needed because std::atomic doesnt support copy/move operations

    bool Push(const T& item);

    bool Pop(T& item);


private:
    static constexpr int x86_MEMORY_BOUNDARY = 64; // 64 byte memory boundary

    size_t GetNext(size_t index) const;

    std::array<T, N + 1> m_buffer;
    alignas(x86_MEMORY_BOUNDARY) std::atomic<size_t> m_head{0};
    alignas(x86_MEMORY_BOUNDARY) std::atomic<size_t> m_tail{0};
};

template <typename T, size_t N>
bool RingBuffer<T, N>::Push(const T& item) {
    size_t currTail{m_tail.load(std::memory_order_relaxed)};
    size_t nextTail{GetNext(currTail)};
    size_t currHead{m_head.load(std::memory_order_acquire)};

    // if buffer full
    if(nextTail == currHead) {
        return false;
    }

    m_buffer.at(currTail) = item;
    m_tail.store(nextTail, std::memory_order_release);
    return true;
}

template <typename T, size_t N>
bool RingBuffer<T, N>::Pop(T& item) {
    size_t currHead{m_head.load(std::memory_order_relaxed)};
    size_t currTail{m_tail.load(std::memory_order_acquire)};
    
    // if buffer empty
    if(currHead == currTail) {
        return false;
    }

    item = m_buffer.at(currHead);
    m_head.store(GetNext(currHead), std::memory_order_release);

    return true;
}   

// private helpers
template <typename T, size_t N>
size_t RingBuffer<T, N>::GetNext(size_t index) const {
    return (index + 1) % (N + 1);
}

#endif