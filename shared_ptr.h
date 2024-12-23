#pragma once

#include <cstddef>
#include <stdexcept>
#include <atomic>
#include "weak_ptr.h"
#include "control_block.h"

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
private:
    ControlBlock<T>* control;

    void release() {
        if (control) {
            if (control->shared_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete control->ptr;
                if (control->weak_count.load(std::memory_order_acquire) == 0) {
                    delete control;
                }
            }
            control = nullptr;
        }
    }

public:

    constexpr shared_ptr() noexcept : control(nullptr) {}

    explicit shared_ptr(T* p) : control(nullptr) {
        if (p) {
            control = new ControlBlock<T>(p);
        }
    }

    shared_ptr(const shared_ptr& other) noexcept : control(other.control) {
        if (control) {
            control->shared_count.fetch_add(1, std::memory_order_relaxed);
        }
    }

    shared_ptr(shared_ptr&& other) noexcept : control(other.control) {
        other.control = nullptr;
    }

    explicit shared_ptr(const weak_ptr<T>& wp) noexcept;

    ~shared_ptr() {
        release();
    }
    shared_ptr& operator=(const shared_ptr& other) noexcept {
        if (this != &other) {
            release();
            control = other.control;
            if (control) {
                control->shared_count.fetch_add(1, std::memory_order_relaxed);
            }
        }
        return *this;
    }
    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if (this != &other) {
            release();
            control = other.control;
            other.control = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        if (control->ptr != nullptr) {
            return *(control->ptr);
        }
        throw std::runtime_error("Dereferencing null unique_ptr");
    }
    T* operator->() const {
        if (control->ptr != nullptr) {
            return control ? control->ptr : nullptr;
        }
        throw std::runtime_error("Dereferencing null unique_ptr");
    }

    T* get() const noexcept { return control ? control->ptr : nullptr; }

    size_t use_count() const noexcept {
        return control ? control->shared_count.load(std::memory_order_relaxed) : 0;
    }

    bool unique() const noexcept {
        return use_count() == 1;
    }

    explicit operator bool() const noexcept {
        return get() != nullptr;
    }

    void reset(T* ptr = nullptr) noexcept {
        release();
        if (ptr) {
            control = new ControlBlock<T>(ptr);
        }
        else {
            control = nullptr;
        }
    }
    friend class weak_ptr<T>;
};


template <typename T>
shared_ptr<T>::shared_ptr(const weak_ptr<T>& wp) noexcept : control(wp.control) {
    if (control) {
        size_t current_shared = control->shared_count.load(std::memory_order_acquire);
        while (current_shared != 0) {

            if (control->shared_count.compare_exchange_weak(
                    current_shared, current_shared + 1, std::memory_order_acq_rel, std::memory_order_acquire)) {
                break;
            }
        }
        if (current_shared == 0) {
            control = nullptr;
        }
    }
}
