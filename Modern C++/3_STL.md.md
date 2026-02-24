## Chapter 11: Enhanced STL Containers

### 11.1 std::array - The Safe Fixed-Size Container

`std::array` is a fixed-size container that wraps a C-style array, providing the performance of raw arrays with the safety and convenience of STL containers. It's essentially a template wrapper around C arrays that adds useful member functions while maintaining zero overhead.

#### Why Use std::array?

Before C++11, C++ developers had to choose between raw C arrays (fast but unsafe) and `std::vector` (safe but with dynamic allocation overhead). `std::array` gives you the best of both worlds:

```c++
#include <array>
#include <iostream>
#include <algorithm>

int main() {
    // Declaration and initialization
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::array<std::string, 3> strings = {"hello", "world", "C++"};
    
    // Access elements - both methods work
    arr[0] = 10;              // No bounds checking (like C arrays)
    arr.at(1) = 20;           // Bounds checking (throws std::out_of_range if invalid)
    
    // Front and back access
    int& front = arr.front();  // First element - equivalent to arr[0]
    int& back = arr.back();    // Last element - equivalent to arr[arr.size()-1]
    
    // Size is a compile-time constant!
    std::cout << "Size: " << arr.size() << "\n";  // 5
    std::cout << "Max size: " << arr.max_size() << "\n";  // Also 5 (fixed size)
    
    // Fill all elements with a value
    arr.fill(0);  // Sets all elements to 0
    
    // Get underlying C array (for C API compatibility)
    int* cArray = arr.data();
    std::cout << "C array element 0: " << cArray[0] << "\n";
    
    // Range-based for loop (works with all STL containers)
    std::cout << "Array contents: ";
    for (const auto& element : arr) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    // STL algorithms work seamlessly
    std::sort(arr.begin(), arr.end());
    std::reverse(arr.begin(), arr.end());
    
    // Comparison operators (lexicographic comparison)
    std::array<int, 3> a1 = {1, 2, 3};
    std::array<int, 3> a2 = {1, 2, 4};
    std::cout << "a1 < a2: " << (a1 < a2) << "\n";  // true
    
    return 0;
}
```

#### Key Advantages Over C Arrays

1. **Size Awareness**: Unlike C arrays, `std::array` knows its own size at compile time
2. **Iterator Support**: Full STL iterator support enables use with algorithms
3. **No Array Decay**: Doesn't decay to pointer when passed to functions
4. **Copy Semantics**: Can be copied, assigned, and passed by value
5. **Exception Safety**: Bounds-checked access with `.at()` method
6. **Template Friendly**: Works seamlessly in generic code

#### Practical Example: Safe Array Wrapper

```c++
#include <array>
#include <stdexcept>

template<typename T, size_t N>
class SafeArray {
    std::array<T, N> data;
public:
    // Bounds-checked access
    T& operator[](size_t index) {
        if (index >= N) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
    
    // STL-style access
    T& at(size_t index) { return data.at(index); }
    const T& at(size_t index) const { return data.at(index); }
    
    size_t size() const { return N; }
    T* data() { return data.data(); }
    const T* data() const { return data.data(); }
};

int main() {
    SafeArray<int, 5> arr;
    arr[0] = 10;  // Safe bounds checking
    arr[10] = 20; // Throws std::out_of_range
}
```

### 11.2 std::forward_list - The Lightweight Linked List

`std::forward_list` is a singly-linked list introduced in C++11 that provides lower memory overhead compared to `std::list` (which is doubly-linked). It's ideal when you need linked list semantics but want to minimize memory usage.

#### Understanding forward_list Design

The key design decision with `std::forward_list` is that it's implemented as a **forward-only** container. This means:

- **No size() method**: Computing size would be O(n), so it's not provided
- **No reverse iteration**: Can only iterate forward
- **Insertion before position**: Uses `emplace_after()` and `insert_after()`
- **Memory efficient**: Each node only stores one pointer (next), not two (prev + next)

```c++
#include <forward_list>
#include <iostream>
#include <algorithm>

int main() {
    // Construction
    std::forward_list<int> fl = {1, 2, 3, 4, 5};
    std::forward_list<std::string> words{"hello", "world"};
    
    // Access front element only (no back() method)
    std::cout << "Front: " << fl.front() << "\n";  // 1
    fl.front() = 10;  // Modify front element
    
    // Insertion at front (O(1))
    fl.push_front(0);
    fl.emplace_front(42);  // Construct in-place
    
    // Insertion after a position
    auto it = fl.begin();
    ++it;  // Point to second element
    fl.insert_after(it, 99);  // Insert 99 after second element
    fl.emplace_after(it, 88);  // Construct 88 in-place after second element
    
    // Removal from front (O(1))
    fl.pop_front();
    
    // Remove consecutive duplicates (O(n))
    fl.unique();  // Requires sorted data for meaningful results
    
    // Sort the list (O(n log n))
    fl.sort();
    
    // Reverse the list (O(n))
    fl.reverse();
    
    // Erase elements
    fl.remove(3);  // Remove all occurrences of 3
    fl.remove_if([](int x) { return x > 5; });  // Remove elements > 5
    
    // Splice operations (move elements between lists)
    std::forward_list<int> fl2 = {100, 200, 300};
    auto it2 = fl2.begin();
    ++it2;
    fl.splice_after(fl.begin(), fl2, fl2.before_begin(), it2);
    // Moves elements from fl2 to fl
    
    // Print all elements
    std::cout << "Forward list contents: ";
    for (const auto& element : fl) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

#### When to Use forward_list vs list

| Feature | forward_list | list |
|---------|-------------|------|
| Memory per node | 1 pointer | 2 pointers |
| Size tracking | No (O(n) to compute) | Yes (O(1)) |
| Reverse iteration | No | Yes |
| Insertion before position | O(n) | O(1) |
| Insertion after position | O(1) | O(1) |
| Memory overhead | Lower | Higher |

**Use forward_list when:**
- Memory usage is critical
- You only need forward iteration
- You frequently insert/delete at the beginning
- You don't need size() or reverse iteration

**Use list when:**
- You need bidirectional iteration
- You need size() in O(1)
- You frequently insert before arbitrary positions
- Memory overhead is not a concern

#### Practical Example: Memory-Efficient Queue

```c++
#include <forward_list>
#include <iostream>

template<typename T>
class MemoryEfficientQueue {
    std::forward_list<T> data;
    typename std::forward_list<T>::iterator tail;
    
public:
    void push(const T& item) {
        data.push_front(item);
        if (data.size() == 1) {
            tail = data.begin();
        }
    }
    
    void push(T&& item) {
        data.push_front(std::move(item));
        if (data.size() == 1) {
            tail = data.begin();
        }
    }
    
    T& front() {
        return data.back();  // Need to traverse to get back
    }
    
    const T& front() const {
        return data.back();
    }
    
    void pop() {
        if (data.size() == 1) {
            data.pop_front();
            tail = data.end();
        } else {
            // Find second-to-last element
            auto it = data.begin();
            while (std::next(it) != tail) {
                ++it;
            }
            tail = it;
            data.pop_front();  // Remove front element
        }
    }
    
    bool empty() const {
        return data.empty();
    }
};
```

### 11.3 Unordered Containers - Hash-Based Performance

C++11 introduced hash-based containers that provide average O(1) performance for insert, delete, and lookup operations. These containers are particularly useful when you need fast access and don't care about element ordering.

#### Understanding Hash Tables

Hash tables work by:
1. **Hash Function**: Converts keys to array indices
2. **Collision Resolution**: Handles when multiple keys hash to same index
3. **Load Factor**: Ratio of elements to buckets (affects performance)

```c++
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>

int main() {
    // Unordered set - stores unique values
    std::unordered_set<int> uset = {5, 3, 1, 4, 2, 3};  // 3 appears only once
    
    uset.insert(10);
    uset.insert(1);  // Already exists, no change
    
    // Average O(1) lookup
    if (uset.find(5) != uset.end()) {
        std::cout << "Found 5 in set\n";
    }
    
    // Check existence (C++20: contains())
    if (uset.count(5) > 0) {
        std::cout << "5 exists in set\n";
    }
    
    // Unordered map - stores key-value pairs
    std::unordered_map<std::string, int> umap;
    umap["alice"] = 30;
    umap["bob"] = 25;
    umap["charlie"] = 35;
    
    // Insert with hint (can be more efficient)
    umap.insert({"diana", 28});
    umap.emplace("eve", 32);  // Construct in-place
    
    // Access values
    std::cout << "Alice's age: " << umap["alice"] << "\n";
    std::cout << "Bob's age: " << umap.at("bob") << "\n";  // Bounds-checked
    
    // Iterate (order is not guaranteed!)
    std::cout << "Map contents:\n";
    for (const auto& [key, value] : umap) {
        std::cout << key << ": " << value << "\n";
    }
    
    // Bucket information (for understanding hash table internals)
    std::cout << "Buckets: " << umap.bucket_count() << "\n";
    std::cout << "Load factor: " << umap.load_factor() << "\n";
    std::cout << "Max load factor: " << umap.max_load_factor() << "\n";
    
    return 0;
}
```

#### Custom Hash Functions

For custom types, you need to provide a hash function:

```c++
#include <unordered_set>
#include <string>

struct Person {
    std::string name;
    int age;
    
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// Custom hash function
struct PersonHash {
    std::size_t operator()(const Person& p) const {
        // Combine hashes of individual members
        std::size_t h1 = std::hash<std::string>{}(p.name);
        std::size_t h2 = std::hash<int>{}(p.age);
        return h1 ^ (h2 << 1);  // Simple hash combination
    }
};

int main() {
    std::unordered_set<Person, PersonHash> people;
    people.insert({"Alice", 30});
    people.insert({"Bob", 25});
}
```

#### Performance Comparison: Ordered vs Unordered

| Feature | map/set | unordered_map/set |
|---------|---------|-------------------|
| Ordering | Sorted (by key) | No ordering |
| Lookup time | O(log n) | O(1) average, O(n) worst case |
| Insertion time | O(log n) | O(1) average |
| Memory usage | Less | More (hash table overhead) |
| Iteration | Ordered | Unordered |
| Hash function needed | No | Yes (for custom types) |

**When to use unordered containers:**
- Fast lookups are critical
- Order doesn't matter
- You have a good hash function
- Memory overhead is acceptable

**When to use ordered containers:**
- You need sorted iteration
- Memory is constrained
- You need operations like `lower_bound()`, `upper_bound()`
- You need guaranteed O(log n) performance (no worst-case O(n))

#### Practical Example: Word Frequency Counter

```c++
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>

class WordFrequencyCounter {
    std::unordered_map<std::string, int> frequencies;
    
public:
    void addText(const std::string& text) {
        std::istringstream iss(text);
        std::string word;
        while (iss >> word) {
            // Convert to lowercase and remove punctuation
            std::string cleanWord;
            for (char c : word) {
                if (std::isalpha(c)) {
                    cleanWord += std::tolower(c);
                }
            }
            if (!cleanWord.empty()) {
                frequencies[cleanWord]++;
            }
        }
    }
    
    int getFrequency(const std::string& word) const {
        auto it = frequencies.find(word);
        return (it != frequencies.end()) ? it->second : 0;
    }
    
    void printTopWords(int n) const {
        // This would require additional sorting logic
        // For now, just print all words
        for (const auto& [word, freq] : frequencies) {
            std::cout << word << ": " << freq << "\n";
        }
    }
};
```

### 11.4 std::tuple - Heterogeneous Collections

`std::tuple` is a fixed-size collection of heterogeneous values, essentially a generalization of `std::pair`. It's incredibly useful for returning multiple values from functions and for generic programming.

#### Basic Tuple Operations

```c++
#include <tuple>
#include <iostream>
#include <string>
#include <type_traits>

int main() {
    // Creating tuples
    std::tuple<int, double, std::string> t1(42, 3.14, "hello");
    auto t2 = std::make_tuple(100, 2.71, std::string("world"));
    std::tuple<int, int, int> t3{1, 2, 3};  // Uniform initialization
    
    // Accessing elements (0-indexed)
    std::cout << "First element: " << std::get<0>(t1) << "\n";  // 42
    std::cout << "Second element: " << std::get<1>(t1) << "\n";  // 3.14
    std::cout << "Third element: " << std::get<2>(t1) << "\n";  // hello
    
    // Get tuple size at compile time
    std::cout << "Tuple size: " << std::tuple_size<decltype(t1)>::value << "\n";  // 3
    
    // Get type of element at compile time
    using FirstType = std::tuple_element<0, decltype(t1)>::type;
    static_assert(std::is_same_v<FirstType, int>);
    
    // Tuple comparison (lexicographic)
    auto t4 = std::make_tuple(1, 2, 3);
    auto t5 = std::make_tuple(1, 2, 4);
    std::cout << "t4 < t5: " << (t4 < t5) << "\n";  // true
    
    // Tuple concatenation
    auto t6 = std::tuple_cat(t4, t5);  // Creates tuple with 6 elements
    
    // Tuple swap
    std::swap(t1, t2);
    
    return 0;
}
```

#### C++17 Structured Bindings

Structured bindings (introduced in C++17) make working with tuples much more convenient:

```c++
#include <tuple>
#include <string>

std::tuple<int, std::string, double> createPerson() {
    return std::make_tuple(25, "Alice", 5.6);
}

int main() {
    // C++17 structured bindings
    auto [age, name, height] = createPerson();
    std::cout << name << " is " << age << " years old and " << height << " feet tall\n";
    
    // Can also use with references
    auto& [age_ref, name_ref, height_ref] = createPerson();
    // Note: This creates references to temporary objects, which is dangerous!
    // Better to bind to a named tuple first
    
    // Named structured bindings
    auto person = createPerson();
    auto& [p_age, p_name, p_height] = person;
    p_age = 26;  // Modifies the tuple
    
    return 0;
}
```

#### Tuple in Generic Programming

Tuples are particularly powerful in generic programming contexts:

```c++
#include <tuple>
#include <type_traits>

// Function to print any tuple
template<typename Tuple, std::size_t... Indices>
void printTupleImpl(const Tuple& t, std::index_sequence<Indices...>) {
    ((std::cout << std::get<Indices>(t) << " "), ...);  // C++17 fold expression
    std::cout << "\n";
}

template<typename... Args>
void printTuple(const std::tuple<Args...>& t) {
    printTupleImpl(t, std::make_index_sequence<sizeof...(Args)>{});
}

// Function to apply a function to each element of a tuple
template<typename Func, typename Tuple, std::size_t... Indices>
auto applyToTupleImpl(Func&& f, Tuple&& t, std::index_sequence<Indices...>) {
    return std::make_tuple(f(std::get<Indices>(std::forward<Tuple>(t)))...);
}

template<typename Func, typename... Args>
auto applyToTuple(Func&& f, const std::tuple<Args...>& t) {
    return applyToTupleImpl(std::forward<Func>(f), t, 
                           std::make_index_sequence<sizeof...(Args)>{});
}

int main() {
    auto t = std::make_tuple(1, 2.5, std::string("hello"));
    printTuple(t);  // 1 2.5 hello
    
    auto doubled = applyToTuple([](auto x) { return x * 2; }, t);
    printTuple(doubled);  // 2 5 hellohello
}
```

#### Practical Example: Database Row Representation

```c++
#include <tuple>
#include <string>
#include <vector>
#include <iostream>

// Type aliases for clarity
using Row = std::tuple<int, std::string, std::string, double>;

class DatabaseRow {
    Row data;
public:
    DatabaseRow(int id, const std::string& name, const std::string& email, double salary)
        : data(id, name, email, salary) {}
    
    // Getters with type safety
    int getId() const { return std::get<0>(data); }
    const std::string& getName() const { return std::get<1>(data); }
    const std::string& getEmail() const { return std::get<2>(data); }
    double getSalary() const { return std::get<3>(data); }
    
    // Update salary
    void setSalary(double salary) {
        std::get<3>(data) = salary;
    }
    
    // Print row
    void print() const {
        std::cout << "ID: " << getId() << ", Name: " << getName() 
                  << ", Email: " << getEmail() << ", Salary: $" << getSalary() << "\n";
    }
};

class Database {
    std::vector<DatabaseRow> rows;
public:
    void addRow(int id, const std::string& name, const std::string& email, double salary) {
        rows.emplace_back(id, name, email, salary);
    }
    
    // Find row by ID
    DatabaseRow* findRow(int id) {
        for (auto& row : rows) {
            if (row.getId() == id) {
                return &row;
            }
        }
        return nullptr;
    }
    
    // Print all rows
    void printAll() const {
        for (const auto& row : rows) {
            row.print();
        }
    }
};
```

#### Tuple Performance Considerations

- **Compile-time**: Tuple operations are resolved at compile time, so there's no runtime overhead
- **Memory layout**: Elements are stored in order, but padding may be added for alignment
- **Copy semantics**: Tuples use value semantics - copying a tuple copies all elements
- **Move semantics**: Tuples support move semantics for efficient transfers

Tuples are particularly valuable in generic programming and when you need to return multiple values from functions without creating custom classes. They provide type safety while maintaining the flexibility of heterogeneous collections.

---

## Chapter 12: Type Traits and Metaprogramming

### 12.1 Type Traits Overview

Type traits provide compile-time information about types, enabling powerful template metaprogramming techniques. They're defined in `<type_traits>` and form the foundation of modern C++ generic programming.

#### Understanding Type Traits

Type traits are template classes that provide information about types at compile time. They're used extensively in the Standard Library and are essential for writing generic, type-safe code.

```c++
#include <type_traits>
#include <iostream>
#include <typeinfo>

int main() {
    // Primary type categories - fundamental type classification
    std::cout << "Type categories:\n";
    std::cout << "int is integral: " << std::is_integral<int>::value << "\n";
    std::cout << "double is floating point: " << std::is_floating_point<double>::value << "\n";
    std::cout << "int* is pointer: " << std::is_pointer<int*>::value << "\n";
    std::cout << "int& is reference: " << std::is_reference<int&>::value << "\n";
    std::cout << "void is void: " << std::is_void<void>::value << "\n";
    std::cout << "std::string is class: " << std::is_class<std::string>::value << "\n";
    
    // Compound type categories - more complex type relationships
    std::cout << "\nCompound categories:\n";
    std::cout << "int is arithmetic: " << std::is_arithmetic<int>::value << "\n";
    std::cout << "int* is object: " << std::is_object<int*>::value << "\n";
    std::cout << "int() is function: " << std::is_function<int()>::value << "\n";
    std::cout << "int[] is array: " << std::is_array<int[]>::value << "\n";
    std::cout << "int[5] is bounded array: " << std::is_bounded_array<int[5]>::value << "\n";
    std::cout << "int[] is unbounded array: " << std::is_unbounded_array<int[]>::value << "\n";
    
    // Type properties - characteristics of types
    std::cout << "\nType properties:\n";
    std::cout << "const int is const: " << std::is_const<const int>::value << "\n";
    std::cout << "volatile int is volatile: " << std::is_volatile<volatile int>::value << "\n";
    std::cout << "int& is lvalue reference: " << std::is_lvalue_reference<int&>::value << "\n";
    std::cout << "int&& is rvalue reference: " << std::is_rvalue_reference<int&&>::value << "\n";
    std::cout << "int is trivial: " << std::is_trivial<int>::value << "\n";
    std::cout << "int is standard layout: " << std::is_standard_layout<int>::value << "\n";
    std::cout << "int is POD: " << std::is_pod<int>::value << "\n";
    std::cout << "int is trivially copyable: " << std::is_trivially_copyable<int>::value << "\n";
    
    // C++17: _v shortcuts for cleaner syntax
    std::cout << "\nC++17 shortcuts:\n";
    std::cout << "int is integral: " << std::is_integral_v<int> << "\n";
    std::cout << "int* is pointer: " << std::is_pointer_v<int*> << "\n";
    std::cout << "const int is const: " << std::is_const_v<const int> << "\n";
    
    return 0;
}
```

#### Type Relationships

Type traits can also determine relationships between types:

```c++
#include <type_traits>

int main() {
    // Type relationships
    std::cout << "Type relationships:\n";
    std::cout << "int is same as int: " << std::is_same<int, int>::value << "\n";
    std::cout << "int is same as double: " << std::is_same<int, double>::value << "\n";
    std::cout << "int is base of int: " << std::is_base_of<int, int>::value << "\n";
    std::cout << "const int is convertible to int: " << std::is_convertible<const int, int>::value << "\n";
    std::cout << "int* is convertible to void*: " << std::is_convertible<int*, void*>::value << "\n";
    std::cout << "int is assignable from int: " << std::is_assignable<int&, int>::value << "\n";
    std::cout << "const int is assignable from int: " << std::is_assignable<const int&, int>::value << "\n";
    
    // C++20 concepts (if available)
    // std::cout << "int is integral: " << std::integral<int> << "\n";
    // std::cout << "double is floating_point: " << std::floating_point<double> << "\n";
}
```

### 12.2 Type Modifications

Type traits can modify types at compile time, which is incredibly useful for template metaprogramming and generic code.

#### Basic Type Modifications

```c++
#include <type_traits>
#include <iostream>
#include <typeinfo>

template<typename T>
void printType(const std::string& description) {
    std::cout << description << ": " << typeid(T).name() << "\n";
}

int main() {
    // Add/remove const
    printType<std::add_const_t<int>>("add_const_t<int>");
    printType<std::remove_const_t<const int>>("remove_const_t<const int>");
    
    // Add/remove volatile
    printType<std::add_volatile_t<int>>("add_volatile_t<int>");
    printType<std::remove_volatile_t<volatile int>>("remove_volatile_t<volatile int>");
    
    // Add/remove reference
    printType<std::add_lvalue_reference_t<int>>("add_lvalue_reference_t<int>");
    printType<std::add_rvalue_reference_t<int>>("add_rvalue_reference_t<int>");
    printType<std::remove_reference_t<int&>>("remove_reference_t<int&>");
    printType<std::remove_reference_t<int&&>>("remove_reference_t<int&&>");
    
    // Add/remove pointer
    printType<std::add_pointer_t<int>>("add_pointer_t<int>");
    printType<std::remove_pointer_t<int*>>("remove_pointer_t<int*>");
    
    // Add/remove extent (array dimensions)
    printType<std::remove_extent_t<int[5]>>("remove_extent_t<int[5]>");
    printType<std::remove_all_extents_t<int[5][10]>>("remove_all_extents_t<int[5][10]>");
    
    return 0;
}
```

#### Type Decay and Perfect Forwarding

The `std::decay` trait is particularly important for perfect forwarding and template argument deduction:

```c++
#include <type_traits>
#include <iostream>

template<typename T>
void demonstrateDecay() {
    std::cout << "Original: " << typeid(T).name() << "\n";
    std::cout << "Decayed: " << typeid(std::decay_t<T>).name() << "\n\n";
}

int main() {
    std::cout << "Type decay examples:\n";
    demonstrateDecay<int[5]>();           // Array to pointer
    demonstrateDecay<const int&>();       // Reference and cv-qualifiers removed
    demonstrateDecay<int(int, double)>(); // Function to function pointer
    demonstrateDecay<int&&>();            // Rvalue reference removed
    demonstrateDecay<const int>();        // cv-qualifiers removed
    
    return 0;
}
```

#### Common Type and Common Reference

These traits help determine common types for mixed-type operations:

```c++
#include <type_traits>
#include <iostream>

template<typename T, typename U>
void demonstrateCommonTypes() {
    std::cout << "T: " << typeid(T).name() << "\n";
    std::cout << "U: " << typeid(U).name() << "\n";
    std::cout << "Common type: " << typeid(std::common_type_t<T, U>).name() << "\n";
    std::cout << "Common reference: " << typeid(std::common_reference_t<T, U>).name() << "\n\n";
}

int main() {
    demonstrateCommonTypes<int, double>();
    demonstrateCommonTypes<int&, const double&>();
    demonstrateCommonTypes<int&&, double&>();
    
    return 0;
}
```

### 12.3 SFINAE and enable_if

Substitution Failure Is Not An Error (SFINAE) is a fundamental C++ template metaprogramming technique. `std::enable_if` is the primary tool for implementing SFINAE.

#### Understanding SFINAE

SFINAE allows the compiler to gracefully handle template substitution failures by removing invalid template specializations from the overload set rather than causing a compilation error.

```c++
#include <type_traits>
#include <iostream>

// Basic enable_if usage
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
processValue(T value) {
    std::cout << "Processing integral value: " << value << "\n";
    return value * 2;
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
processValue(T value) {
    std::cout << "Processing floating point value: " << value << "\n";
    return value * 2.0;
}

// C++14 style with enable_if_t
template<typename T>
std::enable_if_t<std::is_integral<T>::value, T>
processValueV2(T value) {
    std::cout << "Processing integral value (V2): " << value << "\n";
    return value * 3;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, T>
processValueV2(T value) {
    std::cout << "Processing floating point value (V2): " << value << "\n";
    return value * 3.0;
}

int main() {
    processValue(42);      // Calls integral version
    processValue(3.14);    // Calls floating point version
    
    processValueV2(42);    // Calls integral version
    processValueV2(3.14);  // Calls floating point version
    
    return 0;
}
```

#### Advanced SFINAE Patterns

```c++
#include <type_traits>
#include <iostream>

// Check if a type has a specific member function
template<typename T>
class HasToString {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().toString(), std::true_type{});
    
    template<typename U>
    static std::false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

class MyClass {
public:
    std::string toString() const {
        return "MyClass instance";
    }
};

class OtherClass {
    // No toString method
};

// SFINAE with member detection
template<typename T>
typename std::enable_if<HasToString<T>::value, void>::type
printObject(const T& obj) {
    std::cout << "Object: " << obj.toString() << "\n";
}

template<typename T>
typename std::enable_if<!HasToString<T>::value, void>::type
printObject(const T& obj) {
    std::cout << "Object (no toString): " << &obj << "\n";
}

int main() {
    MyClass obj1;
    OtherClass obj2;
    
    printObject(obj1);  // Uses toString
    printObject(obj2);  // Uses address
    
    return 0;
}
```

#### C++20 Concepts (Alternative to SFINAE)

C++20 introduces concepts, which provide a cleaner alternative to SFINAE for constraining templates:

```c++
// C++20 concepts (commented out as they require C++20)
/*
template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template<Integral T>
T processValue(T value) {
    std::cout << "Processing integral value: " << value << "\n";
    return value * 2;
}

template<FloatingPoint T>
T processValue(T value) {
    std::cout << "Processing floating point value: " << value << "\n";
    return value * 2.0;
}
*/
```

### 12.4 Practical Type Trait Applications

#### Type-Safe Generic Functions

```c++
#include <type_traits>
#include <iostream>
#include <vector>
#include <list>

// Generic container size function that works with different container types
template<typename Container>
typename std::enable_if<
    std::is_same<typename Container::size_type, std::size_t>::value,
    std::size_t
>::type
getContainerSize(const Container& container) {
    return container.size();
}

// Specialized version for C arrays
template<typename T, std::size_t N>
constexpr std::size_t getContainerSize(const T (&)[N]) {
    return N;
}

// Generic value printer with type-specific formatting
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
printValue(const T& value) {
    std::cout << "Numeric value: " << value << "\n";
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value, void>::type
printValue(const T& value) {
    std::cout << "String value: \"" << value << "\"\n";
}

template<typename T>
typename std::enable_if<std::is_class<T>::value && !std::is_same<T, std::string>::value, void>::type
printValue(const T& value) {
    std::cout << "Object value: " << &value << "\n";
}

int main() {
    std::vector<int> vec = {1, 2, 3};
    std::list<double> lst = {1.1, 2.2, 3.3};
    int arr[] = {1, 2, 3, 4, 5};
    
    std::cout << "Container sizes:\n";
    std::cout << "Vector: " << getContainerSize(vec) << "\n";
    std::cout << "List: " << getContainerSize(lst) << "\n";
    std::cout << "Array: " << getContainerSize(arr) << "\n";
    
    std::cout << "\nValue printing:\n";
    printValue(42);
    printValue(3.14);
    printValue(std::string("hello"));
    printValue(vec);
    
    return 0;
}
```

#### Template Metaprogramming with Type Traits

```c++
#include <type_traits>
#include <iostream>

// Compile-time type list
template<typename... Types>
struct TypeList {};

// Type list operations using type traits
template<typename T, typename TypeList>
struct Contains;

template<typename T, typename... Types>
struct Contains<T, TypeList<Types...>> : std::false_type {};

template<typename T, typename... Types>
struct Contains<T, TypeList<T, Types...>> : std::true_type {};

template<typename T, typename First, typename... Rest>
struct Contains<T, TypeList<First, Rest...>> : Contains<T, TypeList<Rest...>> {};

// Type list size
template<typename TypeList>
struct Size;

template<typename... Types>
struct Size<TypeList<Types...>> : std::integral_constant<std::size_t, sizeof...(Types)> {};

// Type list front
template<typename TypeList>
struct Front;

template<typename T, typename... Types>
struct Front<TypeList<T, Types...>> {
    using type = T;
};

// Type list pop front
template<typename TypeList>
struct PopFront;

template<typename T, typename... Types>
struct PopFront<TypeList<T, Types...>> {
    using type = TypeList<Types...>;
};

int main() {
    using MyTypes = TypeList<int, double, std::string, char>;
    
    std::cout << "TypeList operations:\n";
    std::cout << "Contains int: " << Contains<int, MyTypes>::value << "\n";
    std::cout << "Contains float: " << Contains<float, MyTypes>::value << "\n";
    std::cout << "Size: " << Size<MyTypes>::value << "\n";
    std::cout << "Front type is int: " << std::is_same_v<typename Front<MyTypes>::type, int> << "\n";
    std::cout << "Pop front size: " << Size<typename PopFront<MyTypes>::type>::value << "\n";
    
    return 0;
}
```

#### Type Traits for Performance Optimization

```c++
#include <type_traits>
#include <iostream>
#include <vector>
#include <algorithm>

// Optimized copy function that uses memcpy for trivially copyable types
template<typename InputIt, typename OutputIt>
typename std::enable_if<
    std::is_trivially_copyable<typename std::iterator_traits<InputIt>::value_type>::value &&
    std::is_pointer<InputIt>::value &&
    std::is_pointer<OutputIt>::value,
    OutputIt
>::type
fastCopy(InputIt first, InputIt last, OutputIt result) {
    std::cout << "Using optimized memcpy\n";
    auto count = last - first;
    std::memcpy(result, first, count * sizeof(*first));
    return result + count;
}

template<typename InputIt, typename OutputIt>
typename std::enable_if<
    !std::is_trivially_copyable<typename std::iterator_traits<InputIt>::value_type>::value ||
    !std::is_pointer<InputIt>::value ||
    !std::is_pointer<OutputIt>::value,
    OutputIt
>::type
fastCopy(InputIt first, InputIt last, OutputIt result) {
    std::cout << "Using standard copy\n";
    return std::copy(first, last, result);
}

int main() {
    std::vector<int> source = {1, 2, 3, 4, 5};
    std::vector<int> dest1(5);
    std::vector<std::string> source2 = {"hello", "world"};
    std::vector<std::string> dest2(2);
    
    // Will use optimized version (trivially copyable + pointers)
    fastCopy(source.data(), source.data() + source.size(), dest1.data());
    
    // Will use standard version (not trivially copyable)
    fastCopy(source2.begin(), source2.end(), dest2.begin());
    
    return 0;
}
```

### 12.5 Summary

Type traits and metaprogramming are powerful tools that enable writing generic, efficient, and type-safe code. They form the foundation of modern C++ template programming and are extensively used throughout the Standard Library.

**Key Concepts**:
1. **Type Classification**: Determine what kind of type you're working with
2. **Type Modification**: Transform types at compile time
3. **SFINAE**: Enable/disable template specializations based on type properties
4. **Template Metaprogramming**: Perform computations and logic at compile time

**Best Practices**:
1. **Use C++17 `_v` suffixes** for cleaner syntax when possible
2. **Prefer concepts over SFINAE** in C++20 and later
3. **Use type traits for optimization** when you can guarantee safety
4. **Document your type requirements** clearly in template code
5. **Test with various type combinations** to ensure robustness

Type traits enable writing code that adapts to the types it works with, making C++ templates both powerful and flexible while maintaining type safety and performance.

---

## Chapter 13: Concurrency Support

### 13.1 Thread Basics

C++11 introduced standardized threading support, providing a portable way to write concurrent programs. This was a major milestone that brought C++ into the modern era of concurrent programming.

#### Understanding Threading Concepts

Before diving into the code, it's important to understand some fundamental concepts:

- **Thread**: An independent sequence of execution within a program
- **Concurrency**: Multiple threads executing simultaneously
- **Synchronization**: Coordinating access to shared resources
- **Race Condition**: When multiple threads access shared data simultaneously with undefined behavior
- **Data Race**: A specific type of race condition that violates memory model rules

```c++
#include <thread>
#include <iostream>
#include <chrono>
#include <string>

// Function to run in a thread
void printMessage(const std::string& message, int times) {
    for (int i = 0; i < times; ++i) {
        std::cout << "Thread " << std::this_thread::get_id() 
                  << ": " << message << " (iteration " << i + 1 << ")\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Lambda function example
void lambdaThreadExample() {
    std::thread t([](int id) {
        for (int i = 0; i < 3; ++i) {
            std::cout << "Lambda thread " << id << ": iteration " << i + 1 << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }, 42);
    
    t.join();
}

// Class method example
class Worker {
    std::string name;
public:
    Worker(const std::string& n) : name(n) {}
    
    void doWork(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            std::cout << "Worker " << name << ": task " << i + 1 << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(75));
        }
    }
};

int main() {
    std::cout << "Main thread ID: " << std::this_thread::get_id() << "\n\n";
    
    // Create thread with function
    std::thread t1(printMessage, "Hello from function", 3);
    
    // Create thread with lambda
    std::thread t2([]() {
        printMessage("Hello from lambda", 2);
    });
    
    // Create thread with class method
    Worker worker("Alice");
    std::thread t3(&Worker::doWork, &worker, 3);
    
    // Create thread with member function and arguments
    std::thread t4(&Worker::doWork, Worker("Bob"), 2);
    
    // Demonstrate lambda thread
    lambdaThreadExample();
    
    // Wait for all threads to complete
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    std::cout << "\nAll threads completed\n";
    return 0;
}
```

#### Thread Management Best Practices

```c++
#include <thread>
#include <iostream>
#include <vector>
#include <memory>

class ThreadManager {
    std::vector<std::thread> threads;
    
public:
    // RAII: Ensure threads are joined in destructor
    ~ThreadManager() {
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }
    
    // Add a thread to the manager
    template<typename Func, typename... Args>
    void addThread(Func&& func, Args&&... args) {
        threads.emplace_back(std::forward<Func>(func), std::forward<Args>(args)...);
    }
    
    // Wait for all threads to complete
    void waitForAll() {
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        threads.clear();
    }
    
    // Get number of active threads
    size_t activeCount() const {
        size_t count = 0;
        for (const auto& t : threads) {
            if (t.joinable()) {
                ++count;
            }
        }
        return count;
    }
};

void workerFunction(int id, int workTime) {
    std::cout << "Worker " << id << " starting work for " << workTime << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(workTime));
    std::cout << "Worker " << id << " completed work\n";
}

int main() {
    ThreadManager manager;
    
    // Create multiple threads with different workloads
    for (int i = 0; i < 5; ++i) {
        manager.addThread(workerFunction, i, 100 + i * 50);
    }
    
    std::cout << "Created " << manager.activeCount() << " threads\n";
    
    // Wait for all threads to complete
    manager.waitForAll();
    std::cout << "All threads completed\n";
    
    return 0;
}
```

### 13.2 Mutex and Locks

Mutexes (mutual exclusion) are the fundamental synchronization primitive for protecting shared data from concurrent access.

#### Understanding Mutex Types

C++ provides several types of mutexes for different use cases:

1. **std::mutex**: Basic mutex with exclusive ownership
2. **std::recursive_mutex**: Allows the same thread to lock multiple times
3. **std::timed_mutex**: Supports timeout-based locking
4. **std::recursive_timed_mutex**: Combination of recursive and timed

```c++
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

class ThreadSafeCounter {
    int value = 0;
    std::mutex mtx;
    
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mtx);  // RAII lock
        ++value;
    }
    
    void incrementBy(int amount) {
        std::lock_guard<std::mutex> lock(mtx);
        value += amount;
    }
    
    int get() const {
        std::lock_guard<std::mutex> lock(mtx);
        return value;
    }
};

class AdvancedCounter {
    int value = 0;
    mutable std::mutex mtx;  // mutable allows locking in const methods
    
public:
    void increment() {
        std::unique_lock<std::mutex> lock(mtx);  // More flexible than lock_guard
        ++value;
    }
    
    int get() const {
        std::unique_lock<std::mutex> lock(mtx);
        return value;
    }
    
    void incrementBatch(int count) {
        std::unique_lock<std::mutex> lock(mtx);
        for (int i = 0; i < count; ++i) {
            ++value;
            if (i % 1000 == 0) {
                lock.unlock();  // Temporarily release lock
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                lock.lock();    // Re-acquire lock
            }
        }
    }
};

class TimedCounter {
    int value = 0;
    std::timed_mutex mtx;
    
public:
    bool tryIncrement(int timeoutMs = 100) {
        // Try to acquire lock with timeout
        if (mtx.try_lock_for(std::chrono::milliseconds(timeoutMs))) {
            ++value;
            mtx.unlock();
            return true;
        }
        return false;  // Could not acquire lock within timeout
    }
    
    void increment() {
        mtx.lock();
        ++value;
        mtx.unlock();
    }
};

void testBasicCounter() {
    ThreadSafeCounter counter;
    std::vector<std::thread> threads;
    
    // Create multiple threads that increment the counter
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.increment();
            }
        });
    }
    
    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Basic counter final value: " << counter.get() << "\n";
}

void testAdvancedCounter() {
    AdvancedCounter counter;
    std::vector<std::thread> threads;
    
    // Test batch incrementation
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&counter]() {
            counter.incrementBatch(2000);
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Advanced counter final value: " << counter.get() << "\n";
}

void testTimedCounter() {
    TimedCounter counter;
    std::vector<std::thread> threads;
    
    // Create a thread that holds the lock for a while
    threads.emplace_back([&counter]() {
        counter.increment();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    });
    
    // Try to increment with timeout while the lock is held
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    bool success = counter.tryIncrement(100);
    std::cout << "Timed increment success: " << success << "\n";
    
    // Wait for the first thread to complete
    threads[0].join();
    
    // Now it should succeed
    success = counter.tryIncrement(100);
    std::cout << "Timed increment success (after lock released): " << success << "\n";
}

int main() {
    std::cout << "Testing basic counter:\n";
    testBasicCounter();
    
    std::cout << "\nTesting advanced counter:\n";
    testAdvancedCounter();
    
    std::cout << "\nTesting timed counter:\n";
    testTimedCounter();
    
    return 0;
}
```

#### Deadlock Prevention

Deadlocks occur when multiple threads wait for each other to release locks. Here are strategies to prevent them:

```c++
#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>

class Account {
    int balance = 0;
    mutable std::mutex mtx;
    std::string name;
    
public:
    Account(const std::string& n, int initialBalance) 
        : name(n), balance(initialBalance) {}
    
    bool transferTo(Account& other, int amount) {
        // Strategy 1: Lock ordering (always lock by address)
        std::lock(this->mtx, other.mtx);
        std::lock_guard<std::mutex> lock1(this->mtx, std::adopt_lock);
        std::lock_guard<std::mutex> lock2(other.mtx, std::adopt_lock);
        
        if (balance >= amount) {
            balance -= amount;
            other.balance += amount;
            std::cout << name << " -> " << other.name << ": " << amount << "\n";
            return true;
        }
        return false;
    }
    
    int getBalance() const {
        std::lock_guard<std::mutex> lock(mtx);
        return balance;
    }
};

class DeadlockSafeBank {
    std::vector<Account> accounts;
    mutable std::mutex mtx;
    
public:
    DeadlockSafeBank(int numAccounts, int initialBalance) {
        accounts.reserve(numAccounts);
        for (int i = 0; i < numAccounts; ++i) {
            accounts.emplace_back("Account" + std::to_string(i), initialBalance);
        }
    }
    
    bool transfer(int from, int to, int amount) {
        if (from == to) return true;
        if (from < 0 || from >= accounts.size()) return false;
        if (to < 0 || to >= accounts.size()) return false;
        
        // Strategy 2: Single lock for the entire operation
        std::lock_guard<std::mutex> lock(mtx);
        return accounts[from].transferTo(accounts[to], amount);
    }
    
    int getBalance(int account) const {
        if (account < 0 || account >= accounts.size()) return -1;
        return accounts[account].getBalance();
    }
};

void bankTransferWorker(DeadlockSafeBank& bank, int numTransfers) {
    for (int i = 0; i < numTransfers; ++i) {
        int from = rand() % 10;
        int to = rand() % 10;
        int amount = rand() % 100;
        bank.transfer(from, to, amount);
    }
}

int main() {
    DeadlockSafeBank bank(10, 1000);
    
    std::vector<std::thread> workers;
    for (int i = 0; i < 5; ++i) {
        workers.emplace_back(bankTransferWorker, std::ref(bank), 1000);
    }
    
    for (auto& w : workers) {
        w.join();
    }
    
    std::cout << "Final balances:\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << "Account " << i << ": " << bank.getBalance(i) << "\n";
    }
    
    return 0;
}
```

### 13.3 Condition Variables

Condition variables allow threads to wait for specific conditions to become true, enabling efficient coordination between threads.

#### Understanding Condition Variables

Condition variables work with mutexes to provide a way for threads to wait until a particular condition is met. They're essential for implementing producer-consumer patterns, barriers, and other synchronization primitives.

```c++
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>
#include <chrono>

template<typename T>
class ThreadSafeQueue {
    std::queue<T> queue;
    mutable std::mutex mtx;
    std::condition_variable cv;
    bool closed = false;
    
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(value));
        cv.notify_one();  // Notify one waiting thread
    }
    
    bool pop(T& value) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Wait until queue is not empty or closed
        cv.wait(lock, [this]() { return !queue.empty() || closed; });
        
        if (queue.empty()) {
            return false;  // Queue is closed and empty
        }
        
        value = std::move(queue.front());
        queue.pop();
        return true;
    }
    
    void close() {
        std::lock_guard<std::mutex> lock(mtx);
        closed = true;
        cv.notify_all();  // Notify all waiting threads
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }
};

void producer(ThreadSafeQueue<int>& queue, int id, int count) {
    for (int i = 0; i < count; ++i) {
        int value = id * 1000 + i;
        queue.push(value);
        std::cout << "Producer " << id << " produced: " << value << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(ThreadSafeQueue<int>& queue, int id) {
    int count = 0;
    int value;
    
    while (queue.pop(value)) {
        std::cout << "Consumer " << id << " consumed: " << value << "\n";
        ++count;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    
    std::cout << "Consumer " << id << " processed " << count << " items\n";
}

int main() {
    ThreadSafeQueue<int> queue;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    // Start producers
    for (int i = 0; i < 3; ++i) {
        producers.emplace_back(producer, std::ref(queue), i, 10);
    }
    
    // Start consumers
    for (int i = 0; i < 2; ++i) {
        consumers.emplace_back(consumer, std::ref(queue), i);
    }
    
    // Wait for producers to finish
    for (auto& p : producers) {
        p.join();
    }
    
    // Close the queue to signal consumers to stop
    queue.close();
    
    // Wait for consumers to finish
    for (auto& c : consumers) {
        c.join();
    }
    
    std::cout << "All producers and consumers completed\n";
    return 0;
}
```

#### Advanced Condition Variable Patterns

```c++
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>

class Barrier {
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    int threshold;
    int generation;
    
public:
    Barrier(int numThreads) : count(0), threshold(numThreads), generation(0) {}
    
    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        int currentGen = generation;
        
        ++count;
        if (count == threshold) {
            // Last thread to arrive
            count = 0;
            ++generation;
            cv.notify_all();
        } else {
            // Wait for all threads to arrive
            cv.wait(lock, [this, currentGen]() {
                return generation != currentGen;
            });
        }
    }
};

class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
    Barrier barrier;
    
public:
    ThreadPool(int numThreads) : barrier(numThreads + 1) {
        for (int i = 0; i < numThreads; ++i) {
            workers.emplace_back([this]() { workerLoop(); });
        }
    }
    
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (auto& w : workers) {
            w.join();
        }
    }
    
    template<typename Func, typename... Args>
    void enqueue(Func&& func, Args&&... args) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            tasks.emplace([=]() {
                func(args...);
            });
        }
        cv.notify_one();
    }
    
    void waitForAll() {
        barrier.wait();
    }
    
private:
    void workerLoop() {
        while (true) {
            std::function<void()> task;
            
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                
                if (stop && tasks.empty()) {
                    break;
                }
                
                if (!tasks.empty()) {
                    task = std::move(tasks.front());
                    tasks.pop();
                }
            }
            
            if (task) {
                task();
            }
        }
    }
};

void workerTask(int id, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::cout << "Worker " << id << " iteration " << i + 1 << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    ThreadPool pool(3);
    
    // Enqueue tasks
    for (int i = 0; i < 6; ++i) {
        pool.enqueue(workerTask, i, 3);
    }
    
    // Wait for all tasks to complete
    pool.waitForAll();
    std::cout << "All tasks completed\n";
    
    return 0;
}
```

### 13.4 Atomic Operations

Atomic operations provide lock-free synchronization for simple operations on fundamental types, offering better performance than mutexes for certain use cases.

#### Understanding Atomic Types

Atomic types guarantee that operations on them are indivisible and visible to other threads without explicit synchronization. They're particularly useful for counters, flags, and simple state variables.

```c++
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

class AtomicCounter {
    std::atomic<int> value{0};
    std::atomic<bool> running{true};
    
public:
    void increment() {
        value.fetch_add(1, std::memory_order_relaxed);
    }
    
    int get() const {
        return value.load(std::memory_order_relaxed);
    }
    
    void reset() {
        value.store(0, std::memory_order_relaxed);
    }
    
    void stop() {
        running.store(false, std::memory_order_relaxed);
    }
    
    bool isRunning() const {
        return running.load(std::memory_order_relaxed);
    }
};

class LockFreeStack {
private:
    struct Node {
        int data;
        Node* next;
        Node(int value) : data(value), next(nullptr) {}
    };
    
    std::atomic<Node*> head{nullptr};
    
public:
    void push(int value) {
        Node* newNode = new Node(value);
        Node* oldHead;
        
        do {
            oldHead = head.load(std::memory_order_relaxed);
            newNode->next = oldHead;
        } while (!head.compare_exchange_weak(newNode->next, newNode,
                                           std::memory_order_release,
                                           std::memory_order_relaxed));
    }
    
    bool pop(int& value) {
        Node* oldHead = head.load(std::memory_order_relaxed);
        
        while (oldHead != nullptr) {
            if (head.compare_exchange_weak(oldHead, oldHead->next,
                                         std::memory_order_consume,
                                         std::memory_order_relaxed)) {
                value = oldHead->data;
                delete oldHead;
                return true;
            }
        }
        return false;
    }
};

void atomicCounterWorker(AtomicCounter& counter, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        counter.increment();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

void lockFreeStackWorker(LockFreeStack& stack, int id, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        stack.push(id * 1000 + i);
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}

int main() {
    std::cout << "Testing atomic counter:\n";
    AtomicCounter counter;
    std::vector<std::thread> threads;
    
    // Test atomic counter
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(atomicCounterWorker, std::ref(counter), 1000);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final counter value: " << counter.get() << "\n";
    
    std::cout << "\nTesting lock-free stack:\n";
    LockFreeStack stack;
    threads.clear();
    
    // Test lock-free stack
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(lockFreeStackWorker, std::ref(stack), i, 100);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // Pop all values from stack
    int value;
    int count = 0;
    while (stack.pop(value)) {
        ++count;
    }
    std::cout << "Popped " << count << " values from stack\n";
    
    return 0;
}
```

#### Memory Ordering

Memory ordering specifies the visibility and ordering constraints for atomic operations. Understanding memory ordering is crucial for writing correct lock-free code.

```c++
#include <atomic>
#include <thread>
#include <iostream>

std::atomic<bool> ready{false};
std::atomic<int> data{0};

void producer() {
    data.store(42, std::memory_order_relaxed);  // Write data
    ready.store(true, std::memory_order_release);  // Signal that data is ready
}

void consumer() {
    while (!ready.load(std::memory_order_acquire)) {  // Wait for signal
        std::this_thread::yield();
    }
    std::cout << "Data: " << data.load(std::memory_order_relaxed) << "\n";
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
    
    return 0;
}
```

### 13.5 Futures and Promises

Futures and promises provide a high-level abstraction for asynchronous programming, allowing you to obtain results from asynchronous operations.

#### Understanding Futures and Promises

- **Promise**: A writable future value that can be set exactly once
- **Future**: A readable handle to a value that may not be available yet
- **std::async**: Launches a function asynchronously and returns a future

```c++
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

// Function that returns a future
std::future<int> asyncCompute(int value) {
    return std::async(std::launch::async, [value]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return value * 2;
    });
}

// Function that uses a promise
std::future<std::string> asyncStringOperation(const std::string& input) {
    auto promise = std::make_shared<std::promise<std::string>>();
    auto future = promise->get_future();
    
    std::thread([promise, input]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        promise->set_value("Processed: " + input);
    }).detach();
    
    return future;
}

// Exception handling with futures
std::future<void> asyncExceptionTest() {
    return std::async(std::launch::async, []() {
        throw std::runtime_error("Something went wrong!");
    });
}

// Multiple futures with when_all
template<typename... Futures>
auto whenAll(Futures&&... futures) {
    return std::async(std::launch::deferred, [futures = std::make_tuple(std::forward<Futures>(futures)...)]() mutable {
        std::apply([](auto&... fs) { (fs.wait(), ...); }, futures);
    });
}

int main() {
    std::cout << "Testing basic futures:\n";
    
    // Basic async operation
    auto future1 = asyncCompute(21);
    std::cout << "Computing...\n";
    std::cout << "Result: " << future1.get() << "\n";
    
    // String operation with promise
    auto future2 = asyncStringOperation("Hello World");
    std::cout << "Processing string...\n";
    std::cout << "Result: " << future2.get() << "\n";
    
    // Exception handling
    std::cout << "\nTesting exception handling:\n";
    auto future3 = asyncExceptionTest();
    try {
        future3.get();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
    
    // Multiple futures
    std::cout << "\nTesting multiple futures:\n";
    std::vector<std::future<int>> futures;
    for (int i = 1; i <= 5; ++i) {
        futures.push_back(std::async(std::launch::async, [i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 100));
            return i * 10;
        }));
    }
    
    // Wait for all futures
    for (auto& f : futures) {
        std::cout << "Result: " << f.get() << "\n";
    }
    
    return 0;
}
```

#### Advanced Future Patterns

```c++
#include <future>
#include <iostream>
#include <vector>
#include <algorithm>

// Continuation pattern
template<typename Future, typename Func>
auto then(Future&& future, Func&& func) {
    using ResultType = std::invoke_result_t<Func, typename Future::value_type>;
    auto promise = std::make_shared<std::promise<ResultType>>();
    auto result = promise->get_future();
    
    std::thread([promise = std::move(promise), future = std::forward<Future>(future), func = std::forward<Func>(func)]() mutable {
        try {
            auto value = future.get();
            promise->set_value(func(std::move(value)));
        } catch (...) {
            promise->set_exception(std::current_exception());
        }
    }).detach();
    
    return result;
}

// Race condition detection
template<typename... Futures>
auto firstReady(Futures&&... futures) {
    return std::async(std::launch::deferred, [futures = std::make_tuple(std::forward<Futures>(futures)...)]() mutable {
        std::vector<std::future_status> statuses;
        statuses.reserve(sizeof...(Futures));
        
        while (true) {
            bool anyReady = false;
            statuses.clear();
            
            std::apply([&](auto&... fs) {
                (statuses.push_back(fs.wait_for(std::chrono::milliseconds(0))), ...);
            }, futures);
            
            for (size_t i = 0; i < statuses.size(); ++i) {
                if (statuses[i] == std::future_status::ready) {
                    return i;  // Return index of first ready future
                }
            }
            
            if (!anyReady) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    });
}

int main() {
    // Demonstrate continuation
    auto future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 42;
    });
    
    auto chained = then(std::move(future), [](int value) {
        return value * 2;
    });
    
    std::cout << "Chained result: " << chained.get() << "\n";
    
    return 0;
}
```

### 13.6 Summary

C++11's concurrency support provides a comprehensive toolkit for writing concurrent programs. Understanding these tools is essential for modern C++ development.

**Key Concepts**:
1. **Threads**: Independent execution sequences
2. **Mutexes**: Protect shared data from concurrent access
3. **Condition Variables**: Coordinate threads based on conditions
4. **Atomic Operations**: Lock-free synchronization for simple operations
5. **Futures/Promises**: High-level asynchronous programming

**Best Practices**:
1. **Prefer higher-level abstractions** (futures, async) when possible
2. **Use RAII for lock management** (lock_guard, unique_lock)
3. **Avoid data races** through proper synchronization
4. **Minimize lock contention** by keeping critical sections small
5. **Test thoroughly** for race conditions and deadlocks
6. **Consider lock-free algorithms** for performance-critical code

Concurrency is a complex topic that requires careful design and testing. Start with simple patterns and gradually build complexity as you gain experience with the synchronization primitives.

---

## Chapter 14: Chrono Library

### 14.1 Duration - Measuring Time Intervals

The `std::chrono` library provides a comprehensive system for working with time in C++. It's designed to be type-safe, efficient, and easy to use for both compile-time and runtime time operations.

#### Understanding Duration Types

Durations represent time intervals and are parameterized by both the representation type and the time unit. This design provides type safety and prevents common errors like mixing different time units.

```c++
#include <chrono>
#include <iostream>
#include <thread>

void demonstrateBasicDurations() {
    using namespace std::chrono;
    
    // Basic duration creation
    seconds s(10);                    // 10 seconds
    milliseconds ms(500);             // 500 milliseconds
    microseconds us(1000);            // 1000 microseconds
    nanoseconds ns(1000000);          // 1000000 nanoseconds
    
    std::cout << "Basic durations:\n";
    std::cout << "Seconds: " << s.count() << "\n";
    std::cout << "Milliseconds: " << ms.count() << "\n";
    std::cout << "Microseconds: " << us.count() << "\n";
    std::cout << "Nanoseconds: " << ns.count() << "\n";
    
    // Duration arithmetic
    auto total = s + ms;              // 10.5 seconds (duration<double>)
    auto difference = s - ms;         // 9.5 seconds
    auto product = ms * 2;            // 1000 milliseconds
    auto quotient = s / 2;            // 5 seconds
    
    std::cout << "\nDuration arithmetic:\n";
    std::cout << "Total (s + ms): " << total.count() << " seconds\n";
    std::cout << "Difference (s - ms): " << difference.count() << " seconds\n";
    std::cout << "Product (ms * 2): " << product.count() << " milliseconds\n";
    std::cout << "Quotient (s / 2): " << quotient.count() << " seconds\n";
}

void demonstrateDurationCasting() {
    using namespace std::chrono;
    
    // Duration casting between different units
    seconds s(10);
    milliseconds ms = duration_cast<milliseconds>(s);  // 10000ms
    microseconds us = duration_cast<microseconds>(s);  // 10000000us
    nanoseconds ns = duration_cast<nanoseconds>(s);    // 10000000000ns
    
    std::cout << "\nDuration casting:\n";
    std::cout << "10 seconds = " << ms.count() << " milliseconds\n";
    std::cout << "10 seconds = " << us.count() << " microseconds\n";
    std::cout << "10 seconds = " << ns.count() << " nanoseconds\n";
    
    // Casting with potential precision loss
    milliseconds ms2(1500);
    seconds s2 = duration_cast<seconds>(ms2);  // 1 second (loses 500ms)
    std::cout << "1500 milliseconds = " << s2.count() << " seconds (precision lost)\n";
    
    // Safe casting that preserves precision
    auto s3 = duration<double, std::ratio<1>>(ms2);  // 1.5 seconds
    std::cout << "1500 milliseconds = " << s3.count() << " seconds (precision preserved)\n";
}

void demonstrateDurationLiterals() {
    using namespace std::chrono_literals;
    using namespace std::chrono;
    
    // C++14 literal suffixes for durations
    auto d1 = 10s;        // seconds
    auto d2 = 500ms;      // milliseconds
    auto d3 = 100us;      // microseconds
    auto d4 = 100ns;      // nanoseconds
    auto d5 = 2min;       // minutes
    auto d6 = 1h;         // hours
    
    std::cout << "\nDuration literals:\n";
    std::cout << "10s = " << d1.count() << " seconds\n";
    std::cout << "500ms = " << d2.count() << " milliseconds\n";
    std::cout << "100us = " << d3.count() << " microseconds\n";
    std::cout << "100ns = " << d4.count() << " nanoseconds\n";
    std::cout << "2min = " << d5.count() << " minutes\n";
    std::cout << "1h = " << d6.count() << " hours\n";
    
    // Duration arithmetic with literals
    auto total = 10s + 500ms + 2min;
    std::cout << "Total (10s + 500ms + 2min): " << total.count() << " seconds\n";
}

void demonstrateCustomDurationUnits() {
    using namespace std::chrono;
    
    // Custom duration with different representation
    using HighResolutionClock = duration<long long, std::ratio<1, 1000000000>>;  // Nanosecond precision
    using MillisecondClock = duration<double, std::milli>;  // Millisecond precision with double
    
    HighResolutionClock hr(1000000000);  // 1 second in nanoseconds
    MillisecondClock ms(1000.5);         // 1000.5 milliseconds
    
    std::cout << "\nCustom duration units:\n";
    std::cout << "High resolution: " << hr.count() << " nanoseconds\n";
    std::cout << "Millisecond with double: " << ms.count() << " milliseconds\n";
    
    // Converting between custom units
    auto converted = duration_cast<seconds>(hr);
    std::cout << "Converted to seconds: " << converted.count() << "\n";
}

int main() {
    demonstrateBasicDurations();
    demonstrateDurationCasting();
    demonstrateDurationLiterals();
    demonstrateCustomDurationUnits();
    return 0;
}
```

#### Duration in Practice

```c++
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

class Timer {
    std::chrono::high_resolution_clock::time_point start_time;
    
public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    template<typename Duration = std::chrono::milliseconds>
    typename Duration::rep elapsed() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<Duration>(end_time - start_time);
        return duration.count();
    }
};

template<typename Container>
void benchmarkSort(Container& container, const std::string& name) {
    Timer timer;
    timer.start();
    
    std::sort(container.begin(), container.end());
    
    auto elapsed = timer.elapsed<std::chrono::milliseconds>();
    std::cout << name << " sort took: " << elapsed << " ms\n";
}

void demonstrateRealWorldUsage() {
    std::cout << "Real-world duration usage:\n\n";
    
    // Benchmarking different container types
    const size_t size = 100000;
    std::vector<int> vec(size);
    std::list<int> lst;
    std::deque<int> deq(size);
    
    // Initialize with random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    
    for (auto& v : vec) v = dis(gen);
    for (size_t i = 0; i < size; ++i) lst.push_back(dis(gen));
    for (auto& d : deq) d = dis(gen);
    
    // Copy containers for benchmarking
    auto vec_copy = vec;
    auto lst_copy = lst;
    auto deq_copy = deq;
    
    // Benchmark sorting
    benchmarkSort(vec_copy, "Vector");
    benchmarkSort(deq_copy, "Deque");
    
    // List requires different approach
    Timer timer;
    timer.start();
    lst_copy.sort();
    auto list_time = timer.elapsed<std::chrono::milliseconds>();
    std::cout << "List sort took: " << list_time << " ms\n";
}

class RateLimiter {
    std::chrono::steady_clock::time_point last_call;
    std::chrono::milliseconds interval;
    bool first_call = true;
    
public:
    explicit RateLimiter(std::chrono::milliseconds ms) : interval(ms) {}
    
    bool shouldAllow() {
        auto now = std::chrono::steady_clock::now();
        
        if (first_call) {
            first_call = false;
            last_call = now;
            return true;
        }
        
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call);
        if (elapsed >= interval) {
            last_call = now;
            return true;
        }
        return false;
    }
};

void demonstrateRateLimiting() {
    std::cout << "\nRate limiting example:\n";
    RateLimiter limiter(std::chrono::milliseconds(100));
    
    for (int i = 0; i < 10; ++i) {
        if (limiter.shouldAllow()) {
            std::cout << "Call " << i << ": Allowed\n";
        } else {
            std::cout << "Call " << i << ": Blocked\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    demonstrateRealWorldUsage();
    demonstrateRateLimiting();
    return 0;
}
```

### 14.2 Time Points and Clocks

Time points represent specific moments in time, while clocks provide the mechanism to measure time and obtain current time points.

#### Understanding Clock Types

C++ provides three standard clocks, each with different characteristics and use cases:

1. **std::chrono::system_clock**: Wall clock time, can be adjusted
2. **std::chrono::steady_clock**: Monotonic clock, cannot be adjusted
3. **std::chrono::high_resolution_clock**: Highest available resolution

```c++
#include <chrono>
#include <iostream>
#include <thread>

void demonstrateClockTypes() {
    using namespace std::chrono;
    
    // System clock - wall time
    auto system_now = system_clock::now();
    auto system_time_t = system_clock::to_time_t(system_now);
    std::cout << "System clock (wall time): " << std::ctime(&system_time_t);
    
    // Steady clock - monotonic time
    auto steady_start = steady_clock::now();
    std::this_thread::sleep_for(milliseconds(100));
    auto steady_end = steady_clock::now();
    auto steady_duration = duration_cast<milliseconds>(steady_end - steady_start);
    std::cout << "Steady clock duration: " << steady_duration.count() << " ms\n";
    
    // High resolution clock
    auto high_res_start = high_resolution_clock::now();
    std::this_thread::sleep_for(milliseconds(50));
    auto high_res_end = high_resolution_clock::now();
    auto high_res_duration = duration_cast<nanoseconds>(high_res_end - high_res_start);
    std::cout << "High resolution duration: " << high_res_duration.count() << " ns\n";
}

void demonstrateTimePointOperations() {
    using namespace std::chrono;
    
    // Creating time points
    auto now = system_clock::now();
    auto future = now + hours(1);
    auto past = now - minutes(30);
    
    std::cout << "Time point operations:\n";
    std::cout << "Current time\n";
    std::cout << "One hour from now\n";
    std::cout << "30 minutes ago\n";
    
    // Time point arithmetic
    auto difference = future - past;
    std::cout << "Difference between future and past: " << duration_cast<hours>(difference).count() << " hours\n";
    
    // Comparing time points
    std::cout << "Future > Past: " << (future > past) << "\n";
    std::cout << "Future == Past: " << (future == past) << "\n";
}

void demonstrateTimePointConversions() {
    using namespace std::chrono;
    
    // Converting between time points of different clocks
    auto system_now = system_clock::now();
    auto steady_now = steady_clock::now();
    auto high_res_now = high_resolution_clock::now();
    
    std::cout << "\nTime point conversions:\n";
    std::cout << "Note: Direct conversion between different clock time points is not supported\n";
    std::cout << "You must work with durations or use time_t for system_clock\n";
    
    // Converting system_clock to time_t and back
    auto time_t_val = system_clock::to_time_t(system_now);
    auto from_time_t = system_clock::from_time_t(time_t_val);
    std::cout << "Round-trip conversion to time_t successful\n";
}

class Stopwatch {
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point pause_time;
    std::chrono::nanoseconds total_paused{0};
    bool running = false;
    bool paused = false;
    
public:
    void start() {
        if (!running) {
            start_time = std::chrono::steady_clock::now();
            total_paused = std::chrono::nanoseconds::zero();
            running = true;
            paused = false;
        }
    }
    
    void pause() {
        if (running && !paused) {
            pause_time = std::chrono::steady_clock::now();
            paused = true;
        }
    }
    
    void resume() {
        if (running && paused) {
            auto pause_duration = std::chrono::steady_clock::now() - pause_time;
            total_paused += std::chrono::duration_cast<std::chrono::nanoseconds>(pause_duration);
            paused = false;
        }
    }
    
    void stop() {
        if (running) {
            if (paused) {
                auto pause_duration = std::chrono::steady_clock::now() - pause_time;
                total_paused += std::chrono::duration_cast<std::chrono::nanoseconds>(pause_duration);
            }
            running = false;
            paused = false;
        }
    }
    
    template<typename Duration = std::chrono::milliseconds>
    typename Duration::rep elapsed() const {
        if (!running) {
            // Return final elapsed time
            auto total_duration = std::chrono::duration_cast<Duration>(
                pause_time - start_time - total_paused
            );
            return total_duration.count();
        }
        
        if (paused) {
            // Return elapsed time up to pause
            auto total_duration = std::chrono::duration_cast<Duration>(
                pause_time - start_time - total_paused
            );
            return total_duration.count();
        }
        
        // Return current elapsed time
        auto current_time = std::chrono::steady_clock::now();
        auto total_duration = std::chrono::duration_cast<Duration>(
            current_time - start_time - total_paused
        );
        return total_duration.count();
    }
};

void demonstrateStopwatch() {
    std::cout << "\nStopwatch demonstration:\n";
    Stopwatch stopwatch;
    
    stopwatch.start();
    std::cout << "Started stopwatch\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Elapsed: " << stopwatch.elapsed() << " ms\n";
    
    stopwatch.pause();
    std::cout << "Paused\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "Elapsed during pause: " << stopwatch.elapsed() << " ms\n";
    
    stopwatch.resume();
    std::cout << "Resumed\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std::cout << "Elapsed after resume: " << stopwatch.elapsed() << " ms\n";
    
    stopwatch.stop();
    std::cout << "Stopped. Final elapsed: " << stopwatch.elapsed() << " ms\n";
}

int main() {
    demonstrateClockTypes();
    demonstrateTimePointOperations();
    demonstrateTimePointConversions();
    demonstrateStopwatch();
    return 0;
}
```

#### Advanced Time Operations

```c++
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>

class TimeFormatter {
public:
    static std::string formatDuration(std::chrono::nanoseconds ns) {
        using namespace std::chrono;
        
        auto hours = duration_cast<hours>(ns);
        ns -= hours;
        auto minutes = duration_cast<minutes>(ns);
        ns -= minutes;
        auto seconds = duration_cast<seconds>(ns);
        ns -= seconds;
        auto milliseconds = duration_cast<milliseconds>(ns);
        ns -= milliseconds;
        auto microseconds = duration_cast<microseconds>(ns);
        ns -= microseconds;
        auto nanoseconds_remainder = ns;
        
        std::ostringstream oss;
        if (hours.count() > 0) {
            oss << hours.count() << "h ";
        }
        if (minutes.count() > 0 || hours.count() > 0) {
            oss << std::setfill('0') << std::setw(2) << minutes.count() << "m ";
        }
        oss << std::setfill('0') << std::setw(2) << seconds.count() << "s ";
        if (milliseconds.count() > 0 || microseconds.count() > 0 || nanoseconds_remainder.count() > 0) {
            oss << std::setfill('0') << std::setw(3) << milliseconds.count() << "ms ";
            oss << std::setfill('0') << std::setw(3) << microseconds.count() << "μs ";
            oss << std::setfill('0') << std::setw(3) << nanoseconds_remainder.count() << "ns";
        }
        
        return oss.str();
    }
    
    static std::string formatTimePoint(std::chrono::system_clock::time_point tp) {
        auto time_t_val = std::chrono::system_clock::to_time_t(tp);
        auto tm_val = *std::localtime(&time_t_val);
        
        std::ostringstream oss;
        oss << std::put_time(&tm_val, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

class PerformanceTimer {
    std::chrono::high_resolution_clock::time_point start_time;
    std::string name;
    bool active;
    
public:
    PerformanceTimer(const std::string& n) : name(n), active(true) {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    ~PerformanceTimer() {
        if (active) {
            stop();
        }
    }
    
    void stop() {
        if (active) {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            std::cout << name << ": " << TimeFormatter::formatDuration(duration) << "\n";
            active = false;
        }
    }
};

void demonstrateAdvancedTimeOperations() {
    std::cout << "\nAdvanced time operations:\n";
    
    // Performance timing with RAII
    {
        PerformanceTimer timer("Matrix multiplication");
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Complex duration formatting
    auto complex_duration = std::chrono::hours(2) + std::chrono::minutes(30) + 
                           std::chrono::seconds(45) + std::chrono::milliseconds(123) +
                           std::chrono::microseconds(456) + std::chrono::nanoseconds(789);
    std::cout << "Complex duration: " << TimeFormatter::formatDuration(complex_duration) << "\n";
    
    // Time point formatting
    auto now = std::chrono::system_clock::now();
    std::cout << "Current time: " << TimeFormatter::formatTimePoint(now) << "\n";
}

int main() {
    demonstrateClockTypes();
    demonstrateTimePointOperations();
    demonstrateTimePointConversions();
    demonstrateStopwatch();
    demonstrateAdvancedTimeOperations();
    return 0;
}
```

### 14.3 Practical Applications

The Chrono library is invaluable for performance measurement, timing operations, and working with time-sensitive applications.

#### Performance Measurement

```c++
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

class BenchmarkSuite {
    std::vector<std::pair<std::string, std::chrono::nanoseconds>> results;
    
public:
    template<typename Func>
    void benchmark(const std::string& name, Func&& func, int iterations = 1) {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto total_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        auto avg_duration = total_duration / iterations;
        
        results.emplace_back(name, avg_duration);
        std::cout << name << " (avg of " << iterations << " runs): " << 
                     TimeFormatter::formatDuration(avg_duration) << "\n";
    }
    
    void printResults() const {
        std::cout << "\nBenchmark Results:\n";
        std::cout << std::string(50, '=') << "\n";
        for (const auto& [name, duration] : results) {
            std::cout << std::left << std::setw(30) << name << ": " << 
                         TimeFormatter::formatDuration(duration) << "\n";
        }
    }
};

void demonstrateBenchmarking() {
    std::cout << "\nBenchmarking demonstration:\n";
    BenchmarkSuite suite;
    
    // Test vector operations
    std::vector<int> vec(100000);
    std::iota(vec.begin(), vec.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    
    suite.benchmark("Vector sort", [&vec, &g]() {
        auto test_vec = vec;
        std::shuffle(test_vec.begin(), test_vec.end(), g);
        std::sort(test_vec.begin(), test_vec.end());
    }, 10);
    
    suite.benchmark("Vector reverse", [&vec]() {
        auto test_vec = vec;
        std::reverse(test_vec.begin(), test_vec.end());
    }, 100);
    
    suite.benchmark("Vector find", [&vec, &g]() {
        auto test_vec = vec;
        std::shuffle(test_vec.begin(), test_vec.end(), g);
        auto it = std::find(test_vec.begin(), test_vec.end(), 50000);
    }, 1000);
    
    suite.printResults();
}

#### Timing Critical Operations

```c++
class TimeoutGuard {
    std::chrono::steady_clock::time_point deadline;
    bool expired;
    
public:
    template<typename Rep, typename Period>
    explicit TimeoutGuard(const std::chrono::duration<Rep, Period>& timeout)
        : deadline(std::chrono::steady_clock::now() + timeout), expired(false) {}
    
    bool check() {
        if (!expired && std::chrono::steady_clock::now() >= deadline) {
            expired = true;
        }
        return !expired;
    }
    
    bool hasExpired() const { return expired; }
};

void demonstrateTimeoutGuard() {
    std::cout << "\nTimeout guard demonstration:\n";
    TimeoutGuard guard(std::chrono::milliseconds(500));
    
    auto start = std::chrono::steady_clock::now();
    while (guard.check()) {
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start
        );
        std::cout << "Working... " << elapsed.count() << "ms elapsed\n";
    }
    std::cout << "Timeout expired!\n";
}

#### Rate Limiting

```c++
class RateLimiter {
    std::chrono::steady_clock::time_point last_call;
    std::chrono::milliseconds interval;
    std::mutex mutex;
    
public:
    explicit RateLimiter(std::chrono::milliseconds ms) : interval(ms) {
        last_call = std::chrono::steady_clock::now() - interval;
    }
    
    bool shouldAllow() {
        std::lock_guard<std::mutex> lock(mutex);
        auto now = std::chrono::steady_clock::now();
        if (now - last_call >= interval) {
            last_call = now;
            return true;
        }
        return false;
    }
    
    void waitForNext() {
        std::unique_lock<std::mutex> lock(mutex);
        auto now = std::chrono::steady_clock::now();
        auto time_until_next = interval - (now - last_call);
        if (time_until_next > std::chrono::steady_clock::duration::zero()) {
            lock.unlock();
            std::this_thread::sleep_for(time_until_next);
            lock.lock();
            last_call = std::chrono::steady_clock::now();
        } else {
            last_call = now;
        }
    }
};

void demonstrateRateLimiting() {
    std::cout << "\nRate limiting demonstration:\n";
    RateLimiter limiter(std::chrono::milliseconds(200));
    
    for (int i = 0; i < 10; ++i) {
        if (limiter.shouldAllow()) {
            std::cout << "Request " << i << " allowed\n";
        } else {
            std::cout << "Request " << i << " denied (rate limited)\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    std::cout << "\nUsing waitForNext():\n";
    for (int i = 0; i < 5; ++i) {
        limiter.waitForNext();
        std::cout << "Request " << i << " processed\n";
    }
}

int main() {
    demonstrateClockTypes();
    demonstrateTimePointOperations();
    demonstrateTimePointConversions();
    demonstrateStopwatch();
    demonstrateAdvancedTimeOperations();
    demonstrateBenchmarking();
    demonstrateTimeoutGuard();
    demonstrateRateLimiting();
    return 0;
}
```

### 14.4 Best Practices and Common Pitfalls

#### Best Practices

1. **Use the appropriate clock for your needs**:
   - `system_clock` for wall time and date operations
   - `steady_clock` for measuring intervals and timeouts
   - `high_resolution_clock` for performance measurement

2. **Prefer duration arithmetic over time point arithmetic**:
   ```c++
   // Good: Work with durations
   auto duration = end_time - start_time;
   auto new_time = start_time + std::chrono::hours(2);
   
   // Avoid: Complex time point arithmetic
   auto complex_time = start_time + std::chrono::hours(2) + std::chrono::minutes(30);
   ```

3. **Use type aliases for readability**:
   ```c++
   using namespace std::chrono_literals;
   using milliseconds = std::chrono::milliseconds;
   using seconds = std::chrono::seconds;
   ```

4. **Be careful with time zone conversions**:
   ```c++
   // system_clock represents UTC, not local time
   auto utc_time = std::chrono::system_clock::now();
   auto local_time = std::chrono::system_clock::to_time_t(utc_time);
   // Convert to local time using std::localtime if needed
   ```

#### Common Pitfalls

1. **Clock drift and adjustments**:
   ```c++
   // system_clock can be adjusted by the system, causing unexpected behavior
   auto start = std::chrono::system_clock::now();
   // ... long operation ...
   auto end = std::chrono::system_clock::now();
   // Duration might be negative if system time was adjusted backward!
   ```

2. **Precision loss in conversions**:
   ```c++
   // Converting from high precision to low precision loses information
   auto high_res = std::chrono::high_resolution_clock::now();
   auto low_res = std::chrono::system_clock::time_point(
       std::chrono::duration_cast<std::chrono::system_clock::duration>(high_res.time_since_epoch())
   );
   ```

3. **Thread safety**:
   ```c++
   // Clock operations are generally thread-safe, but be careful with shared state
   class UnsafeTimer {
       std::chrono::steady_clock::time_point start_time;
   public:
       void start() { start_time = std::chrono::steady_clock::now(); } // Not thread-safe!
   };
   ```

#### Performance Considerations

1. **Clock overhead**:
   ```c++
   // Getting the current time has some overhead
   // For very frequent measurements, consider caching
   auto cached_time = std::chrono::high_resolution_clock::now();
   // Use cached_time for multiple operations
   ```

2. **Duration precision**:
   ```c++
   // Higher precision durations use more memory and may be slower
   using nanoseconds = std::chrono::nanoseconds;  // High precision, more overhead
   using milliseconds = std::chrono::milliseconds; // Lower precision, less overhead
   ```

3. **Avoid unnecessary conversions**:
   ```c++
   // Inefficient: Multiple conversions
   auto start = std::chrono::high_resolution_clock::now();
   auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
       std::chrono::high_resolution_clock::now() - start
   );
   auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_ns);
   
   // Better: Cast directly to desired unit
   auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
       std::chrono::high_resolution_clock::now() - start
   );
   ```

### Summary

The Chrono library provides a comprehensive and type-safe way to work with time in C++. Key takeaways include:

1. **Three main components**: durations, time points, and clocks
2. **Three standard clocks**: system_clock, steady_clock, and high_resolution_clock
3. **Type safety**: Compile-time checking prevents many common time-related errors
4. **Flexibility**: Support for various time units and custom durations
5. **Performance**: High-resolution timing for accurate measurements

The Chrono library is essential for any application that needs to measure time intervals, implement timeouts, perform benchmarking, or work with time-sensitive operations. Its type-safe design and comprehensive feature set make it a powerful tool for modern C++ development.

### Exercises

1. **Basic Operations**: Create a program that measures the execution time of various sorting algorithms on arrays of different sizes.

2. **Custom Duration**: Implement a custom duration type that represents "work days" (Monday through Friday, excluding weekends).

3. **Time Zone Converter**: Create a class that can convert between different time zones using the Chrono library.

4. **Performance Monitor**: Implement a performance monitoring system that tracks function execution times and provides statistical analysis.

5. **Scheduler**: Create a task scheduler that can execute functions at specified intervals or at specific times.

6. **Timeout System**: Implement a comprehensive timeout system that can handle multiple concurrent timeouts with different durations.

7. **Rate Limiter**: Create an advanced rate limiter that supports different limits for different operations and can dynamically adjust limits based on system load.

The Chrono library's combination of type safety, flexibility, and performance makes it an indispensable tool for modern C++ programming, especially in applications where precise time measurement and manipulation are critical.
