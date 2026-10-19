#pragma once
#include <cstddef>
#include <stdexcept>
#include <cmath>
#include <type_traits>

template <typename T>
class Set {
private:
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
    static constexpr double epsilon = 0.000001;
    static bool is_equal(const T& lhs, const T& rhs) {
        if constexpr (std::is_floating_point_v<T>) {
            return std::abs(lhs - rhs) < epsilon;
        }
        else {
            return lhs == rhs;
        }
    }
    void grow() {
        std::size_t new_capacity = 0;
        if (capacity_ == 0) {
            new_capacity = 1;
        }
        else {
            new_capacity = capacity_ * 2;
        }
        T* new_data = new T[new_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
public:
    Set() = default;
    Set(const Set& other) {
        data_ = new T[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
        capacity_ = other.size_;
    }
    Set& operator=(const Set& other) {
        if (this == &other) {
            return *this;
        }
        T* new_data = new T[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            new_data[i] = other.data_[i];
        }
        delete[] data_;
        data_ = new_data;
        size_ = other.size_;
        capacity_ = other.size_;
        return *this;
    }
    ~Set() {
        delete[] data_;
    }
    std::size_t size() const {
        return size_;
    }
    bool empty() const {
        return size() == 0;
    }
    const T& operator[](std::size_t idx) const {
        if (idx < size()) {
            return data_[idx];
        }
        else {
            throw std::out_of_range("Index is out of range");
        }
    }
    bool contains(const T& value) const {
        for (std::size_t i = 0; i < size(); ++i) {
            if (is_equal(data_[i], value)) {
                return true;
            }
        }
        return false;
    }
    Set& operator+=(const T& value) {
        if (contains(value)) {
            return *this;
        }
        if (size_ == capacity_) {
            grow();
        }
        data_[size_] = value;
        ++size_;
        return *this;
    }
    Set operator+(const T& value) const {
        Set<T> set_copied(*this);
        set_copied += value;
        return set_copied;
    }
};
