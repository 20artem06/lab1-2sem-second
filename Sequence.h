#pragma once

#include "unique_ptr.h"
#include "shared_ptr.h"

template<class T>
class Sequence {
public:
    virtual const T& get(int index) const = 0;

    virtual const T& getFirst() const = 0;

    virtual const T& getLast() const = 0;

    virtual unique_ptr<Sequence<T>> getSubSequence(int startIndex, int endIndex) = 0;

    [[nodiscard]] virtual int getLength() const = 0;

    virtual void append(const T& item) = 0;

    virtual void prepend(const T& item) = 0;

    virtual void insertAt(int index, const T& item) = 0;

    virtual void concat(unique_ptr<Sequence<T>> sequence) = 0;
};
