# Modern C++ Exercises - Answers and Grading Rubrics

## 1. Move Semantics and Smart Pointers (18%)

### 1.1 (2%)

**Answer:** False

**Explanation:** After calling `std::move()` on an object, the object is left in a valid but unspecified state. It is NOT guaranteed to be empty. The object must still be destructible and assignable, but its exact value is implementation-defined. The `std::move` is just a cast to an rvalue reference - it doesn't actually move anything by itself.

**Grading Rubric:**
- Full credit (2%): Correct answer with understanding that the state is "valid but unspecified"
- Partial credit (1%): Correct answer but incorrect explanation
- No credit (0%): Incorrect answer

---

### 1.2 (3%)

**Answer:** D M D M X X

**Explanation:**
1. `Widget w1;` - Default constructor: outputs "D"
2. `Widget w2 = std::move(w1);` - Move constructor: outputs "M"
3. Inside `create()`: `Widget w;` - Default constructor: outputs "D"
4. Return from `create()`: Move constructor (without RVO): outputs "M"
5. At end of `create()`: Destructor for local w: outputs "X"
6. At end of `main()`: Destructor for w3: outputs "X"

Note: The destructors for w1 and w2 would also be called, but since we assume no RVO and the problem asks for the shown output, we focus on the main sequence.

**Grading Rubric:**
- Full credit (3%): Correct sequence D M D M X X (or equivalent understanding)
- Partial credit (1-2%): Partially correct sequence or missing destructor calls
- No credit (0%): Completely incorrect sequence

---

### 1.3 (4%)

**Answer:**
- How many `int` objects exist in memory? **1**
- What is the reference count? **3**
- What happens to the original `int` from p3? **It is deleted (memory freed)**

**Explanation:**
1. `p1` creates one `int(42)`
2. `p2 = p1` increases reference count to 2, no new int created
3. `p3` creates a second `int(42)` (different object!)
4. `p3 = p1` copies p1 to p3, increasing reference count to 3
5. The assignment to p3 decreases the reference count of its previous object to 0, causing it to be deleted

**Grading Rubric:**
- Full credit (4%): All three parts correct
- Partial credit (2-3%): Two parts correct
- Partial credit (1%): One part correct
- No credit (0%): All parts incorrect

---

### 1.4 (3%)

**Answer:**
1. **Exception safety**: If `new T(...)` throws after `std::unique_ptr` constructor is called, there's no leak. But with direct `new`, if the constructor throws, memory leaks.
2. **No code duplication**: Using `make_unique` avoids explicitly writing `new`, reducing potential for errors.
3. **Performance**: `make_unique` can be more efficient due to single allocation for the control block (relevant for `make_shared`).

**Grading Rubric:**
- Full credit (3%): At least two correct reasons
- Partial credit (1-2%): One correct reason or partially correct explanation
- No credit (0%): Incorrect or no valid reasons

---

### 1.5 (3%)

**Answer:**
```c++
// Destructor
implement ~Buffer();  // Must delete[] data

// Copy constructor
implement Buffer(const Buffer& other);  // Deep copy needed

// Copy assignment
implement Buffer& operator=(const Buffer& other);  // Deep copy needed

// Move constructor
implement Buffer(Buffer&& other) noexcept;  // Transfer ownership

// Move assignment
implement Buffer& operator=(Buffer&& other) noexcept;  // Transfer ownership
```

**Explanation:** Since `Buffer` manages a raw pointer (`int* data`), it needs all five special member functions implemented to properly handle resource management. The Rule of Five states that if you need to implement any of these, you should implement all five.

**Grading Rubric:**
- Full credit (3%): All five correctly identified as "implement"
- Partial credit (2%): Four correct
- Partial credit (1%): Three correct
- No credit (0%): Two or fewer correct

---

### 1.6 (3%)

**Answer:**
The problem is a **circular reference** (memory leak). When two nodes point to each other with `shared_ptr`:
- Node A's `prev` points to Node B (owns B)
- Node B's `next` points to Node A (owns A)
- Neither can be destroyed because each is still owned by the other

**Solution:** Use `std::weak_ptr` for `prev`:
```c++
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // Does not increase reference count!
};
```

`weak_ptr` doesn't increase the reference count, breaking the cycle and allowing proper destruction.

**Grading Rubric:**
- Full credit (3%): Correctly identifies circular reference and explains weak_ptr solution
- Partial credit (2%): Identifies circular reference but weak explanation
- Partial credit (1%): Mentions memory leak but not circular reference
- No credit (0%): Incorrect or no answer

---

## 2. Type Deduction and Lambda Expressions (15%)

### 2.1 (3%)

**Answer:**
```c++
auto a = x;          // Type: int
auto b = cx;         // Type: int
auto c = ref;        // Type: int
auto& d = cx;        // Type: const int&
auto&& e = 10;       // Type: int&&
auto&& f = x;        // Type: int&
```

**Explanation:**
- `auto` strips references and top-level const by default
- `auto&` preserves const
- `auto&&` is a forwarding reference: binds to rvalues as rvalue reference, to lvalues as lvalue reference

**Grading Rubric:**
- Full credit (3%): All six correct
- Partial credit (2%): 4-5 correct
- Partial credit (1%): 2-3 correct
- No credit (0%): 0-1 correct

---

### 2.2 (2%)

**Answer:**
```c++
std::cout << f1() << " ";  // 6
std::cout << f1() << " ";  // 7
std::cout << f2() << " ";  // 6
std::cout << x << std::endl;  // 6
```

**Explanation:**
- `f1` captures `x` by value with `mutable`, so it modifies its own copy (starts at 5, increments)
- `f2` captures `x` by reference, so it modifies the actual `x`
- After `f2()` increments, `x` becomes 6

**Grading Rubric:**
- Full credit (2%): All four values correct
- Partial credit (1%): Two or three correct
- No credit (0%): One or none correct

---

### 2.3 (4%)

**Answer:**
```c++
int threshold = 10;
std::vector<int> data = {5, 15, 20, 3, 25};

auto countAbove = [threshold](const std::vector<int>& v) {
    return std::count_if(v.begin(), v.end(), 
                         [threshold](int n) { return n > threshold; });
};
```

Or with C++20 ranges:
```c++
auto countAbove = [threshold](const std::vector<int>& v) {
    return std::ranges::count_if(v, [threshold](int n) { return n > threshold; });
};
```

**Grading Rubric:**
- Full credit (4%): Correct lambda with value capture, reference parameter, and correct logic
- Partial credit (2-3%): Minor syntax errors or slight logic issues
- Partial credit (1%): Major issues but shows understanding of lambdas
- No credit (0%): Completely incorrect

---

### 2.4 (3%)

**Answer:**
```
people[0].name = Bob, people[1].name = Alice, people[2].name = Charlie
```

**Explanation:** The lambda sorts by age in ascending order:
- Bob: 25 (youngest)
- Alice: 30
- Charlie: 35 (oldest)

**Grading Rubric:**
- Full credit (3%): All three names in correct order
- Partial credit (1-2%): Partially correct ordering
- No credit (0%): Incorrect ordering

---

### 2.5 (3%)

**Answer:**
```c++
auto add = [](auto a, auto b) { return a + b; };
```

**Explanation:** C++14 generic lambdas allow `auto` in parameters, making the lambda work with any type that supports the `+` operator.

**Grading Rubric:**
- Full credit (3%): Correct generic lambda syntax
- Partial credit (1-2%): Syntax errors but understanding shown
- No credit (0%): Incorrect syntax

---

## 3. Modern C++ Language Features (12%)

### 3.1 (2%)

**Answer:**
```
int
ptr
ptr
```

**Explanation:**
- `f(0)` calls `f(int)` because `0` is an integer literal
- `f(nullptr)` is ambiguous between `f(int*)` and `f(nullptr_t)`, but `nullptr_t` overload is preferred
- `f(p)` where `p` is `int*` calls `f(int*)`

**Grading Rubric:**
- Full credit (2%): All three correct
- Partial credit (1%): Two correct
- No credit (0%): Zero or one correct

---

### 3.2 (3%)

**Answer:**
```c++
int x = c;                      // No (needs cast)
int y = static_cast<int>(c);    // Yes
if (c < Color::GREEN) { }       // Yes (same enum class)
if (c == s) { }                 // No (different enum classes)
```

**Explanation:**
- `enum class` values are strongly typed and don't implicitly convert to int
- Values of the same enum class can be compared
- Values of different enum classes cannot be compared

**Grading Rubric:**
- Full credit (3%): All four correct
- Partial credit (2%): Three correct
- Partial credit (1%): One or two correct
- No credit (0%): None correct

---

### 3.3 (3%)

**Answer:**
```c++
int arr[SIZE];              // Is this valid? Yes
constexpr int F10 = fib(10);  // Value: 55
```

**Explanation:**
- `square(5)` is evaluated at compile time, giving `SIZE = 25`, which is valid for array size
- `fib(10)` computes the 10th Fibonacci number: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55

**Grading Rubric:**
- Full credit (3%): Both correct
- Partial credit (1-2%): One correct or Fibonacci calculation error
- No credit (0%): Both incorrect

---

### 3.4 (2%)

**Answer:**
```c++
int a = 5.5;      // OK, a = 5 (narrowing allowed, value truncated)
int b{5.5};       // Error! Narrowing not allowed in brace initialization
int c = {5.5};    // Error! Narrowing not allowed in brace initialization
int d(5.5);       // OK, d = 5 (narrowing allowed, value truncated)
```

**Grading Rubric:**
- Full credit (2%): All four correct
- Partial credit (1%): Two or three correct
- No credit (0%): Zero or one correct

---

### 3.5 (2%)

**Answer:**
- `v{5}` creates a vector with ONE element having value 5 (initializer list constructor)
- `w(5)` creates a vector with FIVE elements, each default-initialized to 0 (size constructor)

**Grading Rubric:**
- Full credit (2%): Correctly explains both behaviors
- Partial credit (1%): Only explains one correctly
- No credit (0%): Incorrect explanation

---

## 4. Variadic Templates and Perfect Forwarding (15%)

### 4.1 (4%)

**Answer:**
```c++
template<typename T>
void print(T value) {
    std::cout << value << std::endl;
}

template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first << " ";
    print(rest...);
}
```

**Explanation:** This uses recursion. The base case prints a single value with newline. The recursive case prints the first value, a space, then recursively calls with remaining arguments.

**Grading Rubric:**
- Full credit (4%): Correct recursive implementation with proper base case
- Partial credit (2-3%): Minor syntax errors or missing space
- Partial credit (1%): Major errors but shows understanding
- No credit (0%): Incorrect approach

---

### 4.2 (3%)

**Answer:**
```c++
template<typename... Args>
void print(Args... args) {
    ((std::cout << args << " "), ...);
}
```

Or for newline at end:
```c++
template<typename... Args>
void print(Args... args) {
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}
```

**Explanation:** This is a comma fold expression that expands `((std::cout << args << " "), ...)` for each argument.

**Grading Rubric:**
- Full credit (3%): Correct fold expression
- Partial credit (1-2%): Syntax errors but understanding shown
- No credit (0%): Incorrect syntax

---

### 4.3 (4%)

**Answer:**
```c++
func(x);      // Output: lvalue: 42
func(42);     // Output: rvalue: 42
func(std::move(x));  // Output: rvalue: 42
```

**Explanation:**
- `func(x)` where `x` is an lvalue: `T = int&`, `std::forward` returns `int&`, calls lvalue version
- `func(42)` where `42` is an rvalue: `T = int`, `std::forward` returns `int&&`, calls rvalue version
- `func(std::move(x))`: `std::move(x)` is an rvalue, `T = int`, calls rvalue version

**Grading Rubric:**
- Full credit (4%): All three correct
- Partial credit (2-3%): Two correct
- Partial credit (1%): One correct
- No credit (0%): None correct

---

### 4.4 (4%)

**Answer:**

**Problem:** The parameter `arg` is always an lvalue inside the function, even if an rvalue was passed. This means `std::make_unique<T>(arg)` will always call the copy constructor, never the move constructor.

**Fixed version:**
```c++
template<typename T, typename Arg>
std::unique_ptr<T> factory(Arg&& arg) {
    return std::make_unique<T>(std::forward<Arg>(arg));
}
```

**Grading Rubric:**
- Full credit (4%): Correct problem identification and fix with forwarding reference
- Partial credit (2-3%): Correct fix but poor explanation, or vice versa
- Partial credit (1%): Shows partial understanding
- No credit (0%): Incorrect

---

## 5. Standard Library and Containers (12%)

### 5.1 (3%)

**Answer:**

| Feature | std::map | std::unordered_map |
|---------|----------|-------------------|
| Ordering | Sorted by key | No ordering |
| Lookup time | O(log n) | O(1) average, O(n) worst |
| Memory overhead | Lower | Higher (hash table) |

**Grading Rubric:**
- Full credit (3%): All six cells correct
- Partial credit (2%): 4-5 correct
- Partial credit (1%): 2-3 correct
- No credit (0%): 0-1 correct

---

### 5.2 (3%)

**Answer:**
```c++
std::cout << std::get<0>(t) << " ";   // 42
std::cout << std::get<1>(t) << " ";   // 3.14
std::cout << std::get<2>(t) << "\n";  // hello

auto [i, d, s] = t;
// i = 42, d = 3.14, s = "hello"
```

**Grading Rubric:**
- Full credit (3%): All values correct
- Partial credit (2%): 4-5 correct
- Partial credit (1%): 2-3 correct
- No credit (0%): 0-1 correct

---

### 5.3 (3%)

**Answer:**
```c++
// Frequent insertions at end, random access needed: std::vector
// Frequent insertions at both ends: std::deque
// Fixed size, stack-allocated, no heap overhead: std::array
// Frequent insertions/deletions in middle: std::list
```

**Grading Rubric:**
- Full credit (3%): All four correct
- Partial credit (2%): Three correct
- Partial credit (1%): One or two correct
- No credit (0%): None correct

---

### 5.4 (3%)

**Answer:**
- `std::string` owns its character data, manages memory, and can modify the string. Use when you need to own or modify string data.

- `std::string_view` is a non-owning view (pointer + length) into existing string data. Use for read-only access to string data without copying, especially in function parameters.

**Grading Rubric:**
- Full credit (3%): Correctly explains both and gives appropriate use cases
- Partial credit (2%): Correct explanation but missing use cases
- Partial credit (1%): Partial understanding
- No credit (0%): Incorrect

---

## 6. Concurrency (13%)

### 6.1 (3%)

**Answer:** 2000

**Explanation:** `std::atomic<int>` guarantees that the `++` operator is atomic. Two threads each incrementing 1000 times will correctly result in 2000, with no data races or lost updates.

**Grading Rubric:**
- Full credit (3%): Correct answer with understanding of atomicity
- Partial credit (1-2%): Correct answer but wrong reasoning
- No credit (0%): Incorrect answer

---

### 6.2 (4%)

**Answer:**
```c++
class SafeCounter {
    int value = 0;
    std::mutex mtx;
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mtx);
        ++value;
    }
    
    int get() {
        std::lock_guard<std::mutex> lock(mtx);
        return value;
    }
};
```

**Grading Rubric:**
- Full credit (4%): Correct use of lock_guard in both methods
- Partial credit (2-3%): Correct approach but minor syntax errors
- Partial credit (1%): Uses mutex but not lock_guard (manual lock/unlock)
- No credit (0%): Incorrect approach

---

### 6.3 (3%)

**Answer:**
1. **Exception safety**: If an exception is thrown after `mtx.lock()` but before `mtx.unlock()`, the lock_guard ensures the mutex is still unlocked via RAII.
2. **Cleaner code**: No need to remember to call `unlock()` in every code path (including early returns and exceptions).

**Grading Rubric:**
- Full credit (3%): At least one correct reason with good explanation
- Partial credit (1-2%): Partial understanding
- No credit (0%): Incorrect

---

### 6.4 (3%)

**Answer:**
`std::condition_variable` allows threads to wait for a condition to become true, and be notified when it might have changed.

**Real-world scenario:** Producer-consumer queue:
- Consumer threads wait on the condition variable when the queue is empty
- Producer threads notify (wake up) waiting consumers when they add items to the queue

**Grading Rubric:**
- Full credit (3%): Correct explanation with relevant scenario
- Partial credit (1-2%): Partial understanding
- No credit (0%): Incorrect

---

## 7. Type Traits and Concepts (15%)

### 7.1 (4%)

**Answer:**
```c++
std::is_integral<T>::value     // Checks: T is an integer type, Example: int, char, long
std::is_pointer<T>::value      // Checks: T is a pointer type, Example: int*, char*
std::is_same<T, U>::value      // Checks: T and U are the same type, Example: std::is_same<int, int32_t>
std::is_base_of<B, D>::value   // Checks: D is derived from B, Example: std::is_base_of<Base, Derived>
```

**Grading Rubric:**
- Full credit (4%): All four correct
- Partial credit (3%): Three correct
- Partial credit (2%): Two correct
- Partial credit (1%): One correct
- No credit (0%): None correct

---

### 7.2 (4%)

**Answer:**
```c++
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T> multiply(T a, T b) {
    return a * b;
}
```

Or with return type suffix:
```c++
template<typename T>
auto multiply(T a, T b) -> std::enable_if_t<std::is_integral_v<T>, T> {
    return a * b;
}
```

**Grading Rubric:**
- Full credit (4%): Correct syntax for enable_if_t with is_integral_v
- Partial credit (2-3%): Minor syntax errors
- Partial credit (1%): Shows understanding but major errors
- No credit (0%): Incorrect

---

### 7.3 (4%)

**Answer:**
```c++
template<std::integral T>
T add(T a, T b) {
    return a + b;
}
```

Or with `auto`:
```c++
auto add(std::integral auto a, std::integral auto b) {
    return a + b;
}
```

Or with requires clause:
```c++
template<typename T>
requires std::integral<T>
T add(T a, T b) {
    return a + b;
}
```

**Grading Rubric:**
- Full credit (4%): Any correct C++20 concepts syntax
- Partial credit (2-3%): Minor syntax errors
- Partial credit (1%): Shows understanding but incorrect syntax
- No credit (0%): Incorrect

---

### 7.4 (3%)

**Answer:**
```c++
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;
```

**Grading Rubric:**
- Full credit (3%): Correct concept definition using standard concepts
- Partial credit (1-2%): Minor syntax errors or close but incorrect
- No credit (0%): Incorrect

---

## 8. Ranges (Bonus 5%)

### 8.1 (2%)

**Answer:** 4, 16, 36 (the squares of the first 3 even numbers: 2², 4², 6²)

**Grading Rubric:**
- Full credit (2%): Correct values
- Partial credit (1%): Partially correct
- No credit (0%): Incorrect

---

### 8.2 (3%)

**Answer:** Operations are performed **lazily** (on demand).

**Explanation:** Views don't perform computation until you iterate over them. When you iterate `result`, each element goes through the pipeline one at a time: filter → transform → take. This matters because:
1. You can work with infinite sequences
2. You don't process elements you don't need
3. Better memory efficiency (no intermediate containers)

**Grading Rubric:**
- Full credit (3%): Correctly identifies lazy evaluation with good explanation
- Partial credit (1-2%): Identifies lazy but poor explanation
- No credit (0%): Incorrect

---

## 9. Smart Resource Manager (40%)

**Answer:**
```c++
class ResourceManager {
private:
    int* data_;
    size_t size_;

public:
    // Constructor
    explicit ResourceManager(size_t n = 0) : data_(n ? new int[n]() : nullptr), size_(n) {}
    
    // Destructor
    ~ResourceManager() {
        delete[] data_;
    }
    
    // Delete copy operations (unique ownership)
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    // Move constructor
    ResourceManager(ResourceManager&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    // Move assignment
    ResourceManager& operator=(ResourceManager&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Bounds-checked element access
    int& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data_[index];
    }
    
    const int& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data_[index];
    }
    
    // Unchecked element access
    int& operator[](size_t index) { return data_[index]; }
    const int& operator[](size_t index) const { return data_[index]; }
    
    // Size accessor
    size_t size() const { return size_; }
    
    // Iterator support for range-based for loops
    int* begin() { return data_; }
    int* end() { return data_ + size_; }
    const int* begin() const { return data_; }
    const int* end() const { return data_ + size_; }
};
```

**Grading Rubric:**
- (8%) Constructor with proper initialization
- (6%) Destructor correctly releasing memory
- (6%) Copy operations deleted
- (8%) Move constructor and move assignment correct
- (6%) Bounds-checked access (at method with exception)
- (4%) Iterator support for range-based for loops
- (2%) Other supporting methods (size, operator[])

**Deductions:**
- Missing `noexcept` on move operations: -2%
- Memory leak in move assignment: -4%
- Not checking self-assignment in move assignment: -2%
- Not nullifying source in move operations: -4%

---

## 10. Thread-Safe Message Queue (60%)

**Answer:**
```c++
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

template<typename T>
class MessageQueue {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;

public:
    // Adds a message to the queue
    void push(T message) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(message));
        }
        cv_.notify_one();  // Wake up one waiting consumer
    }
    
    // Removes and returns a message, blocks if empty
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait until queue is not empty
        cv_.wait(lock, [this]() { return !queue_.empty(); });
        
        T message = std::move(queue_.front());
        queue_.pop();
        return message;
    }
    
    // Tries to pop a message with timeout
    // Returns true if successful, false if timeout
    bool try_pop_for(T& message, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait until queue is not empty or timeout
        bool success = cv_.wait_for(lock, timeout, [this]() { 
            return !queue_.empty(); 
        });
        
        if (success) {
            message = std::move(queue_.front());
            queue_.pop();
            return true;
        }
        return false;
    }
    
    // Returns the current size
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
};
```

**Grading Rubric:**

**Data Members (6%):**
- (2%) `std::queue<T>` for storage
- (2%) `std::mutex` for synchronization
- (2%) `std::condition_variable` for blocking

**push Method (10%):**
- (4%) Correct mutex locking (lock_guard)
- (3%) Uses std::move for efficiency
- (3%) Correctly notifies condition variable (outside lock or inside)

**pop Method (14%):**
- (4%) Uses unique_lock (required for condition variable)
- (4%) Correct wait with predicate
- (4%) Uses std::move when returning
- (2%) Properly pops from queue

**try_pop_for Method (18%):**
- (4%) Uses unique_lock
- (4%) Correct wait_for with timeout
- (4%) Checks return value of wait_for
- (4%) Correctly handles success/failure cases
- (2%) Uses std::move

**size Method (6%):**
- (4%) Correct mutex locking
- (2%) Returns size correctly

**Additional Considerations:**
- (6%) Generic template implementation (works with any T)
- (Bonus) Handles edge cases properly

**Deductions:**
- Data race (missing lock): -10%
- Deadlock potential: -10%
- Not using std::move where beneficial: -3%
- Notifying all instead of one (acceptable but less efficient): No deduction
- Locking incorrectly (too broad/narrow scope): -5%