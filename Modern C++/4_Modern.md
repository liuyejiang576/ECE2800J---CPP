## Chapter 16: C++17 Features

C++17 is a major update that introduced several game-changing features. Unlike C++11's revolutionary changes, C++17 focuses on **simplifying common tasks** and **improving expressiveness**.

---

### 16.1 Structured Bindings

Structured bindings allow you to decompose tuples, pairs, arrays, and even custom types into individual variables in a single declaration.

#### Why Do We Need This?

Before C++17, extracting values from pairs and tuples was verbose and error-prone:

```c++
// Old way (C++11/14)
std::pair<int, std::string> getResult() { return {42, "answer"}; }

auto p = getResult();
int id = p.first;              // What does "first" mean?
std::string name = p.second;   // What does "second" mean?

// With tuples, it's even worse
std::tuple<int, double, std::string> t = {1, 3.14, "pi"};
int i = std::get<0>(t);        // Which index is which?
double d = std::get<1>(t);
std::string s = std::get<2>(t);
```

Structured bindings make this **clean and self-documenting**:

```c++
// C++17 way
auto [id, name] = getResult();        // Clear variable names!
auto [i, d, s] = t;                   // Decompose in one line
```

#### Core Use Cases

**1. Pairs** (most common in map iteration):

```c++
#include <map>
#include <string>
#include <iostream>

int main() {
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };
    
    // Before C++17
    for (const auto& p : scores) {
        std::cout << p.first << ": " << p.second << "\n";
    }
    
    // C++17: Clear and readable
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << "\n";
    }
}
```

**2. Tuples** (multiple return values):

```c++
#include <tuple>
#include <string>

// Return multiple values
std::tuple<int, double, std::string> parseLine(const std::string& line) {
    int id;
    double value;
    std::string label;
    // ... parsing logic
    return {id, value, label};
}

int main() {
    auto [id, value, label] = parseLine("42 3.14 example");
    // Use id, value, label directly
}
```

**3. Arrays** (fixed-size):

```c++
int main() {
    int coords[] = {10, 20, 30};
    auto [x, y, z] = coords;  // x=10, y=20, z=30
    
    // Also works with std::array
    std::array<int, 3> rgb = {255, 128, 0};
    auto [r, g, b] = rgb;
}
```

**4. Structs and Classes** (all non-static members must be public):

```c++
struct Point {
    double x, y;
};

int main() {
    Point p = {3.0, 4.0};
    auto [x, y] = p;          // x=3.0, y=4.0
    std::cout << "Distance: " << std::sqrt(x*x + y*y) << "\n";
}
```

#### Binding Types

The `auto` in `auto [a, b] = ...` determines how values are captured:

```c++
std::pair<int, std::string> p = {42, "hello"};

auto [a, b] = p;              // Copy: a and b are copies
auto& [c, d] = p;             // Reference: c and d refer to p's members
const auto& [e, f] = p;       // Const reference: read-only access
auto&& [g, h] = p;            // Forwarding reference: works with temporaries
```

**Best Practice**: Use `const auto&` for read-only access (avoids copies), and `auto&` when you need to modify the original.

---

### 16.2 if with Initializer

C++17 allows you to declare a variable inside the `if` statement, limiting its scope to the conditional block.

#### Why Is This Useful?

**Problem**: Variables needed only for an `if` condition pollute the outer scope:

```c++
// Before C++17: variable leaks into outer scope
auto it = map.find(key);          // "it" exists after the if
if (it != map.end()) {
    std::cout << it->second << "\n";
}
// "it" is still in scope here (unwanted)

// Hack to limit scope
{
    auto it = map.find(key);
    if (it != map.end()) {
        std::cout << it->second << "\n";
    }
}
```

**Solution**: The init-statement keeps variables scoped to the `if`:

```c++
// C++17: clean and scoped
if (auto it = map.find(key); it != map.end()) {
    std::cout << it->second << "\n";
}
// "it" is NOT accessible here

// Syntax: if (init-statement; condition)
if (int x = compute(); x > 0) {
    std::cout << "Positive: " << x << "\n";
} else {
    std::cout << "Non-positive: " << x << "\n";  // x still valid in else
}
```

#### Practical Examples

```c++
#include <map>
#include <iostream>

int main() {
    std::map<std::string, int> cache;
    cache["key1"] = 100;
    
    // Common pattern: lookup with scoped iterator
    if (auto it = cache.find("key1"); it != cache.end()) {
        std::cout << "Found: " << it->second << "\n";
    }
    
    // Also works with switch
    switch (int x = getValue(); x) {
        case 1: std::cout << "One\n"; break;
        case 2: std::cout << "Two\n"; break;
        default: std::cout << "Other: " << x << "\n";
    }
}
```

**Best Practice**: Always use the init-statement when a variable is only needed for the condition check—this prevents accidental misuse of temporary variables.

---

### 16.3 `std::optional<T>`

`std::optional` represents a value that **may or may not exist**. It's the C++ equivalent of "nullable" types in other languages, but type-safe.

#### Why Do We Need This?

Traditionally, functions that might fail had poor options:

```c++
// Bad option 1: Return a sentinel value
int findIndex(const std::vector<int>& v, int target) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] == target) return i;
    }
    return -1;  // What if -1 is a valid index? Confusing!
}

// Bad option 2: Use pointers (can be nullptr)
int* findPtr(std::vector<int>& v, int target) {
    for (auto& elem : v) {
        if (elem == target) return &elem;
    }
    return nullptr;  // Caller must check for nullptr
}

// Bad option 3: Use output parameters
bool findValue(const std::vector<int>& v, int target, int& out) {
    for (const auto& elem : v) {
        if (elem == target) { out = elem; return true; }
    }
    return false;  // Awkward API
}
```

`std::optional` provides a **clean, type-safe solution**:

```c++
#include <optional>
#include <vector>
#include <iostream>

std::optional<size_t> findIndex(const std::vector<int>& v, int target) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] == target) return i;  // Found: return index
    }
    return std::nullopt;  // Not found: return empty optional
}

int main() {
    std::vector<int> data = {10, 20, 30, 40, 50};
    
    if (auto idx = findIndex(data, 30)) {
        std::cout << "Found at index: " << *idx << "\n";  // Output: 2
    } else {
        std::cout << "Not found\n";
    }
}
```

#### Core Operations

```c++
#include <optional>
#include <string>
#include <iostream>

int main() {
    // Creation
    std::optional<int> empty;              // Empty optional
    std::optional<int> hasValue = 42;      // Contains 42
    std::optional<int> explicitEmpty = std::nullopt;  // Explicitly empty
    
    // Check if has value
    if (hasValue.has_value()) {
        std::cout << "Value: " << hasValue.value() << "\n";  // Throws if empty
        std::cout << "Value: " << *hasValue << "\n";         // Undefined if empty
    }
    
    // Value or default (safe access)
    std::cout << empty.value_or(0) << "\n";  // Output: 0 (default)
    std::cout << hasValue.value_or(0) << "\n";  // Output: 42
    
    // Reset
    hasValue.reset();  // Now empty
    hasValue = 100;    // Now contains 100
}
```

#### Common Patterns

```c++
#include <optional>
#include <string>
#include <map>
#include <iostream>

// Pattern 1: Parse with validation
std::optional<int> parseInteger(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::nullopt;
    }
}

// Pattern 2: Cache lookup
class Cache {
    std::map<std::string, int> data;
public:
    std::optional<int> get(const std::string& key) const {
        auto it = data.find(key);
        if (it != data.end()) return it->second;
        return std::nullopt;
    }
    
    void set(const std::string& key, int value) {
        data[key] = value;
    }
};

int main() {
    Cache cache;
    cache.set("answer", 42);
    
    // Chained operations
    if (auto val = cache.get("answer")) {
        std::cout << "Cached: " << *val << "\n";
    }
    
    // Parsing with validation
    if (auto num = parseInteger("123")) {
        std::cout << "Parsed: " << *num << "\n";
    }
}
```

**Best Practice**: Use `std::optional` whenever a function might not return a valid value. Avoid sentinel values (like `-1` or `nullptr`) which are error-prone.

---

### 16.4 `std::variant<Types...>`

`std::variant` is a **type-safe union** that can hold one of several specified types. Unlike C-style unions, it knows which type it currently holds and prevents undefined behavior.

#### Why Not Use Unions?

C-style unions are unsafe and limited:

```c++
// C-style union: dangerous!
union Data {
    int i;
    double d;
    char* s;
};

Data u;
u.i = 42;
std::cout << u.d;  // Undefined behavior! Reading wrong member
// No way to know which member is active
```

`std::variant` solves this with **type safety**:

```c++
#include <variant>
#include <string>
#include <iostream>

int main() {
    std::variant<int, double, std::string> v;
    
    v = 42;              // v now holds int
    v = 3.14;            // v now holds double
    v = "hello";         // v now holds std::string
    
    // Safe access: throws if wrong type
    std::cout << std::get<std::string>(v) << "\n";  // "hello"
    
    // Safe access: returns nullptr if wrong type
    if (auto* p = std::get_if<int>(&v)) {
        std::cout << *p << "\n";  // Won't execute
    }
    
    // Check current type
    if (std::holds_alternative<std::string>(v)) {
        std::cout << "Holds a string\n";
    }
    
    // Get index of current type (0=int, 1=double, 2=string)
    std::cout << "Index: " << v.index() << "\n";  // Output: 2
}
```

#### The Visitor Pattern with `std::visit`

`std::visit` provides type-safe access to the variant's value using a visitor:

```c++
#include <variant>
#include <string>
#include <iostream>

struct Visitor {
    void operator()(int i) const { 
        std::cout << "int: " << i << "\n"; 
    }
    void operator()(double d) const { 
        std::cout << "double: " << d << "\n"; 
    }
    void operator()(const std::string& s) const { 
        std::cout << "string: " << s << "\n"; 
    }
};

int main() {
    std::variant<int, double, std::string> v = "hello";
    std::visit(Visitor{}, v);  // Output: string: hello
    
    v = 42;
    std::visit(Visitor{}, v);  // Output: int: 42
}
```

**Generic lambda visitor (C++20 style)**:

```c++
// Generic lambda: works for all types
auto printer = [](const auto& value) {
    std::cout << value << "\n";
};

std::visit(printer, v);
```

#### Practical Example: Expression Evaluation

```c++
#include <variant>
#include <string>
#include <iostream>
#include <memory>

struct Add;
struct Sub;
struct Literal;

using Expr = std::variant<
    int,                                    // Literal value
    std::unique_ptr<Add>,                  // Addition
    std::unique_ptr<Sub>                   // Subtraction
>;

struct Add {
    Expr left, right;
};

struct Sub {
    Expr left, right;
};

int evaluate(const Expr& e);

int main() {
    Expr expr = std::make_unique<Add>(
        Expr{5},
        Expr{3}
    );
    // evaluate(expr) would return 8
}
```

**Best Practice**: Use `std::variant` when you need a value that can be one of several types. Prefer it over unions, inheritance hierarchies (for simple cases), or `void*`.

---

### 16.5 `std::string_view`

`std::string_view` is a **non-owning reference** to a character sequence. It provides string-like operations without copying data.

#### Why Is This Important?

Every time you pass a `std::string` to a function taking `const std::string&`, you might trigger a copy:

```c++
// Inefficient: creates temporary std::string
void process(const std::string& s);

process("hello");  // const char* -> std::string (allocation!)

// Also inefficient with substr
std::string str = "Hello, World!";
std::string sub = str.substr(0, 5);  // Allocates new string
```

`std::string_view` avoids these allocations:

```c++
#include <string_view>
#include <string>
#include <iostream>

// Efficient: no allocation for any string-like input
void process(std::string_view sv) {
    std::cout << sv << "\n";
}

int main() {
    std::string str = "Hello";
    const char* cstr = "World";
    
    process(str);                    // No copy (string -> string_view)
    process(cstr);                   // No copy (const char* -> string_view)
    process("Literal");              // No copy (literal -> string_view)
    
    // Substring without allocation
    std::string_view sv = "Hello, World!";
    std::string_view sub = sv.substr(0, 5);  // No allocation!
    std::cout << sub << "\n";  // "Hello"
}
```

#### Key Properties

```c++
#include <string_view>
#include <iostream>

int main() {
    std::string str = "Hello, World!";
    std::string_view sv = str;
    
    // Basic operations (same as std::string)
    std::cout << sv.size() << "\n";         // 13
    std::cout << sv[0] << "\n";             // 'H'
    std::cout << sv.substr(0, 5) << "\n";   // "Hello"
    
    // Remove prefix/suffix (modifies view in place)
    sv.remove_prefix(7);  // Remove "Hello, "
    std::cout << sv << "\n";  // "World!"
    
    sv.remove_suffix(1);  // Remove "!"
    std::cout << sv << "\n";  // "World"
    
    // Comparison
    std::string_view sv1 = "hello";
    std::string_view sv2 = "hello";
    std::cout << (sv1 == sv2) << "\n";  // 1 (compares content)
}
```

#### Important Caveats

**Lifetime Warning**: `std::string_view` does not own data. The underlying string must outlive the view:

```c++
std::string_view dangerous() {
    std::string temp = "temporary";
    return temp;  // BAD: temp is destroyed, view dangles!
}

std::string_view ok() {
    static const char* literal = "safe";  // Static storage
    return literal;  // OK: literal persists
}
```

**Not Null-Terminated**: `std::string_view` may not be null-terminated. Don't use with C functions expecting null-terminated strings:

```c++
std::string_view sv = "hello";
printf("%s", sv.data());  // May work, but not guaranteed
printf("%.*s", (int)sv.size(), sv.data());  // Safe
```

**Best Practice**: Use `std::string_view` for function parameters that only read string data. Return `std::string` when you need to create a new string. Never return a `string_view` that might dangle.

---

### 16.6 Parallel Algorithms

C++17 adds parallel versions of most STL algorithms. Simply add an execution policy as the first argument.

#### Execution Policies

```c++
#include <execution>
#include <algorithm>

// Available policies:
std::execution::seq       // Sequential (same as non-parallel)
std::execution::par       // Parallel (may use multiple threads)
std::execution::par_unseq // Parallel + vectorization (SIMD)
```

#### Examples

```c++
#include <algorithm>
#include <vector>
#include <execution>
#include <numeric>
#include <iostream>

int main() {
    std::vector<int> data(10'000'000);
    std::iota(data.begin(), data.end(), 0);  // Fill with 0, 1, 2, ...
    
    // Sequential sort
    std::sort(data.begin(), data.end());
    
    // Parallel sort (may be faster on large data)
    std::sort(std::execution::par, data.begin(), data.end());
    
    // Parallel + vectorized sort
    std::sort(std::execution::par_unseq, data.begin(), data.end());
    
    // Parallel for_each
    std::for_each(std::execution::par, data.begin(), data.end(), 
                  [](int& n) { n *= 2; });
    
    // Parallel count
    auto count = std::count_if(std::execution::par, 
                               data.begin(), data.end(),
                               [](int n) { return n % 2 == 0; });
    
    // Parallel reduce
    long long sum = std::reduce(std::execution::par,
                                data.begin(), data.end(), 0LL);
}
```

#### When to Use Parallel Algorithms

- **Large data**: Parallelization has overhead; small datasets won't benefit.
- **Independent operations**: Each element must be processable independently.
- **No data races**: Avoid modifying shared state in parallel algorithms.

**Best Practice**: Always benchmark. Parallel algorithms can be slower for small datasets due to thread creation overhead.

---

## Chapter 17: C++20 Concepts

### 17.1 The Problem with Template Errors

Before C++20, template error messages were notoriously cryptic:

```c++
// C++17 code
template<typename T>
T add(T a, T b) {
    return a + b;
}

struct Point { int x, y; };

int main() {
    Point p1{1, 2}, p2{3, 4};
    add(p1, p2);  // Error! But message is 100+ lines...
}
```

The compiler can't tell you "Point doesn't support +" directly—it reports every step of template instantiation.

**Concepts** solve this by letting you **specify requirements explicitly**:

```c++
// C++20 code
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template<Addable T>
T add(T a, T b) {
    return a + b;
}

// Now the error is clear:
// "Point does not satisfy Addable"
```

---

### 17.2 Defining Concepts

A concept is a compile-time predicate that tests whether a type meets certain requirements.

#### Basic Syntax

```c++
#include <concepts>

// Simple concept: check if a type is numeric
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Concept with requirements
template<typename T>
concept Addable = requires(T a, T b) {
    a + b;                          // Must support +
    { a + b } -> std::same_as<T>;   // Result must be T
};

// Multi-requirement concept
template<typename T>
concept Printable = requires(std::ostream& os, T t) {
    { os << t } -> std::same_as<std::ostream&>;
};

template<typename T>
concept Container = requires(T c) {
    typename T::value_type;                          // Must have value_type
    typename T::iterator;                            // Must have iterator
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { c.size() } -> std::convertible_to<size_t>;
};
```

#### Requirement Types

```c++
template<typename T>
concept Example = requires(T t) {
    // 1. Simple requirements: expression must be valid
    t.size();
    
    // 2. Type requirements: nested type must exist
    typename T::value_type;
    
    // 3. Compound requirements: expression + constraints
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.size() } -> std::convertible_to<size_t>;
    
    // 4. Nested requirements: depend on other concepts
    requires std::default_initializable<T>;
    requires std::copy_constructible<T>;
};
```

---

### 17.3 Using Concepts

There are four ways to use concepts in templates:

```c++
#include <concepts>
#include <iostream>

// Method 1: template<Concept T>
template<std::integral T>
T addInts(T a, T b) { return a + b; }

// Method 2: template<typename T> requires Concept<T>
template<typename T> requires std::floating_point<T>
T addFloats(T a, T b) { return a + b; }

// Method 3: function parameter with Concept auto
auto addNumbers(std::integral auto a, std::integral auto b) {
    return a + b;
}

// Method 4: trailing requires clause
template<typename T>
T multiply(T a, T b) requires std::numeric_limits<T>::is_specialized {
    return a * b;
}

int main() {
    addInts(1, 2);           // OK: int is integral
    // addInts(1.5, 2.5);    // Error: double is not integral
    
    addFloats(1.5, 2.5);     // OK: double is floating_point
    // addFloats(1, 2);      // Error: int is not floating_point
}
```

---

### 17.4 Standard Library Concepts

C++20 provides many predefined concepts in `<concepts>`:

```c++
#include <concepts>

// Core language concepts
std::same_as<T, U>              // T and U are the same type
std::derived_from<D, B>         // D is derived from B
std::convertible_to<From, To>   // From can be converted to To
std::common_with<T, U>          // T and U share a common type

// Type properties
std::integral<T>                // T is an integer type
std::floating_point<T>          // T is a floating-point type
std::numeric<T>                 // T is a numeric type (C++23)
std::nullptr_t<T>               // T is nullptr_t

// Construction/destruction
std::default_initializable<T>   // T can be default-constructed
std::copy_constructible<T>      // T can be copy-constructed
std::move_constructible<T>      // T can be move-constructed
std::destructible<T>            // T can be destroyed

// Comparison concepts
std::equality_comparable<T>     // T supports ==
std::totally_ordered<T>         // T supports <, >, <=, >=

// Callable concepts
std::invocable<F, Args...>      // F can be called with Args
std::predicate<F, Args...>      // F returns something convertible to bool
```

---

### 17.5 Practical Example: Generic Container Processing

```c++
#include <concepts>
#include <vector>
#include <list>
#include <iostream>
#include <numeric>

// Concept for anything iterable
template<typename T>
concept Iterable = requires(T t) {
    std::begin(t);
    std::end(t);
};

// Concept for a number container
template<typename T>
concept NumberContainer = Iterable<T> && 
    requires(T t) {
        requires std::is_arithmetic_v<typename T::value_type>;
    };

// Function that works with any number container
template<NumberContainer C>
auto average(const C& container) {
    auto sum = std::accumulate(container.begin(), container.end(), 
                               typename C::value_type{});
    return sum / static_cast<double>(container.size());
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::list<double> l = {1.5, 2.5, 3.5};
    
    std::cout << average(v) << "\n";  // 3
    std::cout << average(l) << "\n";  // 2.5
    
    // std::vector<std::string> s = {"a", "b"};
    // average(s);  // Clear error: string is not arithmetic
}
```

**Best Practice**: Use concepts to document template requirements. Start with standard concepts; only create custom ones when needed.

---

## Chapter 18: Ranges Library

### 18.1 Overview

The C++20 ranges library provides **composable, lazy algorithms** that eliminate the need for intermediate containers.

#### The Problem with Traditional Algorithms

```c++
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Goal: get first 3 even numbers, doubled
    
    // Traditional approach: multiple steps, verbose
    std::vector<int> evens;
    std::copy_if(numbers.begin(), numbers.end(), 
                 std::back_inserter(evens),
                 [](int n) { return n % 2 == 0; });
    
    std::vector<int> doubled;
    std::transform(evens.begin(), evens.end(),
                   std::back_inserter(doubled),
                   [](int n) { return n * 2; });
    
    std::vector<int> result;
    std::copy_n(doubled.begin(), 3, std::back_inserter(result));
    // Result: {4, 8, 12}
    // Problem: Created 3 vectors!
}
```

#### The Ranges Solution

```c++
#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Composable, no intermediate containers
    auto result = numbers 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * 2; })
        | std::views::take(3);
    
    for (int n : result) {
        std::cout << n << " ";  // 4 8 12
    }
    // No intermediate vectors created!
}
```

---

### 18.2 Views

Views are **lazy** range adaptors—they don't do work until you iterate:

```c++
#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Common views
    
    // take: first N elements
    auto first3 = v | std::views::take(3);
    // 1 2 3
    
    // drop: skip first N elements
    auto after3 = v | std::views::drop(3);
    // 4 5 6 7 8 9 10
    
    // reverse
    auto rev = v | std::views::reverse;
    // 10 9 8 7 6 5 4 3 2 1
    
    // filter: keep elements matching predicate
    auto evens = v | std::views::filter([](int n) { return n % 2 == 0; });
    // 2 4 6 8 10
    
    // transform: apply function to each element
    auto squared = v | std::views::transform([](int n) { return n * n; });
    // 1 4 9 16 25 36 49 64 81 100
    
    // Compose multiple views
    auto result = v 
        | std::views::filter([](int n) { return n > 3; })   // 4 5 6 7 8 9 10
        | std::views::transform([](int n) { return n * 2; }) // 8 10 12 14 16 18 20
        | std::views::take(3);                               // 8 10 12
}
```

#### Important Views

| View | Purpose | Example |
|------|---------|---------|
| `take(N)` | First N elements | `v \| views::take(5)` |
| `drop(N)` | Skip first N elements | `v \| views::drop(2)` |
| `filter(pred)` | Keep matching elements | `v \| views::filter(is_even)` |
| `transform(f)` | Apply function | `v \| views::transform(double)` |
| `reverse` | Reverse order | `v \| views::reverse` |
| `keys` | Get keys from map | `map \| views::keys` |
| `values` | Get values from map | `map \| views::values` |
| `split(delim)` | Split by delimiter | `str \| views::split(',')` |
| `join` | Flatten nested ranges | `nested \| views::join` |

---

### 18.3 Range Algorithms

Range algorithms don't require `.begin()` and `.end()`:

```c++
#include <ranges>
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // Range algorithms (cleaner syntax)
    std::ranges::sort(v);              // Sort entire vector
    
    auto it = std::ranges::find(v, 5); // Find element
    
    auto count = std::ranges::count(v, 1);  // Count occurrences
    
    // Copy to another container
    std::vector<int> copy;
    std::ranges::copy(v, std::back_inserter(copy));
    
    // Works with views
    auto evens = v | std::views::filter([](int n) { return n % 2 == 0; });
    std::ranges::sort(evens);  // Sort just the even elements
    
    // Projection: sort by a field
    struct Person {
        std::string name;
        int age;
    };
    std::vector<Person> people = {{"Alice", 30}, {"Bob", 25}};
    std::ranges::sort(people, {}, &Person::age);  // Sort by age
}
```

---

### 18.4 Views vs. Containers

**Views are lazy** - work is done during iteration:

```c++
std::vector<int> v = {1, 2, 3, 4, 5};

auto view = v | std::views::transform([](int n) {
    std::cout << "Processing " << n << "\n";
    return n * 2;
});
// Nothing printed yet!

for (int n : view) {
    // Now "Processing..." is printed
}
```

**Views are non-owning** - be careful with lifetimes:

```c++
auto makeView() {
    std::vector<int> v = {1, 2, 3};
    return v | std::views::take(2);  // BAD: v is destroyed!
}
```

**Best Practice**: Use views to eliminate intermediate containers in algorithm chains. Materialize to a container with `std::ranges::to<std::vector>()` (C++23) or copy algorithms when you need to store results.

---

## Chapter 19: Coroutines

### 19.1 What Are Coroutines?

Coroutines are functions that can **suspend execution** and **resume later**. They enable:

- **Generators**: Produce values on-demand
- **Asynchronous I/O**: Wait without blocking threads
- **Cooperative multitasking**: Yield control explicitly

### 19.2 Coroutine Keywords

C++20 adds three keywords:

| Keyword | Purpose |
|---------|---------|
| `co_await` | Suspend until awaited value is ready |
| `co_yield` | Return a value and suspend |
| `co_return` | Return from coroutine (final value) |

A function is a coroutine if it uses any of these keywords.

### 19.3 Generator Example

Generators produce values lazily:

```c++
#include <coroutine>
#include <iostream>
#include <memory>

// Minimal generator implementation
template<typename T>
struct Generator {
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    
    struct promise_type {
        T value;
        
        Generator get_return_object() {
            return Generator{handle::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
        
        std::suspend_always yield_value(T v) {
            value = v;
            return {};
        }
    };
    
    handle h;
    
    ~Generator() { if (h) h.destroy(); }
    
    bool next() {
        h.resume();
        return !h.done();
    }
    
    T value() const { return h.promise().value; }
};

// Generator that produces Fibonacci numbers
Generator<int> fibonacci(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        auto temp = a;
        a = b;
        b = temp + b;
    }
}

int main() {
    auto gen = fibonacci(10);
    while (gen.next()) {
        std::cout << gen.value() << " ";  // 0 1 1 2 3 5 8 13 21 34
    }
}
```

### 19.4 Important Notes

- C++20 provides the **mechanism** (keywords, coroutine_handle) but limited **library support**.
- Full utilities (like `std::generator`) arrive in C++23.
- Coroutines are stackless: suspension only happens at `co_*` keywords.
- Each coroutine has its own heap-allocated state frame.

**Best Practice**: For production use, prefer established libraries (cppcoro, Boost.Asio) until C++23 `std::generator` is widely available.

---

## Chapter 20: Modules

### 20.1 The Problem with Headers

Traditional C++ uses `#include` with significant drawbacks:

```c++
// Traditional header
#ifndef MYHEADER_H
#define MYHEADER_H

#include <vector>  // Every translation unit recompiles <vector>
#include <string>

#define MAX_SIZE 100  // Macro pollutes global namespace

void doSomething();

#endif
```

**Problems**:
1. **Slow compilation**: Headers are textually included in every translation unit.
2. **Macro pollution**: `#define` in one header affects all code after it.
3. **Order dependencies**: Include order can break builds.
4. **Opaqueness**: Private implementation must be in headers (visible to all).

### 20.2 Module Basics

C++20 modules replace headers with a cleaner mechanism:

```c++
// math.ixx (module interface file)
export module math;

export int add(int a, int b) {
    return a + b;
}

export int multiply(int a, int b) {
    return a * b;
}

// Internal helper - not exported, truly private
int internalHelper() {
    return 42;
}
```

```c++
// main.cpp
import math;
import <iostream>;  // Import header as module

int main() {
    std::cout << add(2, 3) << "\n";       // 5
    std::cout << multiply(2, 3) << "\n";  // 6
    // internalHelper();  // Error: not exported
}
```

**Benefits**:
1. **Faster compilation**: Modules compile once, not every time they're used.
2. **No macros leak**: Macros in a module don't affect importers.
3. **True encapsulation**: Non-exported entities are truly private.
4. **No include guards needed**: Modules are imported once automatically.

### 20.3 Module Partitions

Large modules can be split into partitions:

```c++
// math.cppm (primary module interface)
export module math;
export import :arithmetic;  // Export arithmetic partition
export import :geometry;    // Export geometry partition

// math-arithmetic.cppm (partition)
export module math:arithmetic;
export int add(int a, int b) { return a + b; }
export int subtract(int a, int b) { return a - b; }

// math-geometry.cppm (partition)
export module math:geometry;
export int area(int w, int h) { return w * h; }
```

### 20.4 Header Units

Import existing headers as modules:

```c++
// Import standard library as modules
import <vector>;
import <string>;
import <iostream>;
import <algorithm>;

int main() {
    std::vector<int> v = {1, 2, 3};
    for (const auto& n : v) {
        std::cout << n << " ";
    }
}
```

### 20.5 Module Adoption

**Current Status** (2024):
- All major compilers support modules.
- Build system support (CMake) is improving.
- Standard library modules (`import std;`) are part of C++23.

**Migration Strategy**:
1. Start with header units for standard library (`import <vector>;`).
2. Gradually convert internal headers to modules.
3. Keep public API headers for backwards compatibility.

---

## Chapter 21: Modern C++ Best Practices

### 21.1 Resource Management

**Rule of Zero/Three/Five**:

```c++
// Rule of Zero: Let compiler generate everything
class SimpleClass {
    std::string name;
    std::vector<int> data;
    // Compiler-generated destructor, copy, and move work correctly
};

// Rule of Five: When managing resources manually
class ResourceOwner {
    int* buffer;
    size_t size;
public:
    // 1. Destructor
    ~ResourceOwner() { delete[] buffer; }
    
    // 2. Copy constructor
    ResourceOwner(const ResourceOwner& other) 
        : size(other.size), buffer(new int[other.size]) {
        std::copy(other.buffer, other.buffer + size, buffer);
    }
    
    // 3. Copy assignment
    ResourceOwner& operator=(const ResourceOwner& other) {
        if (this != &other) {
            delete[] buffer;
            size = other.size;
            buffer = new int[size];
            std::copy(other.buffer, other.buffer + size, buffer);
        }
        return *this;
    }
    
    // 4. Move constructor
    ResourceOwner(ResourceOwner&& other) noexcept
        : buffer(other.buffer), size(other.size) {
        other.buffer = nullptr;
        other.size = 0;
    }
    
    // 5. Move assignment
    ResourceOwner& operator=(ResourceOwner&& other) noexcept {
        if (this != &other) {
            delete[] buffer;
            buffer = other.buffer;
            size = other.size;
            other.buffer = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
```

**Prefer smart pointers**:

```c++
// Bad: manual memory management
void bad() {
    Widget* w = new Widget();
    doSomething();  // If this throws, w leaks!
    delete w;
}

// Good: automatic cleanup
void good() {
    auto w = std::make_unique<Widget>();
    doSomething();  // Safe: w is cleaned up even if exception thrown
}
```

---

### 21.2 Type Safety

**Use `auto` wisely**:

```c++
// Good: type is obvious or verbose
auto ptr = std::make_unique<Widget>();
auto it = container.find(key);
auto [key, value] = *map.begin();

// Avoid: type is unclear
auto result = process(data);  // What type is result?
```

**Use `enum class`**:

```c++
// Bad: plain enum pollutes namespace
enum Color { RED, GREEN, BLUE };
int x = RED;  // Implicitly converts to int

// Good: scoped enum
enum class Color { Red, Green, Blue };
Color c = Color::Red;  // Must use scope
// int x = Color::Red;  // Error: no implicit conversion
```

**Use `std::optional` instead of sentinel values**:

```c++
// Bad: magic number
int findIndex(int value) {
    // ...
    return -1;  // What if -1 is valid?
}

// Good: explicit optional
std::optional<size_t> findIndex(int value) {
    // ...
    return std::nullopt;  // Clear: no value
}
```

---

### 21.3 Performance

**Use move semantics**:

```c++
std::vector<Widget> widgets;
Widget w;
widgets.push_back(std::move(w));  // Move instead of copy
```

**Pass by reference for large objects**:

```c++
// Good: no copy
void process(const std::vector<int>& data);

// Bad: copies entire vector
void process(std::vector<int> data);
```

**Reserve capacity**:

```c++
std::vector<int> v;
v.reserve(1000);  // Pre-allocate, avoid reallocations
for (int i = 0; i < 1000; ++i) {
    v.push_back(i);
}
```

---

### 21.4 Code Clarity

**Prefer range-based for loops**:

```c++
// Bad: index-based
for (size_t i = 0; i < v.size(); ++i) {
    std::cout << v[i] << "\n";
}

// Good: range-based
for (const auto& elem : v) {
    std::cout << elem << "\n";
}
```

**Use structured bindings**:

```c++
// Bad
auto p = getPair();
int id = p.first;
std::string name = p.second;

// Good
auto [id, name] = getPair();
```

**Use `const` liberally**:

```c++
void process(const std::vector<int>& data) {
    const size_t size = data.size();
    const auto& first = data[0];
    // Clearly shows these won't change
}
```

---

## Chapter 22: Common Design Patterns

### 22.1 RAII (Resource Acquisition Is Initialization)

RAII ties resource lifetime to object lifetime:

```c++
#include <fstream>
#include <mutex>
#include <memory>

class FileHandle {
    FILE* file;
public:
    FileHandle(const char* filename, const char* mode) 
        : file(fopen(filename, mode)) {
        if (!file) throw std::runtime_error("Cannot open file");
    }
    
    ~FileHandle() {
        if (file) fclose(file);  // Automatic cleanup
    }
    
    // Disable copy (file handles are unique)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    // Enable move
    FileHandle(FileHandle&& other) noexcept : file(other.file) {
        other.file = nullptr;
    }
    
    FILE* get() const { return file; }
};

// Standard RAII types
void example() {
    auto ptr = std::make_unique<int>(42);      // Memory RAII
    std::ifstream file("data.txt");            // File RAII
    std::lock_guard<std::mutex> lock(mtx);     // Mutex RAII
}
```

### 22.2 Factory Pattern

```c++
#include <memory>
#include <string>

class Widget {
public:
    virtual ~Widget() = default;
    virtual void doSomething() = 0;
};

class WidgetA : public Widget {
public:
    void doSomething() override { std::cout << "Widget A\n"; }
};

class WidgetB : public Widget {
public:
    void doSomething() override { std::cout << "Widget B\n"; }
};

enum class WidgetType { A, B };

std::unique_ptr<Widget> createWidget(WidgetType type) {
    switch (type) {
        case WidgetType::A: return std::make_unique<WidgetA>();
        case WidgetType::B: return std::make_unique<WidgetB>();
    }
    throw std::invalid_argument("Unknown widget type");
}
```

### 22.3 Observer Pattern with `std::function`

```c++
#include <functional>
#include <vector>
#include <algorithm>

template<typename... Args>
class Signal {
public:
    using Slot = std::function<void(Args...)>;
    
    void connect(Slot slot) {
        slots.push_back(std::move(slot));
    }
    
    void emit(Args... args) {
        for (const auto& slot : slots) {
            slot(args...);
        }
    }
    
private:
    std::vector<Slot> slots;
};

// Usage
int main() {
    Signal<int> valueChanged;
    
    valueChanged.connect([](int value) {
        std::cout << "Value changed to: " << value << "\n";
    });
    
    valueChanged.emit(42);  // Output: Value changed to: 42
}
```

### 22.4 Singleton Pattern (Meyers' Singleton)

```c++
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;  // Thread-safe in C++11+
        return instance;
    }
    
    // Delete copy and move
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    
private:
    Singleton() = default;
    ~Singleton() = default;
};
```

---

## Chapter 23: C++23 Features

C++23 builds on C++20 with several quality-of-life improvements:

### 23.1 `std::expected<T, E>`

Better error handling without exceptions:

```c++
#include <expected>
#include <string>

std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");
    }
    return a / b;
}

int main() {
    auto result = divide(10, 2);
    if (result) {
        std::cout << *result << "\n";  // 5
    } else {
        std::cout << result.error() << "\n";
    }
}
```

### 23.2 Deducing `this`

Explicit object parameter for member functions:

```c++
struct Widget {
    void foo(this Widget& self) {  // Explicit object parameter
        std::cout << "Non-const\n";
    }
    
    void foo(this const Widget& self) {
        std::cout << "Const\n";
    }
    
    void foo(this Widget&& self) {
        std::cout << "Rvalue\n";
    }
};

int main() {
    Widget w;
    w.foo();                 // Non-const
    const Widget cw;
    cw.foo();                // Const
    Widget().foo();          // Rvalue
}
```

### 23.3 `std::print` and `std::println`

Type-safe formatted output (Python-style):

```c++
#include <print>

int main() {
    std::print("Hello, {}!\n", "World");
    std::println("Value: {}", 42);  // Auto newline
    std::println("{:.2f}", 3.14159);  // "3.14"
}
```

### 23.4 `std::generator<T>`

Standard library generator:

```c++
#include <generator>
#include <iostream>

std::generator<int> fibonacci(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        std::tie(a, b) = std::make_pair(b, a + b);
    }
}

int main() {
    for (int n : fibonacci(10)) {
        std::cout << n << " ";
    }
}
```

### 23.5 `std::ranges::to`

Convert views to containers:

```c++
#include <ranges>
#include <vector>

int main() {
    auto result = std::views::iota(1, 10)
                | std::views::filter([](int n) { return n % 2 == 0; })
                | std::views::transform([](int n) { return n * n; })
                | std::ranges::to<std::vector>();  // Materialize
    // result: vector {4, 16, 36, 64}
}
```

---

## Chapter 24: Summary

### Key Takeaways by Standard

**C++11 Revolution**:
- Move semantics and rvalue references
- Smart pointers (`unique_ptr`, `shared_ptr`)
- `auto`, `decltype`, `nullptr`
- Lambda expressions
- Range-based for loops
- `constexpr` functions
- Variadic templates

**C++14 Refinements**:
- Generic lambdas (`[](auto x)`)
- Return type deduction
- Binary literals (`0b1010`)
- Relaxed `constexpr`

**C++17 Additions**:
- Structured bindings
- `if` with initializer
- `std::optional`, `std::variant`, `std::any`
- `std::string_view`
- Parallel algorithms
- Fold expressions

**C++20 Modernization**:
- Concepts
- Ranges library
- Coroutines
- Modules

**C++23 Quality of Life**:
- `std::expected`
- Deducing `this`
- `std::print`
- `std::generator`
- `std::ranges::to`

### Best Practices Summary

| Category | Guideline |
|----------|-----------|
| **Memory** | Use smart pointers, follow Rule of Zero/Five |
| **Types** | Prefer `enum class`, `std::optional`, `auto` judiciously |
| **Performance** | Use move semantics, pass by reference, reserve containers |
| **Clarity** | Use structured bindings, range-based loops, `const` |
| **Templates** | Use concepts to document requirements |
| **Algorithms** | Use ranges for composability, parallel algorithms for large data |

---

## Appendix: Quick Reference

### Smart Pointers

| Type | Ownership | Copyable | Use Case |
|------|-----------|----------|----------|
| `unique_ptr` | Exclusive | No (move only) | Default choice |
| `shared_ptr` | Shared | Yes | Multiple owners |
| `weak_ptr` | None | Yes | Break cycles, observe |

### Value Categories

| Category | Has Identity | Can Move From | Example |
|----------|--------------|---------------|---------|
| lvalue | Yes | No | `x`, `*ptr` |
| xvalue | Yes | Yes | `std::move(x)` |
| prvalue | No | Yes | `42`, `x + y` |

### Common Concepts

```c++
std::integral<T>          // T is integer type
std::floating_point<T>    // T is float/double
std::same_as<T, U>        // T and U are same type
std::convertible_to<From, To>
std::copy_constructible<T>
std::default_initializable<T>
```

### Range-based For Patterns

```c++
for (const auto& x : container)   // Read-only
for (auto& x : container)         // Modify elements
for (auto&& x : container)        // Forward (works with proxies)
for (const auto& [k, v] : map)    // Structured binding
```

---

This concludes the Modern C++ Tutorial. The journey from traditional C++ to Modern C++ involves learning new idioms and unlearning old habits. Practice these concepts through hands-on coding, and refer back to this tutorial whenever you need a refresher on specific features.
