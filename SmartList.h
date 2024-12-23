#pragma once

#include "shared_ptr.h"
#include "weak_ptr.h"
#include "unique_ptr.h"

#include <stdexcept>

template<typename T>
class Node {
public:
    T data;
    shared_ptr<Node<T>> right;

    explicit Node(const T& value) : data(value), right(nullptr) {}
};

template<typename T>
class SmartList {
private:
    shared_ptr<Node<T>> first;
    shared_ptr<Node<T>> last;
    int size;

    void destroyList() {
        first.reset();
        last.reset();
        size = 0;
    }

    void checkIndex(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("IndexOutOfRange");
        }
    }

public:
    SmartList<T>() : first(nullptr), last(nullptr), size(0) {}

    SmartList(T* items, int size) : first(nullptr), last(nullptr), size(0) {
        for (int i = 0; i < size; ++i) {
            append(items[i]);
        }
    }

    SmartList(const SmartList<T>& list) : first(nullptr), last(nullptr), size(0) {
        *this = list;
    }

    SmartList(SmartList&& other) noexcept : first{ std::move(other.first) }, last{ std::move(other.last) }, size(other.size) {
        other.size = 0;
    }

    const T& getFirst() const {
        if (!first) {
            throw std::out_of_range("ListIsEmpty");
        }
        return first->data;
    }

    const T& getLast() const {
        if (!last) {
            throw std::out_of_range("ListIsEmpty");
        }
        return last->data;
    }

    const T& getByIndex(int index) const {
        checkIndex(index);
        shared_ptr<Node<T>> temp = first;
        for (int i = 0; i < index; ++i) {
            temp = temp->right;
        }
        return temp->data;
    }

    unique_ptr<SmartList<T>> getSubList(int startIndex, int endIndex) const {
        checkIndex(startIndex);
        checkIndex(endIndex);
        unique_ptr<SmartList<T>> sublist(new SmartList<T>());
        shared_ptr<Node<T>> temp = first;
        for (int i = 0; i < startIndex; ++i) {
            temp = temp->right;
        }
        for (int i = startIndex; i <= endIndex; ++i) {
            sublist->append(temp->data);
            temp = temp->right;
        }
        return sublist;
    }

    [[nodiscard]] int getLength() const {
        return size;
    }

    void append(const T& value) {
        shared_ptr<Node<T>> newNode = shared_ptr<Node<T>>(new Node<T>(value));
        if (!first) {
            first = newNode;
            last = newNode;
        }
        else {
            last->right = newNode;
            last = newNode;
        }
        size++;
    }

    void prepend(const T& value) {
        shared_ptr<Node<T>> newNode = shared_ptr<Node<T>>(new Node<T>(value));
        if (!first) {
            first = last = newNode;
        }
        else {
            newNode->right = first;
            first = newNode;
        }
        size++;
    }

    void insertAt(int index, const T& value) {
        if (index == 0) {
            prepend(value);
            return;
        }
        checkIndex(index);
        shared_ptr<Node<T>> temp = first;
        for (int i = 0; i < index - 1; ++i) {
            temp = temp->right;
        }
        shared_ptr<Node<T>> newNode = shared_ptr<Node<T>>(new Node<T>(value));
        newNode->right = temp->right;
        temp->right = newNode;
        size++;
    }

    unique_ptr<SmartList<T>> concatenate(const SmartList<T>& other) const {
        unique_ptr<SmartList<T>> newList(new SmartList<T>());
        shared_ptr<Node<T>> temp = first;
        while (temp) {
            newList->append(temp->data);
            temp = temp->right;
        }
        temp = other.first;
        while (temp) {
            newList->append(temp->data);
            temp = temp->right;
        }
        return newList;
    }

    SmartList<T>& operator=(const SmartList<T>& list) {
        if (this != &list) {
            destroyList();
            shared_ptr<Node<T>> temp = list.first;
            while (temp) {
                append(temp->data);
                temp = temp->right;
            }
        }
        return *this;
    }
};
