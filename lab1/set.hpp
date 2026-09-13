#pragma once
#include <cstddef>
#include <stdexcept>

template <typename T>
class Set {
private:
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
public:
    Set() = default;
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
            if (data_[i] == value) {
                return true;
            }
        }
        return false;
    }
};
