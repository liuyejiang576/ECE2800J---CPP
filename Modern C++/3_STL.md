## Chapter 1: Enhanced STL Containers

### 1.1 std::array - The Safe Fixed-Size Container

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

Recall that const members can only call other const members, so we need to provide const overloads for our safe array wrapper.

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

### 1.2 std::forward_list - The Lightweight Linked List

`std::forward_list` is a forward-only, singly-linked list introduced in C++11 that provides lower memory overhead compared to `std::list` (which is doubly-linked).

#### Understanding forward_list Design

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

### 1.3 Unordered Containers - Hash-Based Performance

C++11 introduced hash-based containers that provide average O(1) performance for insert, delete, and lookup operations. These containers are particularly useful when you need fast access and don't care about element ordering.

#### Understanding Hash Tables

When working with key-value data (such as student IDs mapped to exam scores, or usernames mapped to account information), common data structures have clear performance limits for lookups. A `std::vector` or array requires a full linear scan to find a key, with O(n) time complexity that slows down dramatically as your dataset grows. A sorted `std::map` (built on a red-black tree) improves this to O(log n) lookup time, but still requires multiple comparison steps for every operation. Hash tables solve this problem: they deliver average O(1) time complexity for insert, lookup, and delete operations.

The core idea of a hash table is simple: map a unique key directly to a fixed storage location, so you never need to scan through unrelated data to find what you need. You can think of a hash table like a bank of numbered lockers: your key is a unique student ID, the hash function is a fixed rule that tells you exactly which locker number your data is stored in, and each locker is a storage slot called a bucket.

There are four foundational components: a unique key that identifies your data, a hash function that converts the key into an integer array index, a bucket that acts as the storage slot for that index, and the value that holds the data you want to store and retrieve.

A hash collision occurs when two different keys are converted to the same bucket index by the hash function. This is unavoidable, because there are infinitely many possible keys but only a fixed number of buckets. The standard resolution method used in C++’s official hash containers is separate chaining: instead of holding a single key-value pair, each bucket stores a linked list (or dynamic array) of all key-value pairs that map to that bucket. When a collision happens, the new pair is simply added to the list inside the shared bucket. For lookups, you go directly to the correct bucket, then only traverse the short list inside that bucket to find your target key, rather than scanning the entire dataset. 

The bucket structure:

```cpp
#include <vector>
#include <string>
#include <utility> // for std::pair

// Underlying storage: an array (vector) of buckets
std::vector<std::vector<std::pair<std::string, int>>> buckets;
// Initialize 10 empty buckets on hash table creation
buckets.resize(10);
```

#### Load Factor & Automatic Rehashing

$$ \text{load factor} = \frac{\text{total number of stored elements}}{\text{total number of buckets}} $$

It measures how "crowded" the hash table is. A load factor that is too high means longer lists inside buckets, which degrades lookup speed from O(1) to O(n). A load factor that is too low wastes memory on large numbers of empty buckets. C++’s standard hash containers have a default `max_load_factor` of 1.0. When the current load factor exceeds this threshold, the container automatically triggers rehashing: it creates a new, larger array of buckets (typically doubling the bucket count), recalculates the bucket index for every existing element using the new bucket count, and moves all elements to their new buckets. 

#### Minimal Implementation for Demonstration

```cpp
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <functional> // for std::hash

class SimpleHashTable {
private:
    // Underlying storage: vector of buckets, each bucket holds colliding key-value pairs
    std::vector<std::vector<std::pair<std::string, int>>> buckets;
    int totalBuckets;
    // Match C++ standard default max load factor
    const double MAX_LOAD_FACTOR = 1.0;
    int elementCount = 0;

    // Hash function: converts a string key to a valid bucket index
    int hashFunction(const std::string& key) const {
        std::hash<std::string> hasher;
        // Mod by bucket count to ensure index is within the bounds of the bucket array
        return static_cast<int>(hasher(key) % totalBuckets);
    }

    // Rehash: resize bucket array and re-map all elements when load factor is too high
    void rehash() {
        // Save all existing data before resizing
        std::vector<std::vector<std::pair<std::string, int>>> oldBuckets = std::move(buckets);
        // Double bucket count to reduce load factor
        totalBuckets *= 2;
        // Reset bucket array with new size
        buckets.clear();
        buckets.resize(totalBuckets);
        elementCount = 0;

        // Re-insert all existing elements into the new bucket array
        for (const auto& bucket : oldBuckets) {
            for (const auto& keyValuePair : bucket) {
                put(keyValuePair.first, keyValuePair.second);
            }
        }
    }

public:
    // Constructor: initialize hash table with a default bucket count
    SimpleHashTable(int initialBucketCount = 10) : totalBuckets(initialBucketCount) {
        buckets.resize(totalBuckets);
    }

    // Insert a new key-value pair, or update the value if the key already exists
    void put(const std::string& key, int value) {
        // Trigger rehash before insertion if load factor exceeds the threshold
        if (static_cast<double>(elementCount) / totalBuckets >= MAX_LOAD_FACTOR) {
            rehash();
        }

        // Get the bucket index for the input key
        int targetBucketIndex = hashFunction(key);
        auto& targetBucket = buckets[targetBucketIndex];

        // Check if the key already exists in the bucket: update value if found
        for (auto& pair : targetBucket) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }

        // Key does not exist: add new pair to the bucket
        targetBucket.emplace_back(key, value);
        elementCount++;
    }

    // Look up a value by key; returns -1 if the key is not found
    int get(const std::string& key) const {
        int targetBucketIndex = hashFunction(key);
        const auto& targetBucket = buckets[targetBucketIndex];

        // Only search the target bucket, not the entire dataset
        for (const auto& pair : targetBucket) {
            if (pair.first == key) {
                return pair.second;
            }
        }

        // Return sentinel value if key is not found
        return -1;
    }

    // Remove a key-value pair; returns true if deletion succeeded, false if key not found
    bool remove(const std::string& key) {
        int targetBucketIndex = hashFunction(key);
        auto& targetBucket = buckets[targetBucketIndex];

        // Iterate through the bucket to find and erase the target key
        for (auto it = targetBucket.begin(); it != targetBucket.end(); ++it) {
            if (it->first == key) {
                targetBucket.erase(it);
                elementCount--;
                return true;
            }
        }

        // Return false if key does not exist
        return false;
    }
};

// Test the hash table implementation
int main() {
    SimpleHashTable scoreTable;
    // Insert key-value pairs
    scoreTable.put("Alice", 92);
    scoreTable.put("Bob", 87);
    // Update value for an existing key
    scoreTable.put("Bob", 89);
    // Look up values by key
    std::cout << "Bob's score: " << scoreTable.get("Bob") << "\n";
    // Delete a key-value pair
    scoreTable.remove("Alice");
    std::cout << "Alice's score after deletion: " << scoreTable.get("Alice") << "\n";

    return 0;
}
```

Seems complicated? Fortunately, as C++ STL encapsulates the complexities of hash table implementation, developers can focus on solving problems rather than optimizing data structures. It provides ready-to-use hash table implementations in the form of `std::unordered_map` and `std::unordered_set`.

#### Practical Usage

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

You can provide a custom hash function for your type by specializing the `std::hash` template. If so, you need to provide a hash function which converts your type to a `std::size_t` hash value. You also need to implement the equality operator (`operator==`) to correctly compare two instances of your type for key matching.

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

### 1.4 std::tuple - Heterogeneous Collections

A heterogeneous collection means that the collection can hold elements of different data types. It is used in situations like returning multiple values from a function. It fundamentally supports generic programming, which stands for programming that works with different data types. It is implemented by `std::tuple`, which is a fixed-size collection of heterogeneous values, essentially a generalization of `std::pair`. 

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

#### Structured Bindings

Structured bindings (introduced in C++17) make working with tuples much more convenient by allowing you to unpack tuple elements directly into variables. This can make your code more readable and concise.

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

Tuples are particularly powerful when you need to work with different data types together, such as returning multiple values from a function or representing a collection of related values.

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

---

## Chapter 2: Type Traits: Compile-Time Type Inspection and Manipulation

Before C++11, writing generic, reusable C++ code meant struggling with undefined behavior, fragile runtime type checks, and impenetrable handwritten template hacks. If you wanted a function to behave differently for integers vs. floating-point numbers, or to enforce that a template only accepts class types, you had to rely on non-standard, error-prone workarounds with no compiler consistency.

C++11 introduced the `<type_traits>` header: a standardized collection of compile-time utilities that let you inspect, query, transform, and constrain types with **zero runtime overhead**. All type traits operate entirely at compile time: their results are resolved before your program even runs, so they add no performance cost to your final executable. They fall into four core categories:
1.  **Classification traits**: Check if a type has a specific property (e.g., is it an integer? a pointer? a class?)
2.  **Transformation traits**: Modify a type to create a new related type (e.g., remove const, add a reference)
3.  **Conditional traits**: Select between types or enable code based on compile-time conditions
4.  **Relationship traits**: Check if two types are compatible (e.g., are they the same? is one convertible to the other?)

---

## 2.1 Type Classification Traits: Check Type Properties at Compile Time

When writing template code, you often need to verify that a type meets specific requirements, or adjust behavior based on the type's core properties. For example, a serialization function might need to handle fundamental types (int, float) differently than custom class types, or a math function might only accept numeric inputs.

Type classification traits solve this by providing a compile-time boolean `value` member that confirms if the type has the property you're checking. C++14 adds the `_v` alias (e.g., `std::is_integral_v<T>`) to simplify usage, eliminating the need for verbose `std::is_integral<T>::value` syntax.

```c++
#include <type_traits>
#include <iostream>
#include <string>
#include <iomanip>

// 1. Compile-time type enforcement with static_assert
// Fails to compile if T is not an integral type, with a human-readable error
template<typename T>
void onlyAcceptsIntegers(T value) {
    static_assert(std::is_integral_v<T>, "This function only accepts integer types!");
    std::cout << "Integer value: " << value << "\n";
}

// 2. Adaptive behavior with if constexpr (C++17)
// Single function that behaves correctly for multiple type categories
template<typename T>
void smartPrint(const T& value) {
    std::cout << std::boolalpha << std::fixed;
    // For pointer types: print the pointed-to value, not the memory address
    if constexpr (std::is_pointer_v<T>) {
        std::cout << "Pointer to value: " << *value << "\n";
    }
    // For floating-point types: print with fixed 2-digit precision
    else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Floating-point value: " << std::setprecision(2) << value << "\n";
    }
    // For integral types: print as standard decimal
    else if constexpr (std::is_integral_v<T>) {
        std::cout << "Integral value: " << value << "\n";
    }
    // For all other types: use default operator<<
    else {
        std::cout << "Generic value: " << value << "\n";
    }
}

int main() {
    std::cout << std::boolalpha;

    // Basic type classification checks
    std::cout << "int is integral? " << std::is_integral_v<int> << "\n"; // true
    std::cout << "double is floating-point? " << std::is_floating_point_v<double> << "\n"; // true
    std::cout << "int* is pointer? " << std::is_pointer_v<int*> << "\n"; // true
    std::cout << "const int is const? " << std::is_const_v<const int> << "\n"; // true
    std::cout << "std::string is class? " << std::is_class_v<std::string> << "\n"; // true
    std::cout << "int and long are the same type? " << std::is_same_v<int, long> << "\n"; // false

    // Static assert enforcement
    onlyAcceptsIntegers(42); // Compiles and runs
    // onlyAcceptsIntegers(3.14); // Fails to compile: static_assert triggered

    // Adaptive smartPrint function
    int x = 10;
    smartPrint(&x); // Pointer type: prints pointed-to value
    smartPrint(3.14159); // Floating-point: fixed precision
    smartPrint(100); // Integral: standard print
    smartPrint(std::string("Hello Type Traits!")); // Generic: default print

    return 0;
}
```

| Trait | What It Checks | Common Use Case |
|-------|----------------|-----------------|
| `std::is_integral_v<T>` | Is T an integer type (int, char, long, etc.)? | Enforce numeric input, optimize integer operations |
| `std::is_floating_point_v<T>` | Is T a floating-point type (float, double, long double)? | Handle precision for math operations |
| `std::is_pointer_v<T>` | Is T a raw pointer type? | Dereference safely, avoid printing memory addresses |
| `std::is_reference_v<T>` | Is T an lvalue or rvalue reference? | Perfect forwarding, avoid copying large objects |
| `std::is_const_v<T>` | Is T a const-qualified type? | Enforce read-only behavior, overload for const/non-const |
| `std::is_class_v<T>` | Is T a class or struct type? | Handle custom user types vs. fundamental types |
| `std::is_same_v<T, U>` | Are T and U exactly the same type? | Enforce type matching, debug template code |
| `std::is_convertible_v<T, U>` | Can T be implicitly converted to U? | Validate type compatibility for assignments |
| `std::is_arithmetic_v<T>` | Is T an integral or floating-point type? | Restrict functions to numeric types |

### Advantages
1.  **Compile-Time Safety**: Catch invalid type usage before your program runs, with no runtime cost
2.  **Adaptive Code**: Write a single template function that behaves correctly for multiple type categories
3.  **Self-Documenting Code**: Explicitly state your type requirements directly in the code
4.  **No Runtime Overhead**: All checks are resolved at compile time, with zero impact on executable performance

### Practical Example: Type-Safe Numeric Input Validator
```c++
#include <type_traits>
#include <stdexcept>
#include <string>
#include <limits>

template<typename T>
class NumericValidator {
    // Enforce that T is a numeric type at compile time
    static_assert(std::is_arithmetic_v<T>, "Validator only works for numeric types!");
public:
    // Validate that a value fits within the valid range of T
    static bool isInRange(double value) {
        constexpr T min = std::numeric_limits<T>::min();
        constexpr T max = std::numeric_limits<T>::max();
        return value >= static_cast<double>(min) && value <= static_cast<double>(max);
    }

    // Safely convert a string to T, with type-specific validation
    static T safeConvert(const std::string& input) {
        double rawValue = std::stod(input);

        if (!isInRange(rawValue)) {
            throw std::out_of_range("Input value is outside the valid range for the target type");
        }

        // For unsigned types: reject negative values
        if constexpr (std::is_unsigned_v<T>) {
            if (rawValue < 0) {
                throw std::invalid_argument("Unsigned type cannot accept negative values");
            }
        }

        return static_cast<T>(rawValue);
    }
};

int main() {
    // Valid conversions
    int validInt = NumericValidator<int>::safeConvert("42");
    unsigned int validUInt = NumericValidator<unsigned int>::safeConvert("100");
    double validDouble = NumericValidator<double>::safeConvert("3.14159");

    // Invalid conversions (throw exceptions)
    // NumericValidator<unsigned int>::safeConvert("-50"); // Fails: negative for unsigned
    // NumericValidator<int>::safeConvert("1000000000000"); // Fails: out of int range
    // NumericValidator<std::string> invalid; // Fails: static_assert triggered (not arithmetic)

    return 0;
}
```

## 2.2 Type Transformation Traits: Modify Types at Compile Time

When writing generic code, you often need to create a new type based on an existing one. For example, you might need to remove the const qualifier from a type to create a mutable copy, add a reference to avoid copying large objects, or strip away all qualifiers to get the "raw" underlying type.

Type transformation traits solve this by exposing a nested `type` member that is the modified version of the input type. C++14 adds the `_t` alias (e.g., `std::remove_const_t<T>`) to simplify usage, eliminating the need for verbose `typename std::remove_const<T>::type` syntax.

```c++
#include <type_traits>
#include <iostream>
#include <string>

// 1. Basic const/volatile modification
template<typename T>
void removeConstExample(const T& value) {
    // Remove const from T to get a mutable type
    using MutableT = std::remove_const_t<T>;
    MutableT copy = value; // Safe to modify the copy
    copy += 10;
    std::cout << "Original value: " << value << ", Modified copy: " << copy << "\n";
}

// 2. Reference manipulation
template<typename T>
void efficientPass(T&& value) {
    // Remove reference to get the base type, then add lvalue reference for safe binding
    using BaseT = std::remove_reference_t<T>;
    using RefT = std::add_lvalue_reference_t<BaseT>;
    RefT ref = value; // Bind to the value without copying
    std::cout << "Referenced value: " << ref << "\n";
}

// 3. Decay: strip away references, const/volatile, and array/function decay
// This replicates the type conversion that happens when you pass an array to a function
template<typename T>
void decayExample(T&& value) {
    // Get the "raw" underlying type, removing all qualifiers and references
    using RawT = std::decay_t<T>;
    std::cout << "Original type: " << typeid(T).name() << "\n";
    std::cout << "Decayed type: " << typeid(RawT).name() << "\n\n";
}

int main() {
    std::cout << std::boolalpha;

    // Basic remove_const example
    const int x = 42;
    removeConstExample(x);
    std::cout << "\n";

    // Reference manipulation
    int y = 100;
    efficientPass(y); // Pass lvalue
    efficientPass(200); // Pass rvalue
    std::cout << "\n";

    // Decay examples: see how types are simplified
    int arr[5] = {1,2,3,4,5};
    decayExample(arr); // int[5] decays to int*
    decayExample("Hello"); // const char[6] decays to const char*
    decayExample(3.14); // double&& decays to double
    decayExample(x); // const int& decays to int

    // Common transformation validation
    using ConstInt = const int;
    using NonConstInt = std::remove_const_t<ConstInt>;
    std::cout << "Const int -> non-const matches int? " << std::is_same_v<NonConstInt, int> << "\n"; // true

    using IntRef = int&;
    using IntNoRef = std::remove_reference_t<IntRef>;
    std::cout << "int& -> no reference matches int? " << std::is_same_v<IntNoRef, int> << "\n"; // true

    return 0;
}
```

| Trait | What It Does | Common Use Case |
|-------|--------------|-----------------|
| `std::remove_const_t<T>` | Removes const qualifier from T | Create mutable copies of const-qualified types |
| `std::add_const_t<T>` | Adds const qualifier to T | Enforce read-only access in generic code |
| `std::remove_reference_t<T>` | Removes lvalue/rvalue references from T | Get the underlying type of a forwarded value |
| `std::add_lvalue_reference_t<T>` | Adds an lvalue reference to T | Enable efficient pass-by-reference in templates |
| `std::decay_t<T>` | Strips references, const/volatile, and decays arrays/functions to pointers | Get the "raw" type for storage or comparison |
| `std::make_signed_t<T>` | Converts an integral type to its signed equivalent | Handle signed/unsigned conversions safely |
| `std::make_unsigned_t<T>` | Converts an integral type to its unsigned equivalent | Enforce non-negative numeric values |
| `std::remove_pointer_t<T>` | Removes the pointer qualifier from T | Get the type pointed to by a raw pointer |

### Advantages
1.  **Consistent Type Handling**: Standardized, compiler-agnostic way to modify types, no more fragile handwritten hacks
2.  **Efficient Code**: Avoid unnecessary copies by adding/removing references as needed
3.  **Simplified Generic Code**: Write code that works with any type, regardless of qualifiers or references
4.  **Predictable Results**: Well-defined behavior for edge cases (e.g., reference collapsing, nested qualifiers)

### Practical Example: Generic Deep Copy Wrapper
```c++
#include <type_traits>
#include <iostream>
#include <cstring>

// Generic wrapper that creates a deep copy of any input, regardless of type qualifiers
template<typename T>
class DeepCopy {
    // Get the raw, underlying type (remove references, const, volatile)
    using ValueT = std::decay_t<T>;
    ValueT* m_data;
public:
    // Constructor: take any input, decay to the raw type, and create a deep copy
    explicit DeepCopy(const T& input) {
        m_data = new ValueT(input);
    }

    // Specialization for array types: deep copy the entire array
    template<typename U, size_t N>
    explicit DeepCopy(U (&input)[N]) {
        m_data = new ValueT[N];
        if constexpr (std::is_trivially_copyable_v<ValueT>) {
            // For trivially copyable types: use memcpy for efficiency
            std::memcpy(m_data, input, N * sizeof(ValueT));
        } else {
            // For non-trivial types: copy each element individually
            for (size_t i = 0; i < N; ++i) {
                m_data[i] = input[i];
            }
        }
    }

    // Access the underlying data
    ValueT& get() { return *m_data; }
    const ValueT& get() const { return *m_data; }

    // Clean up the deep copy
    ~DeepCopy() {
        // For array types: use delete[]
        if constexpr (std::is_array_v<T>) {
            delete[] m_data;
        } else {
            delete m_data;
        }
    }
};

int main() {
    // Deep copy a const int
    const int x = 42;
    DeepCopy<int> copyX(x);
    copyX.get() = 100; // Modify the copy, original remains unchanged
    std::cout << "Original x: " << x << ", Copied x: " << copyX.get() << "\n";

    // Deep copy an array
    int arr[5] = {1, 2, 3, 4, 5};
    DeepCopy<int[5]> copyArr(arr);
    copyArr.get()[0] = 99;
    std::cout << "Original arr[0]: " << arr[0] << ", Copied arr[0]: " << copyArr.get()[0] << "\n";

    return 0;
}
```

## 2.3 Conditional Type Selection: Compile-Time Type Decisions

Sometimes you need to select between two different types based on a compile-time boolean condition. For example, you might want to use a small, fast type for small numeric values and a larger type for big values, or choose between a stack-allocated array and a dynamic vector based on the maximum expected data size.

`std::conditional` solves this: it is the compile-time equivalent of an if-else statement for types. It takes three parameters: a boolean condition, a type to use if the condition is true, and a type to use if the condition is false. It exposes a `type` member (with `_t` alias in C++14) for the selected type.

```c++
#include <type_traits>
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <cstdint>

// 1. Basic conditional type selection
// Select 32-bit integer if size <= 4 bytes, 64-bit otherwise
template<size_t Size>
using IntegerType = std::conditional_t<Size <= 4, int32_t, int64_t>;

// 2. Optimized container selection: stack-allocated for small sizes, dynamic for large
template<typename T, size_t MaxSize>
using OptimizedContainer = std::conditional_t<
    MaxSize <= 100, // Condition: is the maximum size small?
    std::array<T, MaxSize>, // True: use stack-allocated std::array (no dynamic allocation)
    std::vector<T> // False: use dynamic std::vector (flexible size)
>;

// 3. Select between const and non-const type based on a compile-time flag
template<bool IsConst>
using StringType = std::conditional_t<IsConst, const std::string, std::string>;

int main() {
    std::cout << std::boolalpha;

    // Basic integer type selection
    IntegerType<4> smallInt = 2147483647; // Max 32-bit int
    IntegerType<8> largeInt = 9223372036854775807LL; // Max 64-bit int
    std::cout << "Small int size: " << sizeof(smallInt) << " bytes\n"; // 4
    std::cout << "Large int size: " << sizeof(largeInt) << " bytes\n"; // 8
    std::cout << "\n";

    // Optimized container selection
    OptimizedContainer<int, 50> smallContainer; // Stack-allocated std::array
    OptimizedContainer<int, 200> largeContainer; // Dynamic std::vector
    std::cout << "Small container is std::array? " << std::is_same_v<decltype(smallContainer), std::array<int, 50>> << "\n"; // true
    std::cout << "Large container is std::vector? " << std::is_same_v<decltype(largeContainer), std::vector<int>> << "\n"; // true
    std::cout << "\n";

    // Const/non-const string selection
    StringType<true> constString = "Read-only string";
    StringType<false> mutableString = "Modifiable string";
    mutableString += " (modified)";
    std::cout << "Mutable string: " << mutableString << "\n";
    // constString += " (modified)"; // Fails to compile: const-qualified

    return 0;
}
```

### Advantages
1.  **Compile-Time Optimization**: Choose the most efficient type for your use case before the program runs
2.  **Single Interface**: Write a single template that uses the optimal type for different conditions
3.  **Reduced Code Duplication**: Avoid writing separate functions/classes for each type variant
4.  **Zero Runtime Overhead**: All type selection is resolved at compile time, no performance cost

### Practical Example: Optimized Integer Wrapper
```c++
#include <type_traits>
#include <iostream>
#include <limits>
#include <cstdint>

// Wrapper that selects the smallest possible unsigned integer type that can hold a given maximum value
template<uint64_t MaxValue>
class OptimizedInt {
    // Nested conditional: select the smallest type that fits MaxValue
    using ValueT = std::conditional_t<
        MaxValue <= std::numeric_limits<uint8_t>::max(),
        uint8_t,
        std::conditional_t<
            MaxValue <= std::numeric_limits<uint16_t>::max(),
            uint16_t,
            std::conditional_t<
                MaxValue <= std::numeric_limits<uint32_t>::max(),
                uint32_t,
                uint64_t
            >
        >
    >;

    ValueT m_value;
public:
    explicit OptimizedInt(ValueT value) : m_value(value) {
        if (value > MaxValue) {
            throw std::out_of_range("Value exceeds maximum allowed for this wrapper");
        }
    }

    ValueT get() const { return m_value; }
    static constexpr size_t getTypeSize() { return sizeof(ValueT); }
};

int main() {
    // Wrapper for values up to 100: uses uint8_t (1 byte)
    OptimizedInt<100> smallInt(50);
    std::cout << "Small int value: " << static_cast<int>(smallInt.get()) << "\n";
    std::cout << "Small int type size: " << smallInt.getTypeSize() << " byte\n\n"; // 1

    // Wrapper for values up to 100,000: uses uint16_t (2 bytes)
    OptimizedInt<100000> mediumInt(50000);
    std::cout << "Medium int value: " << mediumInt.get() << "\n";
    std::cout << "Medium int type size: " << mediumInt.getTypeSize() << " bytes\n\n"; // 2

    // Wrapper for values up to 10 billion: uses uint32_t (4 bytes)
    OptimizedInt<10000000000> largeInt(5000000000);
    std::cout << "Large int value: " << largeInt.get() << "\n";
    std::cout << "Large int type size: " << largeInt.getTypeSize() << " bytes\n"; // 4

    return 0;
}
```

## 2.4 Relationship Traits: Control Overload Resolution at Compile Time

Relationship traits are a set of type traits that compare two types and return a boolean result. It uses SFINAE (**Substitution Failure Is Not An Error**), a core C++ rule that states: if substituting a template parameter into a function signature fails, the compiler does not throw a fatal error; it simply removes that function overload from the candidate list. 

Before C++11, using SFINAE required complex, unreadable template hacks. `std::enable_if` standardizes this pattern: it lets you enable or disable a function overload or template specialization based on a compile-time boolean condition. This is the most powerful application of type traits, letting you write overloads that only apply to specific type categories.

The logic is simple:
- If the boolean condition is true, `std::enable_if_t` is an alias for `void` (or a custom type you specify)
- If the condition is false, `std::enable_if_t` does not exist - substituting it into the function signature fails, and the overload is silently removed from the candidate list

```c++
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>

// 1. Basic enable_if: enable overloads only for specific type categories
// Enable this overload if T is an integral type
template<typename T>
std::enable_if_t<std::is_integral_v<T>> printType(T value) {
    std::cout << "Integral value: " << value << "\n";
}

// Enable this overload if T is floating-point
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>> printType(T value) {
    std::cout << "Floating-point value: " << std::fixed << value << "\n";
}

// Enable this overload if T is a std::string (decay removes references/const)
template<typename T>
std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>> printType(T&& value) {
    std::cout << "String value: " << value << "\n";
}

// 2. enable_if with custom return type: multiply function only for numeric types
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, T> multiply(T a, T b) {
    return a * b;
}

// 3. enable_if in template parameters: class template specialization
// Primary template: enabled for all non-class types
template<typename T, typename = std::enable_if_t<!std::is_class_v<T>>>
class TypeHandler {
public:
    void handle(const T& value) {
        std::cout << "Handling fundamental type: " << value << "\n";
    }
};

// Specialization: enabled only for class types
template<typename T>
class TypeHandler<T, std::enable_if_t<std::is_class_v<T>>> {
public:
    void handle(const T& value) {
        std::cout << "Handling class type: " << typeid(T).name() << "\n";
    }
};

int main() {
    // Overload resolution with enable_if
    printType(42); // Calls integral overload
    printType(3.14159); // Calls floating-point overload
    printType(std::string("Hello SFINAE!")); // Calls string overload
    std::cout << "\n";

    // Numeric-only multiply function
    std::cout << "3 * 4 = " << multiply(3, 4) << "\n";
    std::cout << "2.5 * 4.0 = " << multiply(2.5, 4.0) << "\n";
    // multiply(std::string("a"), std::string("b")); // Fails to compile: no matching overload
    std::cout << "\n";

    // Class template specialization with enable_if
    TypeHandler<int> fundamentalHandler;
    fundamentalHandler.handle(100); // Handles fundamental type

    TypeHandler<std::string> classHandler;
    classHandler.handle(std::string("Test")); // Handles class type

    return 0;
}
```

### if constexpr vs std::enable_if: When to Use Which
| Feature | `if constexpr` (C++17) | `std::enable_if` (C++11) |
|---------|--------------------------|-------------------|
| **Core Use Case** | Branching logic inside a single function template | Selecting between multiple function overloads or template specializations |
| **Scope** | Works inside the function body | Works in the function signature (return type, template parameters) |
| **Syntax** | Simple, readable, linear logic | More verbose, requires separate overloads |
| **Overload Control** | Does not affect overload resolution | Directly controls which overloads are visible to the compiler |

### Advantages
1.  **Precise Overload Control**: Explicitly define which types can use which function overloads
2.  **Type-Safe Generic Code**: Prevent invalid type usage at compile time, with clear error messages
3.  **Backward Compatibility**: Works with C++11 and later, unlike `if constexpr`
4.  **Customization Points**: Enable template specializations for entire categories of types

### Practical Example: Type-Safe Serialization Interface
```c++
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>

// Serialization interface that handles different type categories automatically
class Serializer {
public:
    // Serialize fundamental arithmetic types (int, float, etc.)
    template<typename T>
    std::enable_if_t<std::is_arithmetic_v<T>, std::string> serialize(const T& value) {
        return std::to_string(value);
    }

    // Serialize std::string
    template<typename T>
    std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>, std::string> serialize(const T& value) {
        return "\"" + value + "\"";
    }

    // Serialize vectors of serializable types
    template<typename T>
    std::enable_if_t<std::is_same_v<T, std::vector<typename T::value_type>>, std::string> serialize(const T& value) {
        std::string result = "[";
        for (size_t i = 0; i < value.size(); ++i) {
            if (i > 0) result += ", ";
            result += serialize(value[i]);
        }
        result += "]";
        return result;
    }
};

int main() {
    Serializer serializer;

    // Serialize different types
    std::cout << "Serialized int: " << serializer.serialize(42) << "\n";
    std::cout << "Serialized double: " << serializer.serialize(3.14159) << "\n";
    std::cout << "Serialized string: " << serializer.serialize(std::string("Hello World")) << "\n";

    // Serialize a vector of ints
    std::vector<int> intVec = {1, 2, 3, 4, 5};
    std::cout << "Serialized int vector: " << serializer.serialize(intVec) << "\n";

    // Serialize a vector of strings
    std::vector<std::string> strVec = {"a", "b", "c"};
    std::cout << "Serialized string vector: " << serializer.serialize(strVec) << "\n";

    return 0;
}
```

## 2.5 Remarks

### Performance Considerations
- **Zero Runtime Overhead**: All type trait operations are resolved at compile time. They add no instructions to your final executable and have no impact on runtime performance.
- **Compile Time Impact**: Overuse of deeply nested type traits (e.g., recursive `std::conditional` or dozens of SFINAE overloads) can increase compile time. For most use cases, this impact is negligible.
- **Standard Library Optimization**: The C++ standard library uses type traits extensively to optimize algorithms (e.g., using `memcpy` instead of element-wise copy for trivially copyable types).

### Best Practices
1.  **Use the `_v` and `_t` Aliases**: Prefer `std::is_integral_v<T>` over `std::is_integral<T>::value`, and `std::remove_const_t<T>` over `typename std::remove_const<T>::type` for cleaner, more readable code.
2.  **Prefer `if constexpr` for Internal Branching**: For logic inside a single function, `if constexpr` is simpler and more maintainable than multiple `enable_if` overloads (when C++17 is available).
3.  **Use `static_assert` for Clear Error Messages**: Enforce type requirements with `static_assert` to give users human-readable error messages, instead of relying on SFINAE's "no matching overload" messages.
4.  **Combine Traits for Complex Checks**: Use logical operators (`&&`, `||`, `!`) to combine multiple traits for complex conditions (e.g., `std::is_integral_v<T> && !std::is_const_v<T>`).
5.  **Use `std::decay_t` for Raw Type Comparison**: When comparing types, use `std::decay_t` to strip away references, const, and volatile qualifiers, so you compare the underlying type.

### Common Pitfalls to Avoid
1.  **Forgetting `typename` for Dependent Types**: When using the `::type` member of a trait in a template, you must use `typename` to tell the compiler it is a type (the `_t` alias eliminates this requirement).
2.  **Overlapping Overloads**: When using `enable_if`, ensure your overload conditions are mutually exclusive. If two overloads are valid for the same type, the compiler will throw an ambiguous overload error.
3.  **Ignoring Reference Collapsing**: When adding/removing references, be aware of C++ reference collapsing rules (e.g., `T& &` collapses to `T&`). Use `std::remove_reference_t` to avoid unexpected behavior.
4.  **Using `is_same_v` with Qualified Types**: `std::is_same_v<const int, int>` is false. Use `std::decay_t` or `std::remove_const_t` if you want to compare the underlying type.

---

## Chapter 3: Template Metaprogramming

The underlying implementation of every standard type trait, and the foundation of modern C++ generic programming, is **Template Metaprogramming (TMP)**.

TMP is C++'s most unique and powerful programming paradigm: it turns the C++ compiler itself into an execution engine. Unlike regular C++ code that runs at runtime, TMP code executes entirely during compilation, performing calculations, manipulating types, and generating optimized runtime code. All TMP logic is resolved before your program even runs, delivering **zero runtime overhead**, absolute type safety, and automatic code generation that would be impossible to write by hand.

To frame the difference simply:
| Regular C++ Runtime Code | Template Metaprogramming (TMP) |
|---------------------------|----------------------------------|
| Executes at program runtime | Executes during compilation |
| Operates on runtime values (variables, data) | Operates on types and compile-time constants |
| Uses loops/conditionals for control flow | Uses template specialization/recursion for control flow |
| Encapsulated in functions | Encapsulated in template structs/aliases |

## 3.1 Compile-Time Value Calculation
The simplest entry point to TMP is **compile-time constant calculation**: using templates to compute fixed values during compilation, so your runtime code uses the precomputed result with zero CPU cost.

This relies on **non-type template parameters**: template parameters that are compile-time constant values (integers, pointers, references, and more in C++20), rather than types. We combine these with **template recursion** for repeated computation, and **full template specialization** to define a base case (termination condition) for the recursion.

We first write a standard runtime factorial function, then its equivalent TMP implementation, to highlight the pattern:
```c++
#include <iostream>
#include <type_traits>

// Runtime factorial (runs when program executes)
int runtimeFactorial(int n) {
    if (n <= 1) return 1;
    return n * runtimeFactorial(n - 1);
}

// TMP Compile-Time factorial (runs when program compiles)
// Primary template: recursive case
template<unsigned int N>
struct CompileTimeFactorial {
    // Value is computed at compile time: N * factorial(N-1)
    static constexpr unsigned int value = N * CompileTimeFactorial<N - 1>::value;
};

// Full template specialization: base case (terminate recursion at N=0)
template<>
struct CompileTimeFactorial<0> {
    static constexpr unsigned int value = 1;
};

// C++14+ alias for cleaner usage (matches standard type trait style)
template<unsigned int N>
constexpr unsigned int CompileTimeFactorial_v = CompileTimeFactorial<N>::value;

int main() {
    // Runtime factorial: computed when program runs
    std::cout << "Runtime 5! = " << runtimeFactorial(5) << "\n";

    // TMP factorial: computed during compilation, runtime just prints the result
    std::cout << "Compile-Time 5! = " << CompileTimeFactorial_v<5> << "\n";

    // Verify it's truly compile-time: static_assert only passes with compile-time constants
    static_assert(CompileTimeFactorial_v<5> == 120, "5! should equal 120");
    static_assert(CompileTimeFactorial_v<0> == 1, "0! should equal 1");

    return 0;
}
```

This example demonstrates the building blocks of all TMP code:
1.  **Primary Template**: Defines the general recursive case for the computation
2.  **Full Specialization**: Defines the base case to terminate recursion
3.  **Compile-Time Constant**: The `value` member is a `constexpr` constant, resolved entirely at compile time
4.  **Zero Overhead**: The compiled program contains only the final value (120), with no trace of the recursive computation

### Modern Alternative: constexpr/consteval Functions
C++11 introduced `constexpr` functions, and C++20 added `consteval` (strict compile-time only functions), which simplify compile-time value calculation dramatically. They use familiar runtime syntax (if/else, loops) while still executing at compile time:
```c++
#include <iostream>

// C++11 constexpr: can run at compile time OR runtime
constexpr unsigned int constexprFactorial(unsigned int n) {
    return (n <= 1) ? 1 : n * constexprFactorial(n - 1);
}

// C++20 consteval: *must* run at compile time, cannot be called at runtime
consteval unsigned int constevalFactorial(unsigned int n) {
    // C++14+ allows loops in constexpr functions
    unsigned int result = 1;
    for (unsigned int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    // All computed at compile time
    static_assert(constexprFactorial(5) == 120);
    static_assert(constevalFactorial(5) == 120);

    std::cout << "constexpr 5! = " << constexprFactorial(5) << "\n";
    std::cout << "consteval 5! = " << constevalFactorial(5) << "\n";

    // constexpr can run at runtime (if given a runtime value)
    int runtimeN = 5;
    std::cout << "constexpr runtime 5! = " << constexprFactorial(runtimeN) << "\n";

    // consteval CANNOT run at runtime: this line will fail to compile
    // std::cout << constevalFactorial(runtimeN) << "\n";

    return 0;
}
```

| Use Case | Template TMP | constexpr/consteval Functions |
|----------|--------------|--------------------------------|
| Compile-time value calculation | Functional, but verbose | Preferred: simpler, more readable syntax |
| Type manipulation/inspection | Core use case: only TMP can operate on types | Not designed for type operations |
| Overload resolution/SFINAE | Required for controlling template instantiation | Cannot affect overload resolution |
| C++ version compatibility | Works in C++98 and later | constexpr requires C++11+, consteval C++20+ |

### 3.2 Type Metaprogramming: Build Custom Type Traits
Type metaprogramming is the heart of TMP: using templates to create, inspect, and transform types at compile time. Every standard type trait from Chapter 2 is implemented with this pattern, and we can build our own custom traits to fit our exact needs.

#### Example 1: Implement std::is_same from Scratch
The standard `std::is_same_v<T, U>` checks if two types are exactly identical. We can implement it with TMP in just a few lines:
```c++
#include <iostream>
#include <type_traits>

// Custom is_same implementation
// Primary template: default case (types are NOT the same)
template<typename T, typename U>
struct CustomIsSame {
    static constexpr bool value = false;
};

// Full specialization: only matches when T and U are exactly the same type
template<typename T>
struct CustomIsSame<T, T> {
    static constexpr bool value = true;
};

// C++14+ alias for cleaner usage
template<typename T, typename U>
constexpr bool CustomIsSame_v = CustomIsSame<T, U>::value;

int main() {
    std::cout << std::boolalpha;

    // Test our custom trait
    std::cout << "int vs int: " << CustomIsSame_v<int, int> << "\n"; // true
    std::cout << "int vs const int: " << CustomIsSame_v<int, const int> << "\n"; // false
    std::cout << "int vs long: " << CustomIsSame_v<int, long> << "\n"; // false
    std::cout << "std::string vs std::string: " << CustomIsSame_v<std::string, std::string> << "\n"; // true

    // Verify against the standard implementation
    static_assert(CustomIsSame_v<int, int> == std::is_same_v<int, int>);
    static_assert(CustomIsSame_v<int, float> == std::is_same_v<int, float>);

    return 0;
}
```

#### Example 2: Implement std::add_pointer from Scratch
We can also build type transformation traits, which take an input type and return a modified output type:
```c++
#include <iostream>
#include <type_traits>

// Custom add_pointer implementation
// Primary template: add a pointer to the input type T
template<typename T>
struct CustomAddPointer {
    using type = T*;
};

// C++14+ alias for cleaner usage
template<typename T>
using CustomAddPointer_t = typename CustomAddPointer<T>::type;

int main() {
    std::cout << std::boolalpha;

    // Test our custom transformation
    using IntPtr = CustomAddPointer_t<int>;
    static_assert(std::is_same_v<IntPtr, int*>);

    using ConstStringPtr = CustomAddPointer_t<const std::string>;
    static_assert(std::is_same_v<ConstStringPtr, const std::string*>);

    std::cout << "CustomAddPointer_t<int> matches int*: " << std::is_same_v<IntPtr, int*> << "\n";

    return 0;
}
```

#### Data Structure: Type List
In runtime code, we use arrays and containers to store collections of values. In type metaprogramming, we use a **Type List** to store collections of types. It is the foundational data structure for all advanced TMP operations.

A Type List is a simple variadic template struct, and we use TMP to implement all standard collection operations (get length, access by index, append, filter, etc.) at compile time:
```c++
#include <iostream>
#include <type_traits>

// Type List Definition: compile-time collection of types
template<typename... Ts>
struct TypeList {};

// Operation 1: Get the length of a Type List
// Primary template: default case
template<typename List>
struct Length;

// Partial specialization: match a TypeList with any number of types
template<typename... Ts>
struct Length<TypeList<Ts...>> {
    // sizeof...() returns the number of types in the parameter pack
    static constexpr size_t value = sizeof...(Ts);
};

template<typename List>
constexpr size_t Length_v = Length<List>::value;

// Operation 2: Get the Nth type in a Type List
// Primary template: recursive case (index > 0)
template<size_t N, typename List>
struct Get;

// Partial specialization: index 0 (base case: return the first type)
template<typename First, typename... Rest>
struct Get<0, TypeList<First, Rest...>> {
    using type = First;
};

// Partial specialization: index > 0 (recurse, decrement index, drop first type)
template<size_t N, typename First, typename... Rest>
struct Get<N, TypeList<First, Rest...>> {
    using type = typename Get<N - 1, TypeList<Rest...>>::type;
};

template<size_t N, typename List>
using Get_t = typename Get<N, List>::type;

// Operation 3: Append a type to the end of a Type List
template<typename List, typename NewType>
struct Append;

template<typename... Ts, typename NewType>
struct Append<TypeList<Ts...>, NewType> {
    using type = TypeList<Ts..., NewType>;
};

template<typename List, typename NewType>
using Append_t = typename Append<List, NewType>::type;

int main() {
    std::cout << std::boolalpha;

    // Define a Type List with 3 types
    using MyTypes = TypeList<int, double, std::string>;

    // Get length
    std::cout << "Type List length: " << Length_v<MyTypes> << "\n"; // 3
    static_assert(Length_v<MyTypes> == 3);

    // Get Nth type
    using SecondType = Get_t<1, MyTypes>;
    std::cout << "Index 1 type is double: " << std::is_same_v<SecondType, double> << "\n"; // true
    static_assert(std::is_same_v<SecondType, double>);

    // Append a type
    using ExtendedTypes = Append_t<MyTypes, float>;
    std::cout << "Extended list length: " << Length_v<ExtendedTypes> << "\n"; // 4
    static_assert(Length_v<ExtendedTypes> == 4);
    static_assert(std::is_same_v<Get_t<3, ExtendedTypes>, float>);

    return 0;
}
```

With this pattern, you can implement any collection operation for Type Lists: filtering types that match a condition, concatenating two Type Lists, reversing the order of types, and more. All operations execute at compile time, with zero runtime cost.

### 3.3 Compile-Time Control Flow: Branching and Loops
TMP uses different control flow patterns than runtime code. There are no runtime `if` or `for` statements in pure TMP—instead, we use template specialization, `if constexpr`, and SFINAE for branching, and template recursion for loops.

#### Compile-Time Branching
We cover three common branching patterns, ordered from simplest to most flexible:

##### 1. Template Specialization for Branching
The same pattern we used for type traits: define a primary template for the default case, and specializations for specific conditions. This is ideal for simple, fixed conditions:
```c++
#include <iostream>
#include <type_traits>
#include <array>
#include <vector>

// Primary template: default case (use dynamic std::vector)
template<size_t MaxSize>
struct ContainerSelector {
    using type = std::vector<int>;
};

// Partial specialization: if MaxSize <= 100, use stack-allocated std::array
template<size_t MaxSize>
requires (MaxSize <= 100) // C++20 constraint, equivalent to earlier enable_if
struct ContainerSelector<MaxSize> {
    using type = std::array<int, MaxSize>;
};

template<size_t MaxSize>
using ContainerSelector_t = typename ContainerSelector<MaxSize>::type;

int main() {
    // Small size: uses std::array (stack-allocated, no dynamic overhead)
    ContainerSelector_t<50> smallContainer;
    static_assert(std::is_same_v<decltype(smallContainer), std::array<int, 50>>);

    // Large size: uses std::vector (dynamic, flexible)
    ContainerSelector_t<200> largeContainer;
    static_assert(std::is_same_v<decltype(largeContainer), std::vector<int>>);

    std::cout << "Small container type: " << typeid(smallContainer).name() << "\n";
    std::cout << "Large container type: " << typeid(largeContainer).name() << "\n";

    return 0;
}
```

##### 2. std::conditional for Type Selection
From Chapter 2, `std::conditional_t` is the compile-time equivalent of an `if-else` statement for types. It is ideal for simple binary conditions without needing full template specializations:
```c++
#include <iostream>
#include <type_traits>
#include <cstdint>

// Select the smallest unsigned integer type that can hold MaxValue
template<uint64_t MaxValue>
using OptimalInt = std::conditional_t<
    MaxValue <= UINT8_MAX,
    uint8_t,
    std::conditional_t<
        MaxValue <= UINT16_MAX,
        uint16_t,
        std::conditional_t<
            MaxValue <= UINT32_MAX,
            uint32_t,
            uint64_t
        >
    >
>;

int main() {
    std::cout << "MaxValue 100 type size: " << sizeof(OptimalInt<100>) << " byte\n"; // 1 (uint8_t)
    std::cout << "MaxValue 100000 type size: " << sizeof(OptimalInt<100000>) << " bytes\n"; // 2 (uint16_t)
    std::cout << "MaxValue 10000000000 type size: " << sizeof(OptimalInt<10000000000>) << " bytes\n"; // 4 (uint32_t)

    static_assert(std::is_same_v<OptimalInt<100>, uint8_t>);
    static_assert(std::is_same_v<OptimalInt<100000>, uint16_t>);

    return 0;
}
```

##### 3. if constexpr for Inline Compile-Time Branching (C++17+)
`if constexpr` is the most readable and maintainable way to write compile-time branching inside a template function. It evaluates the condition at compile time, and only compiles the code block that matches the condition—unused blocks are discarded, and do not generate any code.

This eliminates the need for multiple SFINAE overloads for simple branching, and works seamlessly with type traits:
```c++
#include <iostream>
#include <type_traits>
#include <string>

// Generic print function with compile-time branching
template<typename T>
void smartPrint(const T& value) {
    // Compile-time branch: only compile this block if T is a pointer
    if constexpr (std::is_pointer_v<T>) {
        std::cout << "Pointer to value: " << *value << "\n";
    }
    // Compile-time branch: only compile this block if T is arithmetic
    else if constexpr (std::is_arithmetic_v<T>) {
        std::cout << "Numeric value: " << value << "\n";
    }
    // Compile-time branch: only compile this block if T is a std::string
    else if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
        std::cout << "String value: \"" << value << "\"\n";
    }
    // Fallback for all other types
    else {
        std::cout << "Generic value: " << typeid(T).name() << "\n";
    }
}

int main() {
    int x = 42;
    smartPrint(&x); // Uses pointer branch
    smartPrint(3.14); // Uses numeric branch
    smartPrint(std::string("Hello TMP")); // Uses string branch
    smartPrint(std::array<int, 5>{}); // Uses fallback branch

    return 0;
}
```

#### Compile-Time Loops with Template Recursion
In pure TMP, there is no `for` loop—instead, we use template recursion to iterate over a sequence of types or compile-time values. We define a base case with template specialization to terminate the recursion.

A common use case is iterating over a Type List to perform an operation on every type:
```c++
#include <iostream>
#include <type_traits>

// Type List definition
template<typename... Ts>
struct TypeList {};

// Compile-Time Loop: Sum the size of every type in a Type List
// Primary template: recursive case (process first type, recurse on rest)
template<typename List>
struct TotalSize;

// Base case: empty list (terminate recursion, sum is 0)
template<>
struct TotalSize<TypeList<>> {
    static constexpr size_t value = 0;
};

// Recursive case: add size of first type to sum of remaining types
template<typename First, typename... Rest>
struct TotalSize<TypeList<First, Rest...>> {
    static constexpr size_t value = sizeof(First) + TotalSize<TypeList<Rest...>>::value;
};

template<typename List>
constexpr size_t TotalSize_v = TotalSize<List>::value;

int main() {
    using MyTypes = TypeList<int, double, char, std::string>;

    // Sum of sizes: 4 (int) + 8 (double) + 1 (char) + 24 (std::string) = 37
    std::cout << "Total size of all types: " << TotalSize_v<MyTypes> << " bytes\n";
    static_assert(TotalSize_v<MyTypes> == 4 + 8 + 1 + sizeof(std::string));

    return 0;
}
```

### 3.4 Variadic Templates and Fold Expressions Recapped

We have covered this part in the last tutorial, Chapter 8. Here we will recap it in a more essential perspective of TMP.

C++11 introduced **variadic templates**: templates that accept an arbitrary number of template parameters (called a **parameter pack**). This is the foundation of modern TMP, eliminating the need for complex recursive hacks to handle multiple types/values. C++17 added **fold expressions**, which simplify parameter pack expansion to a single line of code.

#### Variadic Template Basics
A parameter pack can be a **type parameter pack** (accepts any number of types) or a **non-type parameter pack** (accepts any number of compile-time constants). We use the `...` syntax to declare and expand the pack:
```c++
#include <iostream>
#include <type_traits>

// Variadic Template: Type-Safe Print Function
// Base case: terminate recursion when no arguments remain
void variadicPrint() {
    std::cout << "\n";
}

// Recursive case: print the first argument, then recurse on the rest
template<typename First, typename... Rest>
void variadicPrint(const First& first, const Rest&... rest) {
    std::cout << first << " ";
    // Expand the parameter pack: pass the remaining arguments to the next call
    variadicPrint(rest...);
}

// Variadic Template: Compile-Time Sum of Values
template<typename... Ts>
constexpr auto variadicSum(const Ts&... values) {
    // C++17 fold expression: sum all values in the parameter pack
    return (values + ...);
}

int main() {
    // Variadic print: accepts any number of arguments, any type
    variadicPrint(1, 2.5, "hello", std::string("world"), true);

    // Variadic sum: compile-time sum of any number of values
    static_assert(variadicSum(1, 2, 3, 4, 5) == 15);
    static_assert(variadicSum(1.5, 2.5, 3.5) == 7.5);
    std::cout << "Sum of 1-5: " << variadicSum(1, 2, 3, 4, 5) << "\n";

    return 0;
}
```

#### Fold Expressions (C++17)
Fold expressions eliminate the need for recursive parameter pack expansion for common operations. They support four core forms:
| Form | Syntax | Behavior |
|------|--------|----------|
| Unary Right Fold | `(op ... pack)` | Expands to `pack1 op (pack2 op (pack3 op ...))` |
| Unary Left Fold | `(pack ... op)` | Expands to `(((... op pack1) op pack2) op pack3)` |
| Binary Right Fold | `(init op ... op pack)` | Expands to `init op (pack1 op (pack2 op ...))` |
| Binary Left Fold | `(pack ... op init)` | Expands to `(((... op pack1) op pack2) op pack3) op init` |

Common use cases for fold expressions include:
```c++
#include <iostream>
#include <type_traits>

// 1. Check if all types in a parameter pack are arithmetic
template<typename... Ts>
constexpr bool allArithmetic = (std::is_arithmetic_v<Ts> && ...);

// 2. Check if any type in a parameter pack is a pointer
template<typename... Ts>
constexpr bool anyPointer = (std::is_pointer_v<Ts> || ...);

// 3. Type-safe variadic print with fold expressions (no recursion needed!)
template<typename... Ts>
void foldPrint(const Ts&... args) {
    ((std::cout << args << " "), ...); // Unary left fold of the comma operator
    std::cout << "\n";
}

int main() {
    std::cout << std::boolalpha;

    // All arithmetic check
    std::cout << "int, double, char are all arithmetic: " << allArithmetic<int, double, char> << "\n"; // true
    std::cout << "int, std::string are all arithmetic: " << allArithmetic<int, std::string> << "\n"; // false

    // Any pointer check
    std::cout << "int, double*, char have any pointer: " << anyPointer<int, double*, char> << "\n"; // true
    std::cout << "int, double, char have any pointer: " << anyPointer<int, double, char> << "\n"; // false

    // Fold print
    foldPrint(1, 3.14, "fold", "expressions", "are", "awesome");

    return 0;
}
```

### 3.5 Practical Use Cases

#### Use Case 1: Member Detector - Check if a Type Has a Specific Method
A common TMP pattern is detecting if a type has a specific member function or variable at compile time. This lets you write generic code that adapts to the capabilities of the input type:
```c++
#include <iostream>
#include <type_traits>
#include <vector>
#include <array>

// TMP Member Detector: Check if type T has a .size() method
// Primary template: default case (no size() method)
template<typename T, typename = void>
struct HasSizeMethod : std::false_type {};

// Partial specialization: matches if T has a size() method
template<typename T>
struct HasSizeMethod<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

// C++14+ alias
template<typename T>
constexpr bool HasSizeMethod_v = HasSizeMethod<T>::value;

// Generic size function: uses .size() if available, else manual calculation
template<typename T>
size_t getSize(const T& container) {
    if constexpr (HasSizeMethod_v<T>) {
        // Compile-time branch: use .size() if available
        return container.size();
    } else {
        // Fallback: for C-style arrays, calculate size at compile time
        return sizeof(container) / sizeof(container[0]);
    }
}

int main() {
    std::cout << std::boolalpha;

    // Test the detector
    std::cout << "std::vector has size(): " << HasSizeMethod_v<std::vector<int>> << "\n"; // true
    std::cout << "std::array has size(): " << HasSizeMethod_v<std::array<int, 5>> << "\n"; // true
    std::cout << "C-style array has size(): " << HasSizeMethod_v<int[5]> << "\n"; // false
    std::cout << "int has size(): " << HasSizeMethod_v<int> << "\n"; // false

    // Test the generic size function
    std::vector<int> vec = {1,2,3,4,5};
    std::array<int, 5> arr = {1,2,3,4,5};
    int cArr[5] = {1,2,3,4,5};

    std::cout << "Vector size: " << getSize(vec) << "\n"; // 5
    std::cout << "Array size: " << getSize(arr) << "\n"; // 5
    std::cout << "C-array size: " << getSize(cArr) << "\n"; // 5

    return 0;
}
```

#### Use Case 2: Compile-Time Configuration System
TMP lets you build a zero-overhead configuration system that selects code paths and types at compile time, with no runtime branching or overhead:
```c++
#include <iostream>
#include <type_traits>
#include <array>
#include <vector>

// Compile-time configuration flags
constexpr bool ENABLE_DEBUG_LOGGING = false;
constexpr size_t MAX_BUFFER_SIZE = 256;

// TMP Configuration Selector
// Select buffer type based on MAX_BUFFER_SIZE
using BufferType = std::conditional_t<
    MAX_BUFFER_SIZE <= 1024,
    std::array<char, MAX_BUFFER_SIZE>, // Stack buffer for small sizes
    std::vector<char> // Heap buffer for large sizes
>;

// Logging function: compiled out entirely if debug logging is disabled
template<bool EnableDebug>
void debugLog(const std::string& message) {
    if constexpr (EnableDebug) {
        std::cout << "[DEBUG] " << message << "\n";
    }
    // If EnableDebug is false, this function compiles to nothing!
}

int main() {
    BufferType buffer;
    std::cout << "Buffer type size: " << sizeof(buffer) << " bytes\n";
    static_assert(std::is_same_v<BufferType, std::array<char, 256>>);

    // Debug log: compiled out entirely (no runtime cost)
    debugLog<ENABLE_DEBUG_LOGGING>("Program started");

    std::cout << "Program running\n";

    return 0;
}
```

#### Use Case 3: Type-Safe Generic Factory Pattern
TMP lets you build a type-safe factory that automatically registers and creates types, with compile-time validation that all types meet your requirements:
```c++
#include <iostream>
#include <type_traits>
#include <memory>
#include <unordered_map>
#include <string>

// Base class for all factory products
class BaseProduct {
public:
    virtual ~BaseProduct() = default;
    virtual void execute() const = 0;
};

// TMP Factory: Type-Safe Product Registration
class ProductFactory {
private:
    // Map of product names to creator functions
    std::unordered_map<std::string, std::unique_ptr<BaseProduct>(*)()> m_creators;

    // Singleton instance
    ProductFactory() = default;

public:
    // Get singleton instance
    static ProductFactory& getInstance() {
        static ProductFactory instance;
        return instance;
    }

    // Register a product type with the factory
    template<typename T>
    void registerProduct(const std::string& name) {
        // Compile-time validation: T must inherit from BaseProduct
        static_assert(std::is_base_of_v<BaseProduct, T>, "Product must inherit from BaseProduct");
        static_assert(std::is_default_constructible_v<T>, "Product must be default constructible");

        // Register creator function
        m_creators[name] = []() -> std::unique_ptr<BaseProduct> {
            return std::make_unique<T>();
        };
    }

    // Create a product by name
    std::unique_ptr<BaseProduct> create(const std::string& name) {
        auto it = m_creators.find(name);
        if (it == m_creators.end()) {
            return nullptr;
        }
        return it->second();
    }
};

// Auto-Registration Helper (TMP-powered)
template<typename T>
struct ProductRegistrar {
    ProductRegistrar(const std::string& name) {
        ProductFactory::getInstance().registerProduct<T>(name);
    }
};

// Example Products
class ProductA : public BaseProduct {
public:
    void execute() const override {
        std::cout << "Product A executed\n";
    }
};

class ProductB : public BaseProduct {
public:
    void execute() const override {
        std::cout << "Product B executed\n";
    }
};

// Auto-register products at program startup
const ProductRegistrar<ProductA> registrarA("ProductA");
const ProductRegistrar<ProductB> registrarB("ProductB");

int main() {
    // Create products from the factory
    auto productA = ProductFactory::getInstance().create("ProductA");
    auto productB = ProductFactory::getInstance().create("ProductB");

    if (productA) productA->execute();
    if (productB) productB->execute();

    // This line would fail to compile: int does not inherit from BaseProduct
    // ProductFactory::getInstance().registerProduct<int>("InvalidProduct");

    return 0;
}
```

### 3.6 Remarks

TMP is powerful, but it is not the right tool for every job.

Use TMP when:
- You need **zero runtime overhead** for calculations or type operations
- You need to enforce **compile-time type safety** and constraints
- You need to **automatically generate code** for multiple types
- You need to write **generic, reusable libraries** that adapt to any input type
- You need to perform **compile-time validation** of configuration or data

Avoid TMP when:
- A simple runtime function will solve the problem with acceptable performance
- The code will be maintained by developers unfamiliar with TMP
- Compile time is a critical constraint (complex TMP can increase compile time)

#### Best Practices
1.  **Prefer Modern C++ Features**: Use `if constexpr`, fold expressions, and `consteval` instead of complex template recursion and SFINAE whenever possible. They are more readable, maintainable, and less error-prone.
2.  **Encapsulate TMP Logic**: Hide complex TMP implementation details behind simple, well-documented interfaces (like standard type traits). Users should not need to understand the TMP internals to use your code.
3.  **Validate with static_assert**: Use `static_assert` to validate every TMP operation. It confirms your code works as expected at compile time, and provides clear error messages for users.
4.  **Limit Recursion Depth**: Compiler default template recursion depth is typically 1024. Avoid deep recursion, and use fold expressions or `constexpr` loops instead where possible.
5.  **Document Everything**: TMP code is inherently less readable than runtime code. Add detailed comments explaining what each template does, what its inputs/outputs are, and any edge cases.

#### Common Pitfalls
1.  **Template Specialization Matching Errors**: Partial template specialization has complex matching rules. A common mistake is writing a specialization that the compiler does not match, leading to the primary template being used instead. Always test specializations with `static_assert`.
2.  **Compile Time Explosion**: Overusing complex TMP (especially deep recursion, nested type lists, and heavy SFINAE) can drastically increase compile time. Profile your compile time and simplify TMP where needed.
3.  **Reference and CV Qualifier Ignorance**: Type traits and TMP logic often fail to account for `const`, `volatile`, and reference qualifiers. Use `std::decay_t` or `std::remove_cvref_t` (C++20) to strip qualifiers when comparing types.
4.  **Unintended Template Instantiation**: The compiler will instantiate every template used in your code, even if it is never executed. Use `if constexpr` to discard unused code blocks and prevent unnecessary instantiation.

#### Debugging TMP Code
TMP code runs at compile time, so you cannot use a runtime debugger. Use these techniques to debug TMP:
1.  **Use static_assert for Validation**: A `static_assert` will fail and print the type/value if your TMP logic is incorrect. For example: `static_assert(std::is_same_v<MyType, ExpectedType>, "Type mismatch");` will tell you exactly what `MyType` is when it fails.
2.  **Compiler Template Backtraces**: Use compiler flags to print template instantiation details:
    - GCC/Clang: `-ftemplate-backtrace-limit=0` to show full template recursion backtraces
    - MSVC: `/diagnostics:caret` for detailed template error messages
3.  **Split Complex Logic**: Break large TMP components into small, testable pieces. Validate each piece with `static_assert` before combining them.
4.  **Use Type Printing Helpers**: Write a simple helper template that will fail to compile and print the type it receives, to inspect the output of your TMP logic.

---

## Chapter 4: The Chrono Library

Before C++11, handling time in C++ was a fragile, error-prone mess. Developers relied on the C-style `<ctime>` library, which had:
- **No type safety**: Seconds, milliseconds, and system timestamps were all just `int` or `long` values, leading to accidental unit mismatches
- **No compile-time validation**: Unit conversion errors were only caught at runtime (if at all)
- **Limited precision**: No standard way to handle sub-millisecond or high-resolution timestamps
- **Platform-dependent behavior**: Timing functions behaved differently across Windows, Linux, and macOS

C++11 introduced the `<chrono>` library: a modern, TMP-powered time and date library that solves all these problems.

The `<chrono>` library is organized into three core, independent layers:
1.  **Durations**: Represent a span of time (e.g., 5 seconds, 100 milliseconds)
2.  **Time Points**: Represent a specific point in time (e.g., "now", "January 1, 2026, 00:00:00")
3.  **Clocks**: Connect durations and time points to the real world (e.g., system clock, steady clock)

### 4.1 Durations
A **duration** is a type-safe representation of a span of time. It is a template struct that combines two compile-time parameters:
1.  A **representation type** (`Rep`): The numeric type used to store the count of ticks (e.g., `int`, `long`, `double`)
2.  A **period type** (`Period`): A compile-time rational number representing the length of one tick, in seconds (e.g., `std::ratio<1>` for 1 second, `std::ratio<1, 1000>` for 1 millisecond)

This design enforces **absolute type safety**: a duration of 5 seconds (`std::chrono::seconds`) is a completely different type than a duration of 5 milliseconds (`std::chrono::milliseconds`), and the compiler will throw a clear error if you try to mix them incorrectly.

The standard library provides predefined type aliases for all common time units, from nanoseconds to hours:
```c++
#include <chrono>
#include <iostream>
#include <type_traits>

int main() {
    std::cout << std::boolalpha;

    // 1. Predefined duration aliases
    using namespace std::chrono_literals; // C++14: enables duration literals (e.g., 5s, 100ms)

    // Create durations using predefined aliases
    std::chrono::seconds fiveSeconds(5);
    std::chrono::milliseconds oneHundredMs(100);
    std::chrono::nanoseconds tenNs(10);

    // C++14: Create durations using convenient literals
    auto tenSeconds = 10s;
    auto twoFiftyMs = 250ms;
    auto oneMinute = 1min;
    auto halfHour = 0.5h; // Can use floating-point representation for fractional units

    // 2. Duration type inspection (TMP-powered!)
    // Check the representation type (Rep)
    static_assert(std::is_same_v<decltype(fiveSeconds)::rep, long long>);
    static_assert(std::is_same_v<decltype(halfHour)::rep, double>);

    // Check the period type (Period: compile-time rational number)
    // fiveSeconds has a period of 1/1 second
    static_assert(std::is_same_v<decltype(fiveSeconds)::period, std::ratio<1>>);
    // oneHundredMs has a period of 1/1000 second
    static_assert(std::is_same_v<decltype(oneHundredMs)::period, std::ratio<1, 1000>>);

    // 3. Get the tick count
    std::cout << "Five seconds tick count: " << fiveSeconds.count() << "\n"; // 5
    std::cout << "Two hundred fifty ms tick count: " << twoFiftyMs.count() << "\n"; // 250
    std::cout << "Half hour tick count: " << halfHour.count() << "\n"; // 0.5

    // 4. Type-safe arithmetic operations
    auto total = tenSeconds + twoFiftyMs; // Result is std::chrono::milliseconds (10250ms)
    auto difference = oneMinute - tenSeconds; // Result is std::chrono::seconds (50s)
    auto scaled = fiveSeconds * 3; // Result is std::chrono::seconds (15s)
    auto divided = oneMinute / 2; // Result is std::chrono::seconds (30s)

    std::cout << "10s + 250ms = " << total.count() << "ms\n";
    std::cout << "1min - 10s = " << difference.count() << "s\n";

    // 5. Type-safe comparisons
    std::cout << "10s > 5s? " << (tenSeconds > fiveSeconds) << "\n"; // true
    std::cout << "1min == 60s? " << (oneMinute == 60s) << "\n"; // true
    std::cout << "250ms < 0.5s? " << (twoFiftyMs < 0.5s) << "\n"; // true

    return 0;
}
```

### 4.2 Duration Conversions
One of the most powerful features of `<chrono>` durations is **automatic, safe unit conversion**. The library uses TMP to decide whether a conversion is safe (no precision loss) and can be done implicitly, or unsafe (possible precision loss) and requires an explicit cast.

The library uses `std::ratio` and type traits to enforce these rules at compile time:
| Conversion Direction | Implicit or Explicit | Reason |
|----------------------|----------------------|--------|
| **Coarser → Finer** (e.g., seconds → milliseconds) | **Implicit** | No precision loss: every tick in the coarser unit maps exactly to multiple ticks in the finer unit |
| **Finer → Coarser** (e.g., milliseconds → seconds) | **Explicit only** | Possible precision loss: fractional ticks in the coarser unit are truncated |
| **Floating-point → Any** | **Implicit** | Floating-point durations can represent fractional values, so no precision loss |
| **Any → Floating-point** | **Implicit** | Any integer duration can be represented exactly as a floating-point value |

```c++
#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono_literals;

    // 1. Safe implicit conversions (coarser → finer)
    std::chrono::seconds fiveSeconds(5);
    std::chrono::milliseconds fiveThousandMs = fiveSeconds; // Implicit: safe, no precision loss
    std::chrono::nanoseconds fiveBillionNs = fiveSeconds; // Implicit: safe

    std::cout << "5s = " << fiveThousandMs.count() << "ms\n"; // 5000
    std::cout << "5s = " << fiveBillionNs.count() << "ns\n\n"; // 5000000000

    // 2. Unsafe conversions require explicit cast (finer → coarser)
    std::chrono::milliseconds twelveHundredMs(1200);
    // std::chrono::seconds oneSecond = twelveHundredMs; // ERROR: implicit conversion not allowed!

    // Explicit cast with duration_cast: truncates fractional ticks
    std::chrono::seconds oneSecond = std::chrono::duration_cast<std::chrono::seconds>(twelveHundredMs);
    std::cout << "1200ms duration_cast to seconds: " << oneSecond.count() << "s\n"; // 1 (truncated)

    // C++17: floor, ceil, round for controlled rounding
    std::cout << "1200ms floor to seconds: " << std::chrono::floor<std::chrono::seconds>(twelveHundredMs).count() << "s\n"; // 1
    std::cout << "1200ms ceil to seconds: " << std::chrono::ceil<std::chrono::seconds>(twelveHundredMs).count() << "s\n"; // 2
    std::cout << "1200ms round to seconds: " << std::chrono::round<std::chrono::seconds>(twelveHundredMs).count() << "s\n\n"; // 1

    // 3. Floating-point duration conversions are always implicit
    std::chrono::duration<double, std::ratio<1>> twoPointFiveSeconds(2.5); // 2.5s as double
    std::chrono::milliseconds twoThousandFiveHundredMs = twoPointFiveSeconds; // Implicit: safe
    std::chrono::duration<double, std::ratio<60>> halfMinute = twoPointFiveSeconds; // Implicit: 2.5s = 0.041666...min

    std::cout << "2.5s = " << twoThousandFiveHundredMs.count() << "ms\n";
    std::cout << "2.5s = " << halfMinute.count() << "min\n";

    return 0;
}
```

#### Custom Duration Types
You can define your own custom duration types for specialized use cases, using the `std::chrono::duration` template directly:
```c++
#include <chrono>
#include <iostream>

// Custom duration: 1 "frame" = 1/60 second (for 60fps game timing)
using Frame = std::chrono::duration<int, std::ratio<1, 60>>;

// Custom duration: 1 "business_day" = 8 hours (for project scheduling)
using BusinessDay = std::chrono::duration<double, std::ratio<3600 * 8>>;

int main() {
    using namespace std::chrono_literals;

    // Use custom frame duration
    Frame oneFrame(1);
    Frame sixtyFrames(60);
    std::cout << "1 frame = " << std::chrono::duration_cast<std::chrono::milliseconds>(oneFrame).count() << "ms\n"; // ~16.666ms
    std::cout << "60 frames = " << std::chrono::duration_cast<std::chrono::seconds>(sixtyFrames).count() << "s\n\n"; // 1s

    // Use custom business day duration
    BusinessDay oneDay(1);
    BusinessDay halfDay(0.5);
    std::cout << "1 business day = " << std::chrono::duration_cast<std::chrono::hours>(oneDay).count() << "h\n"; // 8h
    std::cout << "0.5 business days = " << std::chrono::duration_cast<std::chrono::hours>(halfDay).count() << "h\n"; // 4h

    return 0;
}
```

### 4.3 Time Points
A **time point** is a type-safe representation of a specific moment in time. It is a template struct that combines two compile-time parameters:
1.  A **clock type** (`Clock`): The clock this time point is measured against (e.g., `std::chrono::system_clock`, `std::chrono::steady_clock`)
2.  A **duration type** (`Duration`): The duration type used to measure the time since the clock's epoch (its starting point, e.g., "January 1, 1970, 00:00:00 UTC" for the system clock)

Time points are designed to work seamlessly with durations: you can add/subtract durations to/from time points to get new time points, and subtract two time points to get the duration between them.

```c++
#include <chrono>
#include <iostream>
#include <ctime>

int main() {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    // 1. Get the current time from a clock
    // system_clock: The system-wide real-time clock (can be adjusted by the user/NTP)
    system_clock::time_point nowSystem = system_clock::now();

    // steady_clock: A monotonic clock that never goes backward (ideal for timing code)
    steady_clock::time_point nowSteady = steady_clock::now();

    // high_resolution_clock: The clock with the highest available precision (alias for system_clock or steady_clock)
    high_resolution_clock::time_point nowHighRes = high_resolution_clock::now();

    // 2. Time point arithmetic
    // Add a duration to a time point to get a future time
    system_clock::time_point oneHourFromNow = nowSystem + 1h;
    system_clock::time_point tenMinutesAgo = nowSystem - 10min;

    // Subtract two time points to get the duration between them
    // (Note: This is only meaningful if both time points use the same clock!)
    auto timeDifference = oneHourFromNow - tenMinutesAgo;
    std::cout << "Time between 10min ago and 1h from now: " 
              << duration_cast<minutes>(timeDifference).count() << "min\n\n"; // 70min

    // 3. Convert system_clock time points to human-readable time
    // Convert to time_t (C-style timestamp)
    std::time_t nowTimeT = system_clock::to_time_t(nowSystem);
    std::time_t oneHourFromNowTimeT = system_clock::to_time_t(oneHourFromNow);

    // Print human-readable time (C-style, for simplicity)
    std::cout << "Current system time: " << std::ctime(&nowTimeT);
    std::cout << "One hour from now: " << std::ctime(&oneHourFromNowTimeT);

    // 4. Time point comparisons
    std::cout << std::boolalpha;
    std::cout << "One hour from now > current time? " << (oneHourFromNow > nowSystem) << "\n"; // true
    std::cout << "Ten minutes ago < current time? " << (tenMinutesAgo < nowSystem) << "\n"; // true

    return 0;
}
```

### 4.4 Clocks
A **clock** is the bridge between the abstract world of durations and time points and the real world. It provides:
1.  A **now()** static method: Returns the current time as a time point
2.  An **epoch**: A fixed starting point in time (e.g., "Unix epoch": January 1, 1970, 00:00:00 UTC)
3.  A **duration type**: The default duration type used to measure time since the epoch
4.  A **is_steady** static boolean constant: Indicates if the clock is monotonic (never goes backward)

The C++ standard library provides three core clocks, each optimized for a specific use case:

| Clock | `is_steady` | Epoch | Best Use Case |
|-------|-------------|-------|---------------|
| `std::chrono::system_clock` | `false` | Unix epoch (1970-01-01 UTC) | Getting the current real-world time, converting to/from human-readable dates |
| `std::chrono::steady_clock` | `true` | Monotonic (no fixed epoch) | Timing code execution, measuring elapsed time, animations |
| `std::chrono::high_resolution_clock` | Implementation-defined | Implementation-defined | Short, high-precision timing measurements (use sparingly, prefer `steady_clock` for portability) |

The most common production use case for `<chrono>` is timing code execution to measure performance. **Always use `std::chrono::steady_clock` for this**: it is monotonic and never goes backward, even if the user adjusts the system clock or NTP updates the time.
```c++
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>

// Helper function to time any function call
template<typename Func, typename... Args>
auto timeFunction(Func&& func, Args&&... args) {
    // Record start time
    auto start = std::chrono::steady_clock::now();

    // Execute the function
    std::forward<Func>(func)(std::forward<Args>(args)...);

    // Record end time and calculate duration
    auto end = std::chrono::steady_clock::now();
    return end - start;
}

// Example function to time: sort a large vector
void sortLargeVector(size_t size) {
    std::vector<int> vec(size);
    // Fill vector with random numbers
    for (size_t i = 0; i < size; ++i) {
        vec[i] = static_cast<int>(i % 1000);
    }
    // Sort the vector
    std::sort(vec.begin(), vec.end());
}

int main() {
    using namespace std::chrono;

    // Time sorting a vector of 1,000,000 elements
    auto duration = timeFunction(sortLargeVector, 1000000);

    // Print the duration in multiple units
    std::cout << "Sorting 1,000,000 elements took:\n";
    std::cout << "  " << duration_cast<nanoseconds>(duration).count() << " ns\n";
    std::cout << "  " << duration_cast<microseconds>(duration).count() << " µs\n";
    std::cout << "  " << duration_cast<milliseconds>(duration).count() << " ms\n";
    std::cout << "  " << duration.count() << " steady clock ticks\n";

    // C++20: Print duration directly with std::format (or use operator<< in C++20)
    // std::cout << "  " << duration << "\n";

    return 0;
}
```

### 4.5 C++20 Chrono Extensions: Calendars and Time Zones
C++11-17 `<chrono>` provided excellent support for durations, time points, and clocks, but it lacked built-in support for **human-readable calendars** and **time zones**. C++20 filled this gap with a massive, TMP-powered extension to `<chrono>` that adds:
1.  **Calendar types**: `year`, `month`, `day`, `weekday`, `year_month`, `year_month_day`, and more for representing calendar dates
2.  **Time zone support**: `std::chrono::tzdb` (time zone database), `std::chrono::zoned_time` for representing time points in specific time zones
3.  **Seamless integration**: All new types work with the existing duration, time point, and clock types

```c++
#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // 1. Basic calendar types
    // Create individual calendar components
    year y{2026};
    month m{February};
    day d{27};
    weekday wd{Friday};

    // Check if components are valid
    std::cout << std::boolalpha;
    std::cout << "Year 2026 is valid? " << y.ok() << "\n"; // true
    std::cout << "Month February is valid? " << m.ok() << "\n"; // true
    std::cout << "Day 27 is valid? " << d.ok() << "\n"; // true
    std::cout << "Friday is valid? " << wd.ok() << "\n\n"; // true

    // 2. Combine components into full dates
    // year_month_day: Full calendar date
    year_month_day today = 2026y / February / 27;
    year_month_day nextMonth = 2026y / March / 1;
    year_month_day lastDayOfMonth = 2026y / February / last;

    std::cout << "Today: " << today << "\n";
    std::cout << "Next month: " << nextMonth << "\n";
    std::cout << "Last day of February 2026: " << lastDayOfMonth << "\n\n";

    // 3. Convert between calendar dates and system_clock time points
    // Convert year_month_day to system_clock::time_point
    system_clock::time_point todayTp = sys_days{today};
    // Convert system_clock::time_point to year_month_day
    year_month_day todayFromTp = floor<days>(system_clock::now());

    std::cout << "Today as system_clock time point: " << todayTp.time_since_epoch().count() << "s since epoch\n";
    std::cout << "system_clock::now() as calendar date: " << todayFromTp << "\n\n";

    // 4. Calendar arithmetic
    // Add/subtract days, months, years
    year_month_day nextWeek = today + days{7};
    year_month_day lastYear = today - years{1};
    year_month_day threeMonthsLater = today + months{3};

    std::cout << "Next week: " << nextWeek << "\n";
    std::cout << "Last year: " << lastYear << "\n";
    std::cout << "Three months later: " << threeMonthsLater << "\n";

    return 0;
}
```

```c++
#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // 1. Load the time zone database
    const auto& tzdb = get_tzdb(); // Get the global time zone database

    // 2. Get a specific time zone
    const time_zone* shanghaiTz = tzdb.locate_zone("Asia/Shanghai");
    const time_zone* newYorkTz = tzdb.locate_zone("America/New_York");
    const time_zone* utcTz = tzdb.locate_zone("UTC");

    // 3. Create zoned_time: time point in a specific time zone
    // Current time in Shanghai
    zoned_time shanghaiNow{shanghaiTz, system_clock::now()};
    // Current time in New York
    zoned_time newYorkNow{newYorkTz, system_clock::now()};
    // Current time in UTC
    zoned_time utcNow{utcTz, system_clock::now()};

    std::cout << "Current time in Shanghai: " << shanghaiNow << "\n";
    std::cout << "Current time in New York: " << newYorkNow << "\n";
    std::cout << "Current time in UTC: " << utcNow << "\n\n";

    // 4. Convert between time zones
    // Convert a local time in Shanghai to New York time
    year_month_day today = 2026y / February / 27;
    zoned_time shanghaiMeeting{shanghaiTz, local_days{today} + 14h + 30min}; // 2:30 PM Shanghai time
    zoned_time newYorkMeeting{newYorkTz, shanghaiMeeting}; // Convert to New York time

    std::cout << "Meeting time in Shanghai: " << shanghaiMeeting << "\n";
    std::cout << "Same meeting time in New York: " << newYorkMeeting << "\n";

    return 0;
}
```

### 4.6 Remarks
#### Best Practices
1.  **Always Use `std::chrono::steady_clock` for Timing Code**: It is the only monotonic, reliable clock for measuring elapsed time. Never use `system_clock` for this—it can jump backward if the user adjusts the system time.
2.  **Use Predefined Duration Aliases and Literals**: Prefer `std::chrono::seconds`, `10s`, `250ms`, etc., over manually defining `std::chrono::duration` types. It makes your code more readable and less error-prone.
3.  **Let the Compiler Handle Safe Conversions**: Never use `duration_cast` for safe, implicit conversions (coarser → finer, floating-point → any). Let the compiler do it automatically—it is faster and safer.
4.  **Use C++20 Calendar/Time Zone Types for Human-Readable Dates**: Avoid C-style `<ctime>` functions for calendar operations. C++20 `<chrono>` calendar types are type-safe, TMP-powered, and much more flexible.
5.  **Encapsulate Timing Logic in Helper Functions**: As shown in Section 4.4, write reusable helper functions to time code execution. It reduces code duplication and makes your timing logic consistent.

#### Common Pitfalls
1.  **Mixing Time Points from Different Clocks**: You can only subtract two time points or compare them if they use the **exact same clock type**. The compiler will throw an error if you try to mix `system_clock` and `steady_clock` time points.
2.  **Accidental Precision Loss with `duration_cast`**: `duration_cast` truncates fractional ticks by default. For finer → coarser conversions, use `std::chrono::floor`, `std::chrono::ceil`, or `std::chrono::round` (C++17) for controlled rounding.
3.  **Ignoring Time Zones in C++20**: When working with human-readable dates in C++20, always be explicit about time zones. Use `zoned_time` instead of raw `system_clock` time points for local time operations.
4.  **Overusing `high_resolution_clock`**: `high_resolution_clock` is an alias for either `system_clock` or `steady_clock`, depending on the implementation. For portability, prefer `steady_clock` for timing and `system_clock` for real-world time.

#### Production Pattern: Type-Safe Rate Limiter
A common production use case for `<chrono>` is a **rate limiter**: a component that limits how often an operation can be performed. The TMP-powered type safety of `<chrono>` ensures you cannot accidentally mix up time units:
```c++
#include <chrono>
#include <iostream>
#include <functional>

template<typename Duration>
class RateLimiter {
private:
    Duration m_minInterval;
    std::chrono::steady_clock::time_point m_lastExecution;
    bool m_firstExecution = true;

public:
    explicit RateLimiter(Duration minInterval) : m_minInterval(minInterval) {}

    // Execute the function if the rate limit is not exceeded
    bool tryExecute(std::function<void()> func) {
        auto now = std::chrono::steady_clock::now();

        if (m_firstExecution) {
            // First execution: always allow
            m_firstExecution = false;
            m_lastExecution = now;
            func();
            return true;
        }

        // Check if enough time has passed since last execution
        auto timeSinceLast = now - m_lastExecution;
        if (timeSinceLast >= m_minInterval) {
            m_lastExecution = now;
            func();
            return true;
        }

        // Rate limit exceeded: do not execute
        return false;
    }

    // Get the time remaining until the next allowed execution
    Duration timeUntilNextAllowed() const {
        if (m_firstExecution) {
            return Duration{0};
        }
        auto now = std::chrono::steady_clock::now();
        auto timeSinceLast = now - m_lastExecution;
        if (timeSinceLast >= m_minInterval) {
            return Duration{0};
        }
        return m_minInterval - std::chrono::duration_cast<Duration>(timeSinceLast);
    }
};

int main() {
    using namespace std::chrono_literals;

    // Rate limiter: allow execution once every 500ms
    RateLimiter limiter(500ms);

    // Try to execute 10 times in a loop
    for (int i = 0; i < 10; ++i) {
        bool executed = limiter.tryExecute([i]() {
            std::cout << "Execution " << i << " allowed\n";
        });

        if (!executed) {
            auto waitTime = limiter.timeUntilNextAllowed();
            std::cout << "Execution " << i << " blocked. Wait " 
                      << waitTime.count() << "ms\n";
        }

        // Simulate work: sleep for 100ms
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
```

