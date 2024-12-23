#pragma once

#include <cstddef>
#include <atomic>
#include "shared_ptr.h"
#include "control_block.h"


template <typename T>
class shared_ptr;



template <typename T>
class weak_ptr {
private:
    ControlBlock<T>* control;

    void release() {
        if (control) {
            if (control->weak_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                if (control->shared_count.load(std::memory_order_acquire) == 0) {
                    delete control;
                }
            }
        }
    }

public:
    constexpr weak_ptr() noexcept : control(nullptr) {}

    weak_ptr(const shared_ptr<T>& sp) noexcept : control(sp.control) {
        if (control) {
            control->weak_count.fetch_add(1, std::memory_order_relaxed);
        }
    }

    weak_ptr(const weak_ptr& other) noexcept : control(other.control) {
        if (control) {
            control->weak_count.fetch_add(1, std::memory_order_relaxed);
        }
    }

    weak_ptr(weak_ptr&& other) noexcept : control(other.control) {
        other.control = nullptr;
    }



    weak_ptr& operator=(const weak_ptr& other) noexcept {
        if (this != &other) {
            release();
            control = other.control;
            if (control) {
                control->weak_count.fetch_add(1, std::memory_order_relaxed);
            }
        }
        return *this;
    }
    weak_ptr& operator=(weak_ptr&& other) noexcept {
        if (this != &other) {
            release();
            control = other.control;
            other.control = nullptr;
        }
        return *this;
    }
    ~weak_ptr() {
        release();
    }
    bool expired() const noexcept {
        return !control || control->shared_count.load(std::memory_order_acquire) == 0;
    }
    shared_ptr<T> lock() const noexcept {
        if (expired()) {
            return shared_ptr<T>();
        }
        return shared_ptr<T>(*this);
    }
    size_t use_count() const noexcept {
        return control ? control->shared_count.load(std::memory_order_relaxed) : 0;
    }

    explicit operator bool() const noexcept {
        return use_count() > 0;
    }

    friend class shared_ptr<T>;
};
