#include "set.hpp"
#include <iostream>
#include <utility>
#include <string>
#include <stdexcept>

bool check_condition(bool condition, const std::string& description) {
    if (condition) {
        std::cout << "[OK] " << description << "\n";
    }
    else {
        std::cout << "[FAIL] " << description << "\n";
    }
    return condition;
}

template <typename T>
int test_empty_set(const T& value, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, empty set ---\n";
    Set<T> s;
    int fail_count = 0;
    if (!check_condition(s.size() == 0, "size() == 0")) {
        fail_count++;
    }
    if (!check_condition(s.empty(), "empty()")) {
        fail_count++;
    }
    if (!check_condition(!s.contains(value), "contains() returns false")) {
        fail_count++;
    }
    bool exception_flag = false;
    try {
        s[0];
    } catch (const std::out_of_range&) {
        exception_flag = true;
    }
    if (!check_condition(exception_flag, "operator[] throws out_of_range")) {
        fail_count++;
    }
    return fail_count;
}

int main() {
    int result_count = 0;
    result_count += test_empty_set<int>(42, "int");
    result_count += test_empty_set<float>(2.5f, "float");
    result_count += test_empty_set<std::string>("abc", "std::string");
    result_count += test_empty_set<std::pair<int, double>>({2,10.6}, "std::pair<int, double>");
    std::cout << "\nFailed: " << result_count << "\n";
    if (result_count == 0) {
        return 0;
    }
    else {
        return 1;
    }
}
