//
// Created by bordplate on 12/26/2022.
//

#ifndef RAC1_MULTIPLAYER_VECTOR_H
#define RAC1_MULTIPLAYER_VECTOR_H

#include "clib.h"

template<typename T>
class Vector {
public:
    // Constructors
    Vector() : data_(0), size_(0), capacity_(0) {}
    Vector(int size) : data_(0), size_(0), capacity_(0) { reserve(size); }

    // Destructor
    ~Vector() { delete[] data_; }

    // Element access
    T &operator[](int index) { return data_[index]; }
    const T &operator[](int index) const { return data_[index]; }

    // Copy assignment
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;

            data_ = 0;
            size_ = 0;
            capacity_ = 0;

            reserve(other.capacity_);
            for (int i = 0; i < other.size_; i++) {
                data_[i] = other.data_[i];
            }
            size_ = other.size_;
        }

        return *this;
    }

    // Iterators
    T *begin() { return data_; }
    const T *begin() const { return data_; }
    T *end() { return data_ + size_; }
    const T *end() const { return data_ + size_; }

    // Capacity
    int size() const { return size_; }
    int capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    void resize(int size) {
        if (size > capacity_) {
            reserve(size);
        }

        size_ = size;
    }

    void reserve(int capacity)  {
        if (capacity > capacity_) {
            T* new_data = new T[capacity];

            for (int i = 0; i < size_; i++) {
                new_data[i] = data_[i];
            }

            delete[] data_;
            data_ = new_data;
            capacity_ = capacity;
        }
    }

    // Modifiers
    void push_back(const T &value) {
        if (size_ == capacity_) {
            reserve(max(1, capacity_ * 2));
        }

        data_[size_++] = value;
    }

    void pop_back() {
        if (size_ > 0) {
            size_--;
        }
    }

private:
    T *data_;
    int size_;
    int capacity_;
};

#endif //RAC1_MULTIPLAYER_VECTOR_H
