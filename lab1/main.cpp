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

template<typename T>
int test_random_constructor(const T& low, const T& high, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, random constructor ---\n";
    int fail_count = 0;
    Set<T> a(5, low, high);
    if (!check_condition(a.size() == 5, "size equals count")) {
        fail_count++;
    }
    bool out_of_range_flag = false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] < low || a[i] > high) {
            out_of_range_flag = true;
        }
    }
    if (!check_condition(!out_of_range_flag, "all values are within [low, high]")) {
        fail_count++;
    }
    bool exception_flag = false;
    try {
        Set<T> b(5, high, low);
    } catch (const std::invalid_argument&) {
        exception_flag = true;
    }
    if (!check_condition(exception_flag, "constructor throws invalid_argument when low is greater than high")) {
        fail_count++;
    }
    return fail_count;
}

int test_random_constructor_int_range() {
    std::cout << "--- Set<int>, random constructor range ---\n";
    int fail_count = 0;
    bool exception_flag = false;
    try {
        Set<int> b(5, 1, 3);
    } catch (const std::invalid_argument&) {
        exception_flag = true;
    }
    if (!check_condition(exception_flag, "constructor throws invalid_argument when count exceeds the number of values in range")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_union(const T& first, const T& second, const T& third, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, union ---\n";
    int fail_count = 0;
    Set<T> a;
    a += first;
    a += second;
    Set<T> b;
    b += second;
    b += third;
    Set<T> c;
    c = a + b;
    if (!check_condition(c.size() == 3, "size of c is 3")) {
        fail_count++;
    }
    if (!check_condition(c[0] == first && c[1] == second && c[2] == third , "order of elements in c is preserved")) {
        fail_count++;
    }
    if (!check_condition(a.size() == 2 && b.size() == 2, "a and b have not changed")) {
        fail_count++;
    }
    Set<T> empty_set;
    Set<T> d;
    d = a + empty_set;
    Set<T> e;
    e = empty_set + a;
    if (!check_condition(a.size() == d.size() && a.size() == e.size(), "union with an empty set yields a set of the same size")) {
        fail_count++;
    }
    if (!check_condition(a[0] == d[0] && a[1] == d[1], "order of elements in d is preserved")) {
        fail_count++;
    }
    if (!check_condition(a[0] == e[0] && a[1] == e[1], "order of elements in e is preserved")) {
        fail_count++;
    }
    Set<T> f;
    f = a + b + c;
    if (!check_condition(f.size() == 3, "size of f is 3")) {
        fail_count++;
    }
    Set<T> g = a + a;
    if (!check_condition(g.size() == 2, "size of g is 2")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_difference(const T& first, const T& second, const T& third, const T& fourth, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, difference ---\n";
    int fail_count = 0;
    Set<T> a;
    a += first;
    a += second;
    a += third;
    Set<T> b;
    b += second;
    b += third;
    b += fourth;
    Set<T> c;
    c = a - b;
    if (!check_condition(c.size() == 1, "size of c is 1")) {
        fail_count++;
    }
    if (!check_condition(c[0] == first, "c contains only first")) {
        fail_count++;
    }
    Set<T> d;
    d = b - a;
    if (!check_condition(d.size() == 1, "size of d is 1")) {
        fail_count++;
    }
    if (!check_condition(d[0] == fourth, "d contains only fourth")) {
        fail_count++;
    }
    Set<T> second_set;
    second_set += second;
    Set<T> e;
    e = a - second_set;
    if (!check_condition(e[0] == first && e[1] == third, "order of elements in e is preserved")) {
        fail_count++;
    }
    if (!check_condition(a.size() == 3, "size of a is still 3")) {
        fail_count++;
    }
    if (!check_condition(b.size() == 3, "size of b is still 3")) {
        fail_count++;
    }
    Set<T> f;
    f = a - a;
    if (!check_condition(f.empty(), "f is empty")) {
        fail_count++;
    }
    Set<T> empty_set;
    Set<T> g;
    g = a - empty_set;
    if (!check_condition(g.size() == a.size(), "size of g and a is equal")) {
        fail_count++;
    }
    if (!check_condition(g[0] == a[0] && g[1] == a[1] && g[2] == a[2], "order of elements in g is preserved")) {
        fail_count++;
    }
    Set<T> h;
    h = empty_set - a;
    if (!check_condition(h.empty(), "h is empty")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_intersection(const T& first, const T& second, const T& third, const T& fourth, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, intersection ---\n";
    int fail_count = 0;
    Set<T> a;
    a += first;
    a += second;
    a += third;
    Set<T> b;
    b += third;
    b += second;
    b += fourth;
    Set<T> c;
    c = a.intersection(b);
    if (!check_condition(c.size() == 2, "size of c is 2")) {
        fail_count++;
    }
    if (!check_condition(c[0] == second && c[1] == third, "order of elements in c is preserved")) {
        fail_count++;
    }
    Set<T> d;
    d = b.intersection(a);
    if (!check_condition(d.size() == 2, "size of d is 2")) {
        fail_count++;
    }
    if (!check_condition(d[0] == third && d[1] == second, "order of elements in d is preserved")) {
        fail_count++;
    }
    Set<T> set_of_first;
    set_of_first += first;
    Set<T> set_of_fourth;
    set_of_fourth += fourth;
    Set<T> e;
    e = set_of_first.intersection(set_of_fourth);
    if (!check_condition(e.empty(), "e is empty")) {
        fail_count++;
    }
    Set<T> f;
    f = a.intersection(a);
    if (!check_condition(f.size() == a.size(), "size of f and a is equal")) {
        fail_count++;
    }
    if (!check_condition(f[0] == a[0] && f[1] == a[1] && f[2] == a[2], "order of elements in f is preserved")) {
        fail_count++;
    }
    Set<T> empty_set;
    Set<T> g;
    g = a.intersection(empty_set);
    if (!check_condition(g.empty(), "g is empty")) {
        fail_count++;
    }
    Set<T> h;
    h = empty_set.intersection(a);
    if (!check_condition(h.empty(), "h is empty")) {
        fail_count++;
    }
    if (!check_condition(a.size() == 3, "size of a is still 3")) {
        fail_count++;
    }
    if (!check_condition(b.size() == 3, "size of b is still 3")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_equality(const T& first, const T& second, const T& third, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, equality ---\n";
    int fail_count = 0;
    Set<T> a;
    a += first;
    a += second;
    a += third;
    Set<T> b;
    b += third;
    b += first;
    b += second;
    if (!check_condition(a == b, "a is equal b")) {
        fail_count++;
    }
    if (!check_condition(!(a != b), "a is not equal b is false")) {
        fail_count++;
    }
    Set<T> c;
    c += first;
    c += second;
    if (!check_condition(!(a == c), "a is equal c is false")) {
        fail_count++;
    }
    if (!check_condition(a != c, "a is not equal c")) {
        fail_count++;
    }
    Set<T> empty_set_1;
    Set<T> empty_set_2;
    if (!check_condition(empty_set_1 == empty_set_2, "empty sets are equal")) {
        fail_count++;
    }
    if (!check_condition(a == a, "identical sets are equal")) {
        fail_count++;
    }
    c = a;
    c -= first;
    if (!check_condition(a != c, "a is not equal c")) {
        fail_count++;
    }
    c += first;
    if (!check_condition(a == c, "the order of elements does not affect equality")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_task(const T& first, const T& second, const T& third, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, task ---\n";
    int fail_count = 0;
    Set<T> a;
    a += first;
    a += second;
    Set<T> b;
    b += second;
    b += first;
    b += third;
    if (!check_condition(is_subset(a, b), "a is subset b")) {
        fail_count++;
    }
    if (!check_condition(!is_superset(a, b), "a is not superset b")) {
        fail_count++;
    }
    if (!check_condition(!is_subset(b, a), "b is not subset a")) {
        fail_count++;
    }
    if (!check_condition(is_superset(b, a), "b is superset a")) {
        fail_count++;
    }
    Set<T> empty_set_1;
    if (!check_condition(is_subset(empty_set_1, a), "empty set is subset a")) {
        fail_count++;
    }
    if (!check_condition(is_subset(empty_set_1, b), "empty set is subset b")) {
        fail_count++;
    }
    Set<T> c;
    c += third;
    c += first;
    c += second;
    if (!check_condition(is_subset(c, b), "c is subset b")) {
        fail_count++;
    }
    if (!check_condition(is_superset(c, b), "c is superset b")) {
        fail_count++;
    }
    Set<T> d;
    d += first;
    d += second;
    Set<T> e;
    e += second;
    e += third;
    if (!check_condition(!is_subset(e, d), "e is not subset d")) {
        fail_count++;
    }
    if (!check_condition(!is_superset(e, d), "e is not superset d")) {
        fail_count++;
    }
    Set<T> f;
    f += first;
    Set<T> g;
    g += second;
    g += third;
    if (!check_condition(!is_subset(f, g), "f is not subset g")) {
        fail_count++;
    }
    if (!check_condition(!is_superset(f, g), "f is not superset g")) {
        fail_count++;
    }
    Set<T> empty_set_2;
    if (!check_condition(is_subset(empty_set_1, empty_set_2), "empty set is subset of empty set")) {
        fail_count++;
    }
    if (!check_condition(is_superset(empty_set_1, empty_set_2), "empty set is superset of empty set")) {
        fail_count++;
    }
    return fail_count;
}

template<typename T>
int test_relation(const T& first, const T& second, const T& third, const std::string& type_name) {
    std::cout << "--- Set<" << type_name << ">, relation ---\n";
    int fail_count = 0;
    Set<T> a;
    a += first;
    a += second;
    a += third;
    Set<T> b;
    b += third;
    b += first;
    b += second;
    if (!check_condition(get_relation(a, b) == SetRelation::equal, "a equals b")) {
        fail_count++;
    }
    if (!check_condition(get_relation(b, a) == SetRelation::equal, "b equals a")) {
        fail_count++;
    }
    a -= third;
    if (!check_condition(get_relation(a, b) == SetRelation::subset, "a is subset b")) {
        fail_count++;
    }
    if (!check_condition(get_relation(b, a) == SetRelation::superset, "b is superset a")) {
        fail_count++;
    }
    Set<T> c;
    c += first;
    c += second;
    Set<T> d;
    d += second;
    d += third;
    if (!check_condition(get_relation(c, d) == SetRelation::no_nesting, "c and d are not nested")) {
        fail_count++;
    }
    if (!check_condition(get_relation(d, c) == SetRelation::no_nesting, "d and c are not nested")) {
        fail_count++;
    }
    Set<T> empty_set_1;
    if (!check_condition(get_relation(empty_set_1, a) == SetRelation::subset, "empty set is subset a")) {
        fail_count++;
    }
    if (!check_condition(get_relation(a, empty_set_1) == SetRelation::superset, "empty set is superset a")) {
        fail_count++;
    }
    Set<T> empty_set_2;
    if (!check_condition(get_relation(empty_set_1, empty_set_2) == SetRelation::equal, "empty set equals empty set")) {
        fail_count++;
    }
    if (!check_condition(get_relation(a, a) == SetRelation::equal, "a equals a")) {
        fail_count++;
    }
    return fail_count;
}

void run_demo() {
    std::cout << std::boolalpha;
    std::cout << "=== Demonstration on random sets ===\n";
    Set<int> a(6, 1, 15);
    Set<int> b(6, 1, 15);
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a.size() = " << a.size() << "\n";
    std::cout << "a.empty() = " << a.empty() << "\n";
    std::cout << "a[0] = " << a[0] << "\n";
    std::cout << "a.contains(a[0]) = " << a.contains(a[0]) << "\n";
    std::cout << "a.contains(100) = " << a.contains(100) << "\n";
    try {
        std::cout << a[a.size()] << "\n";
    } catch (const std::out_of_range& error) {
        std::cout << "a[a.size()] throws: " << error.what() << "\n";
    }
    Set<int> copied = a;
    std::cout << "copy of a = " << copied << "\n";
    Set<int> assigned;
    assigned = b;
    std::cout << "assignment from b = " << assigned << "\n";
    const int values[] = {4, 8, 4, 15};
    Set<int> from_array(values, sizeof(values) / sizeof(values[0]));
    std::cout << "set from array {4, 8, 4, 15} = " << from_array << "\n";
    std::cout << "a + 100 = " << a + 100 << "\n";
    std::cout << "a - a[0] = " << a - a[0] << "\n";
    Set<int> modified = a;
    modified += 100;
    std::cout << "copy of a after += 100 = " << modified << "\n";
    modified -= 100;
    std::cout << "copy of a after -= 100 = " << modified << "\n";
    std::cout << "a = " << a << " (unchanged)\n";
    std::cout << "a + b = " << a + b << "\n";
    std::cout << "a - b = " << a - b << "\n";
    std::cout << "a.intersection(b) = " << a.intersection(b) << "\n";
    std::cout << "a == b: " << (a == b) << "\n";
    std::cout << "a != b: " << (a != b) << "\n";
    std::cout << "a == copy of a: " << (a == copied) << "\n";
    std::cout << "--- Relations between sets ---\n";
    Set<int> common = a.intersection(b);
    Set<int> united = a + b;
    std::cout << "is_subset(a, b) = " << is_subset(a, b) << "\n";
    std::cout << "is_superset(a, b) = " << is_superset(a, b) << "\n";
    std::cout << "relation(a, b) = " << relation_to_string(get_relation(a, b)) << "\n";
    std::cout << "relation(a, copy of a) = " << relation_to_string(get_relation(a, copied)) << "\n";
    std::cout << "relation(a.intersection(b), a) = " << relation_to_string(get_relation(common, a)) << "\n";
    std::cout << "relation(a, a + b) = " << relation_to_string(get_relation(a, united)) << "\n";
    std::cout << "relation(a + b, a) = " << relation_to_string(get_relation(united, a)) << "\n";
    std::cout << "--- Other element types ---\n";
    Set<float> floats;
    floats += 1.5f;
    floats += 2.5f;
    std::cout << "Set<float> = " << floats << "\n";
    Set<std::string> strings;
    strings += "samara";
    strings += "aisd";
    std::cout << "Set<std::string> = " << strings << "\n";
    Set<std::pair<int, double>> pairs;
    pairs += {1, 2.3};
    pairs += {6, 5.4};
    std::cout << "Set<std::pair<int, double>> = " << pairs << "\n";
    std::cout << std::noboolalpha;
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
    result_count += test_random_constructor<int>(1, 100, "int");
    result_count += test_random_constructor<float>(0.0f, 10.0f, "float");
    result_count += test_random_constructor_int_range();
    result_count += test_union<int>(2, 5, 3, "int");
    result_count += test_union<float>(2.5f, 3.7f, 1.2f, "float");
    result_count += test_union<std::string>("abc", "cucumber", "def", "std::string");
    result_count += test_union<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, "std::pair<int, double>");
    result_count += test_difference<int>(2, 5, 3, 7, "int");
    result_count += test_difference<float>(2.5f, 3.7f, 1.2f, 4.2f, "float");
    result_count += test_difference<std::string>("abc", "cucumber", "def", "samara", "std::string");
    result_count += test_difference<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, {12, 10.3}, "std::pair<int, double>");
    result_count += test_intersection<int>(2, 5, 3, 7, "int");
    result_count += test_intersection<float>(2.5f, 3.7f, 1.2f, 4.2f, "float");
    result_count += test_intersection<std::string>("abc", "cucumber", "def", "samara", "std::string");
    result_count += test_intersection<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, {12, 10.3}, "std::pair<int, double>");
    result_count += test_equality<int>(2, 5, 3, "int");
    result_count += test_equality<float>(2.5f, 3.7f, 1.2f, "float");
    result_count += test_equality<std::string>("abc", "cucumber", "def",  "std::string");
    result_count += test_equality<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, "std::pair<int, double>");
    result_count += test_task<int>(2, 5, 3, "int");
    result_count += test_task<float>(2.5f, 3.7f, 1.2f, "float");
    result_count += test_task<std::string>("abc", "cucumber", "def",  "std::string");
    result_count += test_task<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, "std::pair<int, double>");
    result_count += test_relation<int>(2, 5, 3, "int");
    result_count += test_relation<float>(2.5f, 3.7f, 1.2f, "float");
    result_count += test_relation<std::string>("abc", "cucumber", "def",  "std::string");
    result_count += test_relation<std::pair<int, double>>({1, 4.6}, {4, 1.4}, {2, 3.2}, "std::pair<int, double>");
    run_demo();
    std::cout << "\nFailed: " << result_count << "\n";
    if (result_count == 0) {
        return 0;
    }
    else {
        return 1;
    }
}
