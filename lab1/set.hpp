#pragma once
#include <cstddef>
#include <stdexcept>
#include <cmath>
#include <type_traits>
#include <random>
#include <ostream>
#include <utility>
#include <string>

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
    std::size_t find_index(const T& value) const {
        for (std::size_t idx = 0; idx < size(); ++idx) {
            if (is_equal(data_[idx], value)) {
                return idx;
            }
        }
        return size_;
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
        for (std::size_t i = 0; i < size(); ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
public:
    Set() = default;
    Set(const T* values, std::size_t count) {
        if (values == nullptr && count > 0) {
            throw std::invalid_argument("Values pointer is null");
        }
        for (std::size_t i = 0; i < count; ++i) {
            *this += values[i];
        }
    }
    Set(std::size_t count, const T& low, const T& high) {
        if (low > high) {
            throw std::invalid_argument("Low value more than high value");
        }
        if constexpr (std::is_integral_v<T>) {
            std::size_t distinct_values = static_cast<std::size_t>(high) - static_cast<std::size_t>(low) + 1;
            if (count > distinct_values) {
                throw std::invalid_argument("Value of count more than distance");
            }
        }
        using distribution_type = std::conditional_t<std::is_floating_point_v<T>, std::uniform_real_distribution<T>, std::uniform_int_distribution<T>>;
        std::random_device device;
        std::mt19937 engine(device());
        distribution_type distribution(low, high);
        while (size() < count) {
            *this += distribution(engine);
        }
    }
    Set(const Set& other) {
        data_ = new T[other.size_];
        for (std::size_t i = 0; i < other.size(); ++i) {
            data_[i] = other.data_[i];
        }
        size_ = other.size();
        capacity_ = other.size();
    }
    Set& operator=(const Set& other) {
        if (this == &other) {
            return *this;
        }
        T* new_data = new T[other.size()];
        for (std::size_t i = 0; i < other.size(); ++i) {
            new_data[i] = other.data_[i];
        }
        delete[] data_;
        data_ = new_data;
        size_ = other.size();
        capacity_ = other.size();
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
        if (find_index(value) != size()) {
            return true;
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
    Set operator+(const Set& other) const {
        Set<T> set_copied(*this);
        for (std::size_t i = 0; i < other.size(); ++i) {
            set_copied += other[i];
        }
        return set_copied;
    }
    Set& operator-=(const T& value) {
        std::size_t idx = find_index(value);
        if (idx == size()) {
            return *this;
        }

        for (; idx < size() - 1; ++idx) {
            data_[idx] = data_[idx + 1];
        }
        --size_;
        return *this;
    }
    Set operator-(const T& value) const {
        Set<T> set_copied(*this);
        set_copied -= value;
        return set_copied;
    }
    Set operator-(const Set& other) const {
        Set<T> set_copied(*this);
        for (std::size_t i = 0; i < other.size(); ++i) {
            set_copied -= other[i];
        }
        return set_copied;
    }
    Set intersection(const Set& other) const {
        Set<T> result;
        for (std::size_t i = 0; i < size(); ++i) {
            if (other.contains((*this)[i])) {
                result += (*this)[i];
            }
        }
        return result;
    }
    bool operator==(const Set& other) const {
        if (size() != other.size()) {
            return false;
        }
        for (std::size_t i = 0; i < size(); ++i) {
            if (!other.contains((*this)[i])) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Set& other) const {
        return !(*this == other);
    }
};

template <typename First, typename Second>
std::ostream& operator<<(std::ostream& os, const std::pair<First, Second>& value) {
    os << "(" << value.first << ", " << value.second << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Set<T>& set) {
    os << "{";
    for (std::size_t i = 0; i < set.size(); ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << set[i];
    }
    os << "}";
    return os;
}

template <typename T>
bool is_subset(const Set<T>& a, const Set<T>& b) {
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (!b.contains(a[i])) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool is_superset(const Set<T>& a, const Set<T>& b) {
    return is_subset(b, a);
}

enum class SetRelation {
    equal,
    subset,
    superset,
    no_nesting
};

template <typename T>
SetRelation get_relation(const Set<T>& a, const Set<T>& b) {
    if (a == b) {
        return SetRelation::equal;
    }
    if (is_subset(a, b)) {
        return SetRelation::subset;
    }
    if (is_superset(a, b)) {
        return SetRelation::superset;
    }
    return SetRelation::no_nesting;
}

inline std::string relation_to_string(SetRelation relation) {
    switch (relation) {
        case SetRelation::equal:
            return "equal";
        case SetRelation::subset:
            return "subset";
        case SetRelation::superset:
            return "superset";
        case SetRelation::no_nesting:
            return "no nesting";
    }
    return "unknown";
}
