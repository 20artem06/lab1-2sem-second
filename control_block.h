#pragma once
#include <atomic>

template <typename T>
class ControlBlock {
public:
    std::atomic<size_t> shared_count;
    std::atomic<size_t> weak_count;
    T* ptr;

    ControlBlock(T* p) : shared_count(1), weak_count(0), ptr(p) {}


};
