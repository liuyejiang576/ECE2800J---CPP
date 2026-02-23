# Modern C++ Exercises

## (18%) 1. Move Semantics and Smart Pointers

1. (2%) True or false? After calling `std::move()` on an object, the object is guaranteed to be in an empty state.

   __

2. (3%) Consider the following code:

```c++
class Widget {
public:
    Widget() { std::cout << "D"; }
    Widget(const Widget&) { std::cout << "C"; }
    Widget(Widget&&) noexcept { std::cout << "M"; }
    ~Widget() { std::cout << "X"; }
};

Widget create() {
    Widget w;
    return w;
}

int main() {
    Widget w1;
    Widget w2 = std::move(w1);
    Widget w3 = create();
}
```

Assume no compiler optimizations (no RVO). What is the output?

__

3. (4%) Given the following code:

```c++
std::shared_ptr<int> p1(new int(42));
std::shared_ptr<int> p2 = p1;
std::shared_ptr<int> p3(new int(42));
p3 = p1;
```

After executing this code:
- How many `int` objects exist in memory? __
- What is the reference count of the surviving `int` object? __
- What happens to the `int` with value 42 that was originally pointed to by `p3`? __

4. (3%) Why is `std::make_unique<T>(...)` preferred over `std::unique_ptr<T>(new T(...))`? Give at least two reasons.

__

__

5. (3%) Consider the following class declaration. What special member functions should be explicitly defined (or deleted) following the Rule of Five? Write "default", "delete", or "implement" for each.

```c++
class Buffer {
    int* data;
    size_t size;
public:
    // Constructor
    Buffer(size_t n);
    
    // Destructor
    __ ~Buffer();
    
    // Copy constructor
    __ Buffer(const Buffer& other);
    
    // Copy assignment
    __ Buffer& operator=(const Buffer& other);
    
    // Move constructor
    __ Buffer(Buffer&& other) noexcept;
    
    // Move assignment
    __ Buffer& operator=(Buffer&& other) noexcept;
};
```

6. (3%) Explain why a `std::weak_ptr` is needed in the following scenario, and how it solves the problem:

```c++
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // Problem here!
};
```

__

__

## (15%) 2. Type Deduction and Lambda Expressions

1. (3%) For each of the following declarations, determine the actual type:

```c++
int x = 10;
const int cx = 20;
int& ref = x;

auto a = x;          // Type: __
auto b = cx;         // Type: __
auto c = ref;        // Type: __
auto& d = cx;        // Type: __
auto&& e = 10;       // Type: __
auto&& f = x;        // Type: __
```

2. (2%) What is the output of the following code?

```c++
int x = 5;
auto f1 = [x]() mutable { return ++x; };
auto f2 = [&x]() { return ++x; };

std::cout << f1() << " ";  // __
std::cout << f1() << " ";  // __
std::cout << f2() << " ";  // __
std::cout << x << std::endl;  // __
```

3. (4%) Write a lambda expression that:
   - Takes a vector of integers by reference
   - Returns the count of elements greater than a captured threshold value
   - The threshold should be captured by value

```c++
int threshold = 10;
std::vector<int> data = {5, 15, 20, 3, 25};

auto countAbove = _________________;

std::cout << countAbove(data) << std::endl;  // Should print 3
```

4. (3%) Consider the following code that uses a lambda with `std::sort`. What will the vector contain after sorting?

```c++
struct Person {
    std::string name;
    int age;
};

std::vector<Person> people = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}};

std::sort(people.begin(), people.end(), 
          [](const Person& a, const Person& b) { return a.age < b.age; });

// people[0].name = __, people[1].name = __, people[2].name = __
```

5. (3%) Complete the following code to create a generic lambda (C++14) that adds two values and returns the result:

```c++
auto add = _________________;

auto r1 = add(1, 2);      // int
auto r2 = add(1.5, 2.5);  // double
auto r3 = add(std::string("Hello "), std::string("World"));  // string
```

## (12%) 3. Modern C++ Language Features

1. (2%) What is the output of the following code?

```c++
void f(int) { std::cout << "int\n"; }
void f(int*) { std::cout << "ptr\n"; }
void f(nullptr_t) { std::cout << "null\n"; }

int main() {
    f(0);
    f(nullptr);
    int* p = nullptr;
    f(p);
}
```

__

2. (3%) Consider the following code:

```c++
enum class Color { RED, GREEN, BLUE };
enum class Size { SMALL, MEDIUM, LARGE };

Color c = Color::RED;
Size s = Size::SMALL;

// Which of the following compile? Write Yes or No.
int x = c;                      // __
int y = static_cast<int>(c);    // __
if (c < Color::GREEN) { }       // __
if (c == s) { }                 // __
```

3. (3%) What is the value of each variable after execution?

```c++
constexpr int square(int x) { return x * x; }
constexpr int SIZE = square(5);

int arr[SIZE];              // Is this valid? __
constexpr int fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);  // C++14 allows this
}
constexpr int F10 = fib(10);  // Value: __
```

4. (2%) What happens with each of the following initializations?

```c++
int a = 5.5;      // __
int b{5.5};       // __
int c = {5.5};    // __
int d(5.5);       // __
```

5. (2%) Why is the following code problematic? What is the issue?

```c++
std::vector<int> v{5};
std::vector<int> w(5);
```

__

## (15%) 4. Variadic Templates and Perfect Forwarding

1. (4%) Complete the following variadic template function to print all arguments separated by spaces:

```c++
template<typename T>
void print(T value) {
    std::cout << value << std::endl;
}

template<typename T, typename... Args>
void print(T first, Args... rest) {
    ___________________
    ___________________
}

// Usage: print(1, 2.5, "hello", 'c');
// Output: 1 2.5 hello c
```

2. (3%) Rewrite the above using a C++17 fold expression:

```c++
template<typename... Args>
void print(Args... args) {
    ___________________
}
```

3. (4%) Consider the following code:

```c++
template<typename T>
void func(T&& arg) {
    process(std::forward<T>(arg));
}

void process(int& x) { std::cout << "lvalue: " << x << "\n"; }
void process(int&& x) { std::cout << "rvalue: " << x << "\n"; }

int main() {
    int x = 42;
    func(x);      // Output: __
    func(42);     // Output: __
    func(std::move(x));  // Output: __
}
```

4. (4%) Why doesn't the following factory function work correctly? What is the problem, and how would you fix it?

```c++
template<typename T, typename Arg>
std::unique_ptr<T> factory(Arg arg) {
    return std::make_unique<T>(arg);
}

// Problem: _________________

// Fixed version:
template<typename T, typename Arg>
std::unique_ptr<T> factory(_____________) {
    return std::make_unique<T>(_____________);
}
```

## (12%) 5. Standard Library and Containers

1. (3%) Fill in the comparison table for ordered vs unordered containers:

| Feature | std::map | std::unordered_map |
|---------|----------|-------------------|
| Ordering | __ | __ |
| Lookup time | __ | __ |
| Memory overhead | __ | __ |

2. (3%) What is the output of the following code?

```c++
std::tuple<int, double, std::string> t = std::make_tuple(42, 3.14, "hello");

std::cout << std::get<0>(t) << " ";   // __
std::cout << std::get<1>(t) << " ";   // __
std::cout << std::get<2>(t) << "\n";  // __

// C++17 structured bindings
auto [i, d, s] = t;
// i = __, d = __, s = __
```

3. (3%) For each container, indicate the best use case:

```c++
// Choose from: std::vector, std::list, std::deque, std::array

// Frequent insertions at end, random access needed: __
// Frequent insertions at both ends: __
// Fixed size, stack-allocated, no heap overhead: __
// Frequent insertions/deletions in middle: __
```

4. (3%) What is the difference between `std::string` and `std::string_view`? When should you use each?

__

__

## (13%) 6. Concurrency

1. (3%) What is the output of the following code (assuming proper synchronization)?

```c++
std::atomic<int> counter{0};

void increment() {
    for (int i = 0; i < 1000; ++i) {
        counter++;
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();
    std::cout << counter << std::endl;  // __
}
```

2. (4%) Complete the thread-safe counter class:

```c++
class SafeCounter {
    int value = 0;
    std::mutex mtx;
public:
    void increment() {
        ___________________
        ___________________
    }
    
    int get() {
        ___________________
        ___________________
    }
};
```

3. (3%) Why do we use `std::lock_guard` or `std::unique_lock` instead of manually calling `mtx.lock()` and `mtx.unlock()`?

__

__

4. (3%) What is the purpose of `std::condition_variable`? Give a real-world scenario where it would be useful.

__

__

## (15%) 7. Type Traits and Concepts

1. (4%) For each type trait, write what it checks for and give an example:

```c++
std::is_integral<T>::value     // Checks: __, Example: __
std::is_pointer<T>::value      // Checks: __, Example: __
std::is_same<T, U>::value      // Checks: __, Example: __
std::is_base_of<B, D>::value   // Checks: __, Example: __
```

2. (4%) Complete the following `enable_if` example to create a function that only accepts integral types:

```c++
template<typename T>
_________________ multiply(T a, T b) {
    return a * b;
}

// This function should NOT compile for floating-point types
```

3. (4%) Using C++20 concepts, rewrite the following template to require that T is `std::integral`:

```c++
// Before (no constraints)
template<typename T>
T add(T a, T b) {
    return a + b;
}

// After (with concepts)
_________________
T add(T a, T b) {
    return a + b;
}
```

4. (3%) Define a concept `Numeric` that is satisfied if T is either an integral type or a floating-point type:

```c++
template<typename T>
concept Numeric = _________________;
```

## (Bonus 5%) 8. Ranges (C++20)

Given the following code:

```c++
std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

auto result = numbers 
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; })
    | std::views::take(3);
```

1. (2%) What values will `result` contain when iterated?

__

2. (3%) Are the operations performed eagerly (all at once) or lazily (on demand)? Explain what this means and why it matters.

__

---

## (40%) 9. Smart Resource Manager

Implement a class `ResourceManager` that manages a dynamically allocated array of resources. The class should:

1. Have a constructor that takes a size and allocates an array of that size
2. Properly manage memory using RAII principles
3. Support move semantics but not copy semantics (unique ownership)
4. Provide bounds-checked element access
5. Support range-based for loops

```c++
class ResourceManager {
    // Your implementation here
    // Include all necessary member functions
};
```

Write the complete class implementation below:

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

---

## (60%) 10. Thread-Safe Message Queue

Implement a thread-safe message queue that can be used for producer-consumer scenarios. The queue should:

1. Support multiple producers and consumers
2. Block consumers when the queue is empty
3. Support timeout on blocking operations
4. Be generic over the message type

Complete the following class:

```c++
template<typename T>
class MessageQueue {
public:
    // Adds a message to the queue
    void push(T message);
    
    // Removes and returns a message, blocks if empty
    T pop();
    
    // Tries to pop a message with timeout
    // Returns true if successful, false if timeout
    bool try_pop_for(T& message, std::chrono::milliseconds timeout);
    
    // Returns the current size
    size_t size() const;

private:
    // Your data members here
};
```

Write the complete implementation:

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__

__