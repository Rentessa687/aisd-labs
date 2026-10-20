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

template<typename T>
int test_copy(const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, copy ---\n";
    int fail_count = 0;
    Set<T> a;
    Set<T> b = a;
    if (!check_condition(b.size() == 0, "copy constructor: size is 0")) {
        fail_count++;
    }
    if (!check_condition(b.empty(), "copy constructor: empty()")) {
        fail_count++;
    }

    Set<T> c(b);
    if (!check_condition(c.size() == 0, "copy constructor with parentheses")) {
        fail_count++;
    }

    Set<T> d;
    d = a;
    if (!check_condition(d.size() == 0, "assignment: size is 0")) {
        fail_count++;
    }

    Set<T>& ref = d;
    d = ref;
    if (!check_condition(d.size() == 0, "self assignment keeps object valid")) {
        fail_count++;
    }

    a = b = c;
    if (!check_condition(a.size() == 0, "chained assignment")) {
        fail_count++;
    }

    {
        Set<T> temp = a;
        Set<T> another;
        another = temp;
    }
    if (!check_condition(a.size() == 0, "original survives destruction of copies")) {
        fail_count++;
    }

    return fail_count;
}

template<typename T>
int test_add(const T& first, const T& second, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, add ---\n";
    int fail_count = 0;
    Set<T> s;
    s += first;
    if (!check_condition(s.size() == 1, "first added: size = 1")) {
        fail_count++;
    }
    if (!check_condition(s.contains(first), "s contains first")) {
        fail_count++;
    }
    s += first;
    if (!check_condition(s.size() == 1, "first added again: size also 1")) {
        fail_count++;
    }
    s += second;
    if (!check_condition(s.size() == 2, "second added: size = 2")) {
        fail_count++;
    }
    if (!check_condition(s.contains(second), "s contains second")) {
        fail_count++;
    }
    if (!check_condition(s[0] == first, "s[0] = first")) {
        fail_count++;
    }
    if (!check_condition(s[1] == second, "s[1] = second")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_add_operator(const T& first, const T& second, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, add operator ---\n";
    int fail_count = 0;
    Set<T> a;
    Set<T> b = a + first;
    if (!check_condition(b.size() == 1, "first added: size = 1")) {
        fail_count++;
    }
    if (!check_condition(b.contains(first), "b contains first")) {
        fail_count++;
    }
    if (!check_condition(a.size() == 0, "a is still empty")) {
        fail_count++;
    }
    Set<T> c = b + second;
    if (!check_condition(c.size() == 2, "first and second added: size = 2")) {
        fail_count++;
    }
    if (!check_condition(c.contains(first), "c contains first")) {
        fail_count++;
    }
    if (!check_condition(c.contains(second), "c contains second")) {
        fail_count++;
    }
    if (!check_condition(b.size() == 1, "size of b is still 1")) {
        fail_count++;
    }
    if (!check_condition(!b.contains(second), "b does not contain second")) {
        fail_count++;
    }
    Set<T> d = b + first;
    if (!check_condition(d.size() == 1, "size of d is 1 after added first")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_remove(const T& first, const T& second, const T& third, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, remove ---\n";
    int fail_count = 0;
    Set<T> s;
    s += first;
    s += second;
    s += third;
    s -= second;
    if (!check_condition(s.size() == 2, "size of s is 2 after removing second")) {
        fail_count++;
    }
    if (!check_condition(!s.contains(second), "s does not contain second")) {
        fail_count++;
    }
    if (!check_condition(s[0] == first && s[1] == third, "order of elements is preserved")) {
        fail_count++;
    }
    s -= second;
    if (!check_condition(s.size() == 2, "same size after repeat removal")) {
        fail_count++;
    }
    s -= first;
    s -= third;
    if (!check_condition(s.empty(), "s is empty after removing first and third")) {
        fail_count++;
    }
    s -= first;
    if (!check_condition(s.empty(), "s stays empty when removing from empty set")) {
        fail_count++;
    }
    s += first;
    if (!check_condition(s.size() == 1, "size of s is 1 after added first")) {
        fail_count++;
    }
    if (!check_condition(s.contains(first), "first is in s")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_remove_operator(const T& first, const T& second, const T& third, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, remove operator ---\n";
    int fail_count = 0;
    Set<T> empty;
    Set<T> a = empty + first + second + third;
    Set<T> b = a - second;
    if (!check_condition(b.size() == 2, "second removed: size = 2")) {
        fail_count++;
    }
    if (!check_condition(!b.contains(second), "b does not contain second")) {
        fail_count++;
    }
    if (!check_condition(a.size() == 3, "size of a is still 3")) {
        fail_count++;
    }
    if (!check_condition(a.contains(second), "a contains second")) {
        fail_count++;
    }
    Set<T> c = b - second;
    if (!check_condition(c.size() == 2, "removing a missing element keeps the size of c")) {
        fail_count++;
    }
    if (!check_condition(c.contains(first), "c contains first")) {
        fail_count++;
    }
    if (!check_condition(c.contains(third), "c contains third")) {
        fail_count++;
    }
    if (!check_condition(c[0] == first && c[1] == third, "order of elements in c is preserved")) {
        fail_count++;
    }
    if (!check_condition(b.size() == 2, "size of b is 2 same")) {
        fail_count++;
    }
    if (!check_condition(b[0] == first && b[1] == third, "order of elements in b is preserved")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_array_constructor(const T& first, const T& second, const T& third, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, array constructor ---\n";
    int fail_count = 0;
    const T values1[] = {first, second, third};
    Set<T> a(values1, sizeof(values1) / sizeof(values1[0]));
    if (!check_condition(a.size() == 3, "array of three values: size = 3")) {
        fail_count++;
    }
    if (!check_condition(a[0] == first && a[1] == second && a[2] == third, "order of the elements is the same")) {
        fail_count++;
    }
    const T values2[] = {first, second, first, third, second};
    Set<T> b(values2, sizeof(values2) / sizeof(values2[0]));
    if (!check_condition(b.size() == 3, "duplicates were not added to the Set")) {
        fail_count++;
    }
    if (!check_condition(b[0] == first && b[1] == second && b[2] == third, "the order of elements is preserved based on their first occurrences")) {
        fail_count++;
    }
    Set<T> c(nullptr, 0);
    if (!check_condition(c.empty(), "Set created from an empty array is also empty")) {
        fail_count++;
    }
    bool exception_flag = false;
    try {
        Set<T> d(nullptr, 5);
    } catch (const std::invalid_argument&) {
        exception_flag = true;
    }
    if (!check_condition(exception_flag, "constructor throws invalid_argument for null array")) {
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
    result_count += test_copy<int>("int");
    result_count += test_copy<float>("float");
    result_count += test_copy<std::string>("std::string");
    result_count += test_copy<std::pair<int, double>>("std::pair<int, double>");
    result_count += test_add<int>(2, 5, "int");
    result_count += test_add<float>(2.5f, 3.7f, "float");
    result_count += test_add<std::string>("abc", "cucumber", "std::string");
    result_count += test_add<std::pair<int, double>>({1, 4.6}, {4, 1.4}, "std::pair<int, double>");
    result_count += test_add_operator<int>(2, 5, "int");
    result_count += test_add_operator<float>(2.5f, 3.7f, "float");
    result_count += test_add_operator<std::string>("abc", "cucumber", "std::string");
    result_count += test_add_operator<std::pair<int, double>>({1, 4.6}, {4, 1.4}, "std::pair<int, double>");
    result_count += test_remove<int>(2, 5, 3, "int");
    result_count += test_remove<float>(2.5f, 3.7f, 1.2f, "float");
    result_count += test_remove<std::string>("abc", "cucumber", "def",  "std::string");
    result_count += test_remove<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, "std::pair<int, double>");
    result_count += test_remove_operator<int>(2, 5, 3, "int");
    result_count += test_remove_operator<float>(2.5f, 3.7f, 1.2f, "float");
    result_count += test_remove_operator<std::string>("abc", "cucumber", "def",  "std::string");
    result_count += test_remove_operator<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, "std::pair<int, double>");
    result_count += test_array_constructor<int>(2, 5, 3, "int");
    result_count += test_array_constructor<float>(2.5f, 3.7f, 1.2f, "float");
    result_count += test_array_constructor<std::string>("abc", "cucumber", "def",  "std::string");
    result_count += test_array_constructor<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, "std::pair<int, double>");
    std::cout << "\nFailed: " << result_count << "\n";
    if (result_count == 0) {
        return 0;
    }
    else {
        return 1;
    }
}
