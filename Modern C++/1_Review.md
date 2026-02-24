# Review - Traditional C++

### 1 Lifetime Management

#### 1.1 static Storage Duration

The `static` keyword has multiple uses in C++ with different meanings depending on context:

##### Static Local Variables

Static local variables persist across function calls and are initialized only once:

```c++
void counter() {
    static int count = 0;  // Initialized only once, persists between calls
    count++;
    std::cout << "Count: " << count << "\n";
}

void demonstrateStaticLocal() {
    counter();  // Output: Count: 1
    counter();  // Output: Count: 2
    counter();  // Output: Count: 3
}

// Thread-safe initialization (C++11)
void threadSafeCounter() {
    static thread_local int count = 0;  // Each thread has its own copy
    count++;
    std::cout << "Thread " << std::this_thread::get_id() 
              << " count: " << count << "\n";
}
```

**Characteristics:**
- Initialized only once during program execution
- Lifetime extends for the entire program duration
- Memory is allocated in the static storage area
- Thread-safe initialization in C++11 and later

##### Static Member Variables

Static member variables are shared across all instances of a class:

```c++
class Widget {
private:
    static int instanceCount;     // Declaration (not definition)
    static const int MAX_SIZE = 100;  // Static const members can be initialized
    static constexpr double PI = 3.14159;  // C++11: constexpr static members
    int id;
    
public:
    Widget() : id(++instanceCount) {}
    
    static int getCount() { return instanceCount; }
    static void resetCount() { instanceCount = 0; }
    
    // Static member functions can only access static members
    static bool isTooMany() { return instanceCount > MAX_SIZE; }
};

// Definition and initialization (required in exactly one translation unit)
int Widget::instanceCount = 0;

// Usage
int main() {
    std::cout << "Initial count: " << Widget::getCount() << "\n";  // 0
    
    Widget w1, w2, w3;
    std::cout << "After creating 3 widgets: " << Widget::getCount() << "\n";  // 3
    
    Widget::resetCount();
    std::cout << "After reset: " << Widget::getCount() << "\n";  // 0
    
    std::cout << "Widget PI: " << Widget::PI << "\n";  // 3.14159
}
```

**Key points:**
- Must be declared in class and defined outside
- Shared among all class instances
- Can be accessed without object instantiation
- Static member functions can only access static members
- Useful for counting instances, configuration, or shared resources

##### Static Global Variables and Functions

Static global variables and functions have internal linkage (file scope):

```c++
// file1.cpp
static int fileLocalVar = 42;  // Only visible in this file
static void helperFunction() {  // Only callable from this file
    std::cout << "Helper function called\n";
}

// This function is visible to other translation units
void publicFunction() {
    helperFunction();  // OK: same translation unit
    std::cout << "File local var: " << fileLocalVar << "\n";
}

// file2.cpp
// extern int fileLocalVar;  // Error: not visible
// extern void helperFunction();  // Error: not visible

extern void publicFunction();  // OK: visible
```

**Benefits:**
- Encapsulation at file level
- Prevents name clashes between translation units
- Hides implementation details
- Enables internal helper functions

##### Static vs Other Storage Durations

```c++
void demonstrateStorageDurations() {
    // Automatic storage duration (default)
    int autoVar = 10;  // Destroyed when function exits
    
    // Static storage duration
    static int staticVar = 20;  // Persists for program lifetime
    
    // Dynamic storage duration
    int* dynamicVar = new int(30);  // Must be manually deleted
    delete dynamicVar;
    
    // Thread storage duration (C++11)
    thread_local int threadVar = 40;  // One per thread
}
```

**Storage duration comparison:**
- **Automatic**: Created on entry, destroyed on exit
- **Static**: Created at program start, destroyed at program end
- **Dynamic**: Created with `new`, destroyed with `delete`
- **Thread**: Created per thread, destroyed when thread exits

#### 1.2 extern (External Linkage)

The `extern` keyword declares that a variable or function has external linkage, meaning it's defined in another translation unit. This is essential for sharing global variables and functions across multiple source files.

##### extern Variables

`extern` allows you to declare a variable without defining it, indicating that the definition exists elsewhere:

```c++
// global.h
#ifndef GLOBAL_H
#define GLOBAL_H

extern int globalCounter;     // Declaration only
extern const double PI;       // Can be initialized in header for const
extern std::string appName;   // Declaration only

#endif

// global.cpp
#include "global.h"
#include <string>

int globalCounter = 0;                    // Definition
const double PI = 3.14159265359;          // Definition (const can be initialized)
std::string appName = "My Application";   // Definition

// main.cpp
#include "global.h"
#include <iostream>

void incrementCounter() {
    globalCounter++;  // Uses the variable defined in global.cpp
}

int main() {
    std::cout << "App: " << appName << "\n";
    std::cout << "PI: " << PI << "\n";
    incrementCounter();
    std::cout << "Counter: " << globalCounter << "\n";
    return 0;
}
```

##### extern Functions

Function declarations are `extern` by default, but you can explicitly use the keyword for clarity:

```c++
// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

extern int add(int a, int b);           // Explicit extern (optional)
extern double calculateArea(double radius);
extern void printVersion();

#endif

// math_utils.cpp
#include "math_utils.h"
#include <iostream>

int add(int a, int b) {
    return a + b;
}

double calculateArea(double radius) {
    extern const double PI;  // References external PI
    return PI * radius * radius;
}

void printVersion() {
    std::cout << "Math Utils v1.0\n";
}

// main.cpp
#include "math_utils.h"
#include "global.h"  // Contains extern const double PI

int main() {
    int result = add(5, 3);
    double area = calculateArea(5.0);
    
    std::cout << "5 + 3 = " << result << "\n";
    std::cout << "Area of circle (r=5): " << area << "\n";
    printVersion();
    
    return 0;
}
```

##### extern "C" (C Compatibility)

`extern "C"` prevents C++ name mangling, allowing C++ code to call C functions and vice versa:

```c++
// c_library.h (C header)
#ifndef C_LIBRARY_H
#define C_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

int c_function(int x);
void c_init();
void c_cleanup();

#ifdef __cplusplus
}
#endif

#endif

// c_library.c (C implementation)
#include "c_library.h"
#include <stdio.h>

int c_function(int x) {
    printf("C function called with %d\n", x);
    return x * 2;
}

void c_init() {
    printf("C library initialized\n");
}

void c_cleanup() {
    printf("C library cleaned up\n");
}

// cpp_wrapper.cpp (C++ wrapper)
#include "c_library.h"
#include <iostream>

class CppWrapper {
public:
    void useCFunction(int value) {
        int result = c_function(value);
        std::cout << "C++ wrapper got result: " << result << "\n";
    }
    
    void initialize() {
        c_init();
    }
    
    void cleanup() {
        c_cleanup();
    }
};

// main.cpp
#include "cpp_wrapper.cpp"

int main() {
    CppWrapper wrapper;
    wrapper.initialize();
    wrapper.useCFunction(42);
    wrapper.cleanup();
    return 0;
}
```

##### extern Templates (C++11)

`extern template` prevents template instantiation in the current translation unit, reducing compilation time and binary size:

```c++
// container.h
#ifndef CONTAINER_H
#define CONTAINER_H

template<typename T>
class Container {
private:
    T* data;
    size_t size;
public:
    Container(size_t s) : size(s), data(new T[s]) {}
    ~Container() { delete[] data; }
    
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    size_t getSize() const { return size; }
};

// Explicit instantiation declarations (prevent instantiation here)
extern template class Container<int>;
extern template class Container<double>;
extern template class Container<std::string>;

#endif

// container.cpp
#include "container.h"

// Explicit instantiation definitions (instantiation happens here)
template class Container<int>;
template class Container<double>;
template class Container<std::string>;

// main.cpp
#include "container.h"
#include <iostream>

int main() {
    Container<int> intContainer(10);
    Container<double> doubleContainer(5);
    
    intContainer[0] = 42;
    doubleContainer[0] = 3.14;
    
    std::cout << "Int container[0]: " << intContainer[0] << "\n";
    std::cout << "Double container[0]: " << doubleContainer[0] << "\n";
    
    return 0;
}
```

##### extern Variables Best Practices

```c++
// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Good: Use inline variables (C++17) for header-only definitions
inline const std::string VERSION = "1.0.0";
inline const int MAX_CONNECTIONS = 100;

// Good: Use extern for variables that need single definition
extern bool debugMode;
extern std::string applicationName;

// Good: Use static inline for compile-time constants
static inline constexpr double GRAVITY = 9.81;

#endif

// config.cpp
#include "config.h"

// Single definition in one translation unit
bool debugMode = false;
std::string applicationName = "MyApp";

// main.cpp
#include "config.h"
#include <iostream>

void setup() {
    debugMode = true;
    applicationName = "DebugApp";
}

int main() {
    setup();
    
    std::cout << "Version: " << VERSION << "\n";
    std::cout << "Debug mode: " << (debugMode ? "ON" : "OFF") << "\n";
    std::cout << "App name: " << applicationName << "\n";
    std::cout << "Gravity: " << GRAVITY << "\n";
    
    return 0;
}
```

##### extern and Linkage Rules

```c++
// linkage_example.h
#ifndef LINKAGE_EXAMPLE_H
#define LINKAGE_EXAMPLE_H

// Internal linkage (file scope)
static int fileLocalVar = 42;
static void fileLocalFunction() {
    std::cout << "File local function\n";
}

// External linkage
extern int globalVar;
extern void globalFunction();

// Anonymous namespace (C++ alternative to static)
namespace {
    int anonymousVar = 100;
    void anonymousFunction() {
        std::cout << "Anonymous namespace function\n";
    }
}

#endif

// linkage_example.cpp
#include "linkage_example.h"
#include <iostream>

// Definition of extern variables
int globalVar = 200;

void globalFunction() {
    std::cout << "Global function\n";
    std::cout << "File local var: " << fileLocalVar << "\n";
    std::cout << "Anonymous var: " << anonymousVar << "\n";
}

// main.cpp
#include "linkage_example.h"

int main() {
    std::cout << "Global var: " << globalVar << "\n";
    globalFunction();
    
    // fileLocalVar;  // Error: not accessible
    // fileLocalFunction();  // Error: not accessible
    // anonymousVar;  // Error: not accessible
    // anonymousFunction();  // Error: not accessible
    
    return 0;
}
```

### 2 Class Behavior

Modern C++ provides several keywords to control class behavior and improve safety, performance, and code clarity.

#### 2.1 `final`

The `final` keyword prevents inheritance from a class or overriding of virtual functions, enabling important optimizations and API design decisions.

##### final Classes

```c++
class Base {
public:
    virtual void foo() { std::cout << "Base::foo\n"; }
    virtual ~Base() = default;
};

class FinalClass final : public Base {  // Cannot be inherited from
public:
    void foo() override { std::cout << "FinalClass::foo\n"; }
};

// class SubClass : public FinalClass {};  // Error: FinalClass is final

// Usage example
void processBase(const Base& obj) {
    obj.foo();  // May require virtual dispatch
}

void processFinal(const FinalClass& obj) {
    obj.foo();  // Can be inlined - no virtual dispatch needed
}
```

##### final Member Functions

```c++
class Base {
public:
    virtual void foo() { std::cout << "Base::foo\n"; }
    virtual void bar() { std::cout << "Base::bar\n"; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void foo() final override { std::cout << "Derived::foo\n"; }  // Cannot be overridden further
    void bar() override { std::cout << "Derived::bar\n"; }
};

class FurtherDerived : public Derived {
public:
    // void foo() override;  // Error: foo is final in Derived
    void bar() override { std::cout << "FurtherDerived::bar\n"; }  // OK
};
```

##### API Design with final

```c++
// Design pattern: Template Method with final hook
class GameCharacter {
public:
    // Template method - final to prevent modification of algorithm
    void performAction() final {
        prepare();
        execute();
        cleanup();
    }
    
    virtual ~GameCharacter() = default;
    
protected:
    virtual void prepare() = 0;
    virtual void execute() = 0;
    virtual void cleanup() = 0;
};

class Warrior : public GameCharacter {
protected:
    void prepare() override { std::cout << "Warrior draws sword\n"; }
    void execute() override { std::cout << "Warrior attacks\n"; }
    void cleanup() override { std::cout << "Warrior sheathes sword\n"; }
};

class Mage final : public GameCharacter {  // Mage behavior is complete
protected:
    void prepare() override { std::cout << "Mage chants spell\n"; }
    void execute() override { std::cout << "Mage casts fireball\n"; }
    void cleanup() override { std::cout << "Mage lowers hands\n"; }
};
```

#### 2.2 `default`

The `default` keyword explicitly requests the compiler to generate default implementations for special member functions, providing better performance and clearer intent than user-defined equivalents.

##### When to Use default

```c++
// Case 1: When other constructors are defined
class Resource {
private:
    std::unique_ptr<int> data;
    std::string name;
public:
    Resource(const std::string& n) : name(n), data(std::make_unique<int>(0)) {}
    
    // Still need copy/move operations for container compatibility
    Resource(const Resource&) = default;
    Resource& operator=(const Resource&) = default;
    Resource(Resource&&) = default;
    Resource& operator=(Resource&&) = default;
};

// Case 2: When you want to ensure trivial operations
class Trivial {
public:
    int x, y;
    double z;
    
    // Explicitly default to ensure triviality
    Trivial() = default;
    Trivial(const Trivial&) = default;
    Trivial& operator=(const Trivial&) = default;
    ~Trivial() = default;
};

static_assert(std::is_trivial_v<Trivial>, "Trivial should be trivial");
```

#### 2.3 `delete`

The `delete` keyword explicitly prevents the use of functions or operators, enabling fine-grained control over class interfaces and preventing dangerous operations.

##### Preventing Copy Operations

```c++
class NonCopyable {
public:
    NonCopyable() = default;
    
    // Prevent copying
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    
    // Allow moving
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
    
    ~NonCopyable() = default;
};

// Usage
void demonstrateNonCopyable() {
    NonCopyable obj1;
    // NonCopyable obj2 = obj1;  // Error: copy constructor deleted
    // obj1 = obj1;              // Error: copy assignment deleted
    
    NonCopyable obj3 = std::move(obj1);  // OK: move constructor available
}
```

##### Preventing Dangerous Conversions

```c++
class SafeInteger {
private:
    int value;
public:
    SafeInteger(int v) : value(v) {}
    
    // Allow int conversion
    SafeInteger& operator=(int v) { value = v; return *this; }
    
    // Prevent dangerous floating-point conversions
    SafeInteger& operator=(double) = delete;
    SafeInteger& operator=(float) = delete;
    SafeInteger& operator=(long double) = delete;
    
    // Prevent pointer conversions
    SafeInteger& operator=(void*) = delete;
    SafeInteger& operator=(const char*) = delete;
};

void demonstrateSafeConversions() {
    SafeInteger si(42);
    si = 100;        // OK: int assignment
    // si = 3.14;     // Error: double assignment deleted
    // si = "test";   // Error: const char* assignment deleted
}
```

##### Template Specialization with delete

```c++
template<typename T>
class Container {
private:
    std::vector<T> data;
public:
    void add(const T& item) { data.push_back(item); }
    
    // Delete problematic specializations
    template<typename U = T>
    typename std::enable_if<std::is_pointer<U>::value>::type
    add(U item) = delete;  // Prevent pointer addition
};

// More modern approach with concepts (C++20)
template<typename T>
class ModernContainer {
public:
    void add(const T& item) { /* implementation */ }
    
    // Delete pointer types
    void add(T* item) = delete;
};
```

##### Preventing Array Operations

```c++
class NoArrays {
public:
    NoArrays() = default;
    
    // Prevent array operations that don't make sense
    void operator[](int) = delete;
    void operator[](size_t) = delete;
    void operator[](long) = delete;
};

void demonstrateNoArrays() {
    NoArrays obj;
    // obj[0];  // Error: operator[] deleted
}
```

#### 2.4 `explicit`

The `explicit` keyword prevents implicit conversions, making code safer and more predictable by requiring explicit type conversions.

##### Explicit Constructors

```c++
class String {
private:
    std::string data;
public:
    // Single-argument constructor should be explicit
    explicit String(const char* str) : data(str) {}
    
    // Multi-argument constructors don't need explicit (can't be implicit)
    String(const char* str, size_t len) : data(str, len) {}
    
    // Converting constructor (sometimes useful but dangerous)
    String(int size) : data(size, ' ') {}  // Creates string of spaces
};

void demonstrateExplicitConstructors() {
    String s1("hello");           // OK: explicit construction
    // String s2 = "world";       // Error: implicit conversion not allowed
    
    String s3("test", 4);         // OK: multi-argument, no implicit conversion possible
    String s4(10);                // OK: explicit construction
    // String s5 = 10;            // Error: implicit conversion not allowed
}
```

##### Explicit Conversion Operators

```c++
class SafeBool {
private:
    bool value;
public:
    SafeBool(bool v) : value(v) {}
    
    // Explicit conversion to bool
    explicit operator bool() const { return value; }
    
    // Explicit conversion to int (if needed)
    explicit operator int() const { return value ? 1 : 0; }
};

void demonstrateExplicitOperators() {
    SafeBool sb(true);
    
    if (sb) {                     // OK: explicit conversion in boolean context
        std::cout << "sb is true\n";
    }
    
    // int x = sb;                 // Error: implicit conversion not allowed
    int x = static_cast<int>(sb); // OK: explicit conversion
    bool b = static_cast<bool>(sb); // OK: explicit conversion
}
```

##### Explicit Template Conversions

```c++
template<typename T>
class Wrapper {
private:
    T value;
public:
    Wrapper(T v) : value(v) {}
    
    // Explicit conversion to other types
    template<typename U>
    explicit operator U() const {
        return static_cast<U>(value);
    }
};

void demonstrateExplicitTemplateConversions() {
    Wrapper<int> w(42);
    
    // double d = w;  // Error: implicit conversion not allowed
    double d = static_cast<double>(w);  // OK: explicit conversion
    std::string s = static_cast<std::string>(w);  // Error: no conversion to string
}
```

### 3 Explicit Type Casting

C++ provides specific casting operators for different type conversion scenarios: static_cast, const_cast, reinterpret_cast, dynamic_cast; each with its own use cases, safety guarantees, and performance characteristics.

#### 3.1 `static_cast`

`static_cast` performs compile-time type conversions between related types, providing type safety while allowing explicit conversions that the compiler cannot perform automatically.

##### Basic static_cast Usage

```c++
int main() {
    // Numeric conversions with potential data loss
    double d = 3.14159;
    int i = static_cast<int>(d);  // 3 - truncates decimal part
    float f = static_cast<float>(d);  // 3.14159f - may lose precision
    
    // Explicit narrowing conversions
    long long big = 1000000000000LL;
    int small = static_cast<int>(big);  // May overflow, but explicit
    
    // Boolean conversions
    int value = 42;
    bool b = static_cast<bool>(value);  // true (non-zero is true)
    int zero = 0;
    bool false_b = static_cast<bool>(zero);  // false
}
```

##### Pointer Conversions in Inheritance Hierarchies

```c++
class Base {
public:
    virtual ~Base() = default;
    virtual void baseMethod() { std::cout << "Base method\n"; }
};

class Derived : public Base {
public:
    void derivedMethod() { std::cout << "Derived method\n"; }
    void baseMethod() override { std::cout << "Derived base method\n"; }
};

void demonstratePointerCasts() {
    Derived derived;
    Base* basePtr = &derived;
    Derived* derivedPtr = &derived;
    
    // Safe upcast (implicit, but can be explicit)
    Base* upcast = static_cast<Base*>(derivedPtr);
    upcast->baseMethod();  // Calls Derived::baseMethod()
    
    // Downcast - potentially unsafe but allowed
    Derived* downcast = static_cast<Derived*>(basePtr);
    downcast->derivedMethod();  // Safe here because basePtr actually points to Derived
    
    // Unsafe downcast example
    Base base;
    Base* basePtr2 = &base;
    Derived* unsafeDowncast = static_cast<Derived*>(basePtr2);
    // unsafeDowncast->derivedMethod();  // Undefined behavior!
}
```

##### Explicit Constructor Calls and Conversions

```c++
class String {
private:
    std::string data;
public:
    String(const char* str) : data(str) {}
    String(const std::string& str) : data(str) {}
    operator std::string() const { return data; }
};

void demonstrateConstructorCasts() {
    // Explicit constructor calls
    std::string s1 = static_cast<std::string>("hello");
    String s2 = static_cast<String>("world");
    
    // Explicit conversion operator calls
    String customString("test");
    std::string stdString = static_cast<std::string>(customString);
}
```

##### Void Pointer Conversions

```c++
void demonstrateVoidPointerCasts() {
    int value = 42;
    void* voidPtr = &value;
    
    // Convert void* to typed pointer
    int* intPtr = static_cast<int*>(voidPtr);
    std::cout << "Value: " << *intPtr << "\n";
    
    // Array to void pointer and back
    double arr[10];
    void* arrPtr = static_cast<void*>(arr);
    double* typedArr = static_cast<double*>(arrPtr);
    typedArr[0] = 3.14;
}
```

##### Template-based static_cast

```c++
template<typename Target, typename Source>
Target safe_cast(Source source) {
    // Additional safety checks could go here
    return static_cast<Target>(source);
}

void demonstrateTemplateCasts() {
    double d = 3.14;
    int i = safe_cast<int>(d);
    std::cout << "Safe cast: " << i << "\n";
}
```

#### 3.2 const_cast

`const_cast` removes const and volatile qualifiers, allowing modification of objects that were declared as const or volatile. This should be used with extreme caution.

##### Basic const_cast Usage

```c++
void demonstrateBasicConstCast() {
    const int value = 42;
    const int* constPtr = &value;
    
    // Remove const qualifier
    int* mutablePtr = const_cast<int*>(constPtr);
    
    // WARNING: Undefined behavior if original object was truly const
    // *mutablePtr = 100;  // Dangerous!
    
    // Safe usage: when original object is not const
    int mutableValue = 42;
    const int* constPtr2 = &mutableValue;
    int* mutablePtr2 = const_cast<int*>(constPtr2);
    *mutablePtr2 = 100;  // Safe: original object is not const
    std::cout << "Modified value: " << mutableValue << "\n";
}
```

#### 3.3 reinterpret_cast

`reinterpret_cast` performs low-level bit reinterpretation between unrelated types, providing maximum flexibility but minimal safety. Use only when absolutely necessary for low-level programming.

##### Basic reinterpret_cast Usage

```c++
void demonstrateBasicReinterpretCast() {
    // Integer to pointer conversion
    int value = 42;
    uintptr_t addr = reinterpret_cast<uintptr_t>(&value);
    int* ptr = reinterpret_cast<int*>(addr);
    std::cout << "Reinterpreted value: " << *ptr << "\n";
    
    // Pointer to different pointer type
    int* intPtr = new int(42);
    double* doublePtr = reinterpret_cast<double*>(intPtr);
    // *doublePtr = 3.14;  // Dangerous: undefined behavior!
    delete intPtr;
}
```

#### 3.4 dynamic_cast

`dynamic_cast` provides safe downcasting in inheritance hierarchies by performing runtime type checking. It's the only cast that can safely determine an object's actual type at runtime.

##### Basic dynamic_cast Usage

```c++
class Base {
public:
    virtual ~Base() = default;
    virtual void baseMethod() { std::cout << "Base method\n"; }
    virtual std::string getType() const { return "Base"; }
};

class Derived : public Base {
public:
    void derivedMethod() { std::cout << "Derived method\n"; }
    std::string getType() const override { return "Derived"; }
};

class FurtherDerived : public Derived {
public:
    void furtherMethod() { std::cout << "Further derived method\n"; }
    std::string getType() const override { return "FurtherDerived"; }
};

void demonstrateDynamicCast() {
    Base* basePtr = new Derived();
    Base* basePtr2 = new FurtherDerived();
    Base* basePtr3 = new Base();
    
    // Safe downcast with pointer
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);
    if (derivedPtr) {
        derivedPtr->derivedMethod();
        std::cout << "Type: " << derivedPtr->getType() << "\n";
    }
    
    // Failed downcast returns nullptr
    Derived* derivedPtr2 = dynamic_cast<Derived*>(basePtr3);
    if (!derivedPtr2) {
        std::cout << "basePtr3 is not a Derived object\n";
    }
}
```

##### dynamic_cast with References

```c++
void demonstrateDynamicCastReferences() {
    Base base;
    Derived derived;
    
    try {
        // Successful cast
        Derived& derivedRef = dynamic_cast<Derived&>(derived);
        derivedRef.derivedMethod();
        
        // Failed cast throws std::bad_cast
        Derived& baseRef = dynamic_cast<Derived&>(base);
    } catch (const std::bad_cast& e) {
        std::cout << "Bad cast caught: " << e.what() << "\n";
    }
}
```

##### Runtime Type Information (RTTI)

```c++
void demonstrateRTTI() {
    std::vector<std::unique_ptr<Base>> objects;
    objects.push_back(std::make_unique<Base>());
    objects.push_back(std::make_unique<Derived>());
    objects.push_back(std::make_unique<FurtherDerived>());
    
    for (const auto& obj : objects) {
        // Check type at runtime
        if (dynamic_cast<Derived*>(obj.get())) {
            std::cout << "Object is a Derived\n";
            if (dynamic_cast<FurtherDerived*>(obj.get())) {
                std::cout << "Object is a FurtherDerived\n";
            }
        } else {
            std::cout << "Object is a Base\n";
        }
    }
}
```

**Key Principles for Type Casting:**

1. **Prefer static_cast**: For compile-time safe conversions between related types
2. **Use const_cast sparingly**: Only when you know the original object is not const
3. **Avoid reinterpret_cast**: Use only for low-level programming with full understanding of risks
4. **Use dynamic_cast for polymorphism**: Safe downcasting in inheritance hierarchies
5. **Consider virtual functions**: Often better than casting for polymorphic behavior

### 4 Multiple Inheritance and The Diamond Problem

Multiple inheritance allows a class to inherit from multiple base classes, but introduces complexity in terms of ambiguity resolution and memory layout.

#### 4.1 Multiple Inheritance

Multiple inheritance enables a class to inherit functionality from multiple base classes, allowing for more flexible and reusable code design.

##### Simple Multiple Inheritance Example

```c++
class Animal {
protected:
    std::string name;
    int age;
public:
    Animal(const std::string& n, int a) : name(n), age(a) {}
    virtual ~Animal() = default;
    
    virtual void makeSound() = 0;
    virtual void move() = 0;
    
    const std::string& getName() const { return name; }
    int getAge() const { return age; }
};

class Flyable {
protected:
    double maxAltitude;
    double wingSpan;
public:
    Flyable(double altitude, double wings) : maxAltitude(altitude), wingSpan(wings) {}
    virtual ~Flyable() = default;
    
    virtual void fly() = 0;
    virtual void land() = 0;
    
    double getMaxAltitude() const { return maxAltitude; }
    double getWingSpan() const { return wingSpan; }
};

class Swimmable {
protected:
    double maxDepth;
    double swimSpeed;
public:
    Swimmable(double depth, double speed) : maxDepth(depth), swimSpeed(speed) {}
    virtual ~Swimmable() = default;
    
    virtual void swim() = 0;
    virtual void dive() = 0;
    
    double getMaxDepth() const { return maxDepth; }
    double getSwimSpeed() const { return swimSpeed; }
};

// Multiple inheritance - inherits from all three base classes
class Duck : public Animal, public Flyable, public Swimmable {
private:
    bool isMigratory;
public:
    Duck(const std::string& n, int a, double altitude, double wings, 
         double depth, double speed, bool migratory = false)
        : Animal(n, a), Flyable(altitude, wings), Swimmable(depth, speed), 
          isMigratory(migratory) {}
    
    // Implement pure virtual functions from all base classes
    void makeSound() override { std::cout << name << " quacks\n"; }
    void move() override { std::cout << name << " waddles\n"; }
    void fly() override { std::cout << name << " is flying at " << maxAltitude << "m\n"; }
    void land() override { std::cout << name << " is landing gracefully\n"; }
    void swim() override { std::cout << name << " is swimming at " << swimSpeed << " km/h\n"; }
    void dive() override { std::cout << name << " is diving to " << maxDepth << "m\n"; }
    
    bool isMigratoryBird() const { return isMigratory; }
    void setMigratory(bool migratory) { isMigratory = migratory; }
};
```

##### Polymorphic Usage

```c++
void demonstratePolymorphicUsage() {
    Duck duck("Donald", 3, 1000.0, 1.2, 5.0, 8.0, true);
    
    // Can be treated as any of its base types
    Animal* animal = &duck;
    Flyable* flyer = &duck;
    Swimmable* swimmer = &duck;
    
    // Each interface provides different functionality
    animal->makeSound();  // "Donald quacks"
    flyer->fly();          // "Donald is flying at 1000m"
    swimmer->swim();       // "Donald is swimming at 8 km/h"
    
    // Access specific functionality
    if (duck.isMigratoryBird()) {
        std::cout << "This duck is migratory\n";
    }
}
```

##### Interface Segregation with Multiple Inheritance

```c++
// Define clear interfaces for different capabilities
class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw() const = 0;
    virtual void setColor(const std::string& color) = 0;
};

class Serializable {
public:
    virtual ~Serializable() = default;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

class Movable {
public:
    virtual ~Movable() = default;
    virtual void move(double dx, double dy) = 0;
    virtual void getPosition(double& x, double& y) const = 0;
};

// A class that implements multiple interfaces
class GameObject : public Drawable, public Serializable, public Movable {
private:
    double x, y;
    std::string color;
    std::string name;
public:
    GameObject(double x, double y, const std::string& color, const std::string& name)
        : x(x), y(y), color(color), name(name) {}
    
    // Drawable interface
    void draw() const override {
        std::cout << "Drawing " << name << " at (" << x << ", " << y << ") with color " << color << "\n";
    }
    
    void setColor(const std::string& newColor) override {
        color = newColor;
    }
    
    // Serializable interface
    std::string serialize() const override {
        return name + ":" + std::to_string(x) + ":" + std::to_string(y) + ":" + color;
    }
    
    void deserialize(const std::string& data) override {
        // Simple parsing logic (in real code, use proper parsing)
        size_t pos1 = data.find(':');
        size_t pos2 = data.find(':', pos1 + 1);
        size_t pos3 = data.find(':', pos2 + 1);
        
        name = data.substr(0, pos1);
        x = std::stod(data.substr(pos1 + 1, pos2 - pos1 - 1));
        y = std::stod(data.substr(pos2 + 1, pos3 - pos2 - 1));
        color = data.substr(pos3 + 1);
    }
    
    // Movable interface
    void move(double dx, double dy) override {
        x += dx;
        y += dy;
    }
    
    void getPosition(double& outX, double& outY) const override {
        outX = x;
        outY = y;
    }
};
```

#### 4.2 The Diamond Problem

The diamond problem occurs when a class inherits from two classes that both inherit from the same base class, creating ambiguity about which base class members to use.

##### Diamond Problem Demonstration

```c++
class Animal {
public:
    int age;
    std::string species;
    
    Animal(int a, const std::string& s) : age(a), species(s) {
        std::cout << "Animal constructor: " << species << "\n";
    }
    
    virtual ~Animal() {
        std::cout << "Animal destructor: " << species << "\n";
    }
    
    virtual void breathe() {
        std::cout << species << " is breathing\n";
    }
};

class Mammal : public Animal {
public:
    bool hasFur;
    
    Mammal(int a, const std::string& s, bool fur) 
        : Animal(a, s), hasFur(fur) {
        std::cout << "Mammal constructor: " << species << "\n";
    }
    
    ~Mammal() {
        std::cout << "Mammal destructor: " << species << "\n";
    }
    
    void giveBirth() {
        std::cout << species << " gives birth to live young\n";
    }
};

class Bird : public Animal {
public:
    double wingSpan;
    
    Bird(int a, const std::string& s, double wings) 
        : Animal(a, s), wingSpan(wings) {
        std::cout << "Bird constructor: " << species << "\n";
    }
    
    ~Bird() {
        std::cout << "Bird destructor: " << species << "\n";
    }
    
    void layEggs() {
        std::cout << species << " lays eggs\n";
    }
};

// Diamond inheritance - inherits from both Mammal and Bird
class Bat : public Mammal, public Bird {
public:
    Bat(int a, const std::string& s, bool fur, double wings) 
        : Mammal(a, s, fur), Bird(a, s, wings) {
        std::cout << "Bat constructor: " << species << "\n";
    }
    
    ~Bat() {
        std::cout << "Bat destructor: " << species << "\n";
    }
    
    void printInfo() {
        // Ambiguity: which Animal::age and Animal::species?
        // std::cout << "Age: " << age << "\n";  // Error: ambiguous
        // std::cout << "Species: " << species << "\n";  // Error: ambiguous
        
        // Must specify which base class
        std::cout << "Age: " << Mammal::age << "\n";
        std::cout << "Species: " << Mammal::species << "\n";
        std::cout << "Has fur: " << (hasFur ? "Yes" : "No") << "\n";
        std::cout << "Wing span: " << wingSpan << "m\n";
    }
};
```

##### Problems with Diamond Inheritance

```c++
void demonstrateDiamondProblems() {
    // Problem 1: Constructor call ambiguity
    Bat bat(5, "Bat", true, 0.5);
    bat.printInfo();
    
    // Problem 2: Destructor call ambiguity
    // When bat goes out of scope, both Mammal and Bird destructors are called,
    // which both call Animal destructor - potential double destruction
    
    // Problem 3: Member access ambiguity
    // bat.age;  // Error: which Animal::age?
    // bat.species;  // Error: which Animal::species?
}
```

#### 4.3 Virtual Inheritance Solution

Virtual inheritance ensures that only one instance of the virtual base class exists in the inheritance hierarchy, eliminating the diamond problem.

##### Virtual Inheritance Implementation

```c++
class Animal {
public:
    int age;
    std::string species;
    
    Animal(int a, const std::string& s) : age(a), species(s) {
        std::cout << "Animal constructor: " << species << "\n";
    }
    
    virtual ~Animal() {
        std::cout << "Animal destructor: " << species << "\n";
    }
    
    virtual void breathe() {
        std::cout << species << " is breathing\n";
    }
};

class Mammal : virtual public Animal {  // Virtual inheritance
public:
    bool hasFur;
    
    Mammal(int a, const std::string& s, bool fur) 
        : Animal(a, s), hasFur(fur) {  // Animal constructor called explicitly
        std::cout << "Mammal constructor: " << species << "\n";
    }
    
    ~Mammal() {
        std::cout << "Mammal destructor: " << species << "\n";
    }
    
    void giveBirth() {
        std::cout << species << " gives birth to live young\n";
    }
};

class Bird : virtual public Animal {  // Virtual inheritance
public:
    double wingSpan;
    
    Bird(int a, const std::string& s, double wings) 
        : Animal(a, s), wingSpan(wings) {  // Animal constructor called explicitly
        std::cout << "Bird constructor: " << species << "\n";
    }
    
    ~Bird() {
        std::cout << "Bird destructor: " << species << "\n";
    }
    
    void layEggs() {
        std::cout << species << " lays eggs\n";
    }
};

// Virtual inheritance eliminates the diamond problem
class Bat : public Mammal, public Bird {
public:
    Bat(int a, const std::string& s, bool fur, double wings) 
        : Animal(a, s), Mammal(a, s, fur), Bird(a, s, wings) {  // Animal constructor called once
        std::cout << "Bat constructor: " << species << "\n";
    }
    
    ~Bat() {
        std::cout << "Bat destructor: " << species << "\n";
    }
    
    void printInfo() {
        // No ambiguity: only one Animal instance
        std::cout << "Age: " << age << "\n";  // OK: unambiguous
        std::cout << "Species: " << species << "\n";  // OK: unambiguous
        std::cout << "Has fur: " << (hasFur ? "Yes" : "No") << "\n";
        std::cout << "Wing span: " << wingSpan << "m\n";
    }
};
```

**When to use virtual inheritance:**

- When you have a true "is-a" relationship with shared base
- When the base class represents a fundamental interface or capability
- When you want to ensure single inheritance of base functionality

**Virtual Inheritance Characteristics:**

- **Single Instance**: Only one instance of the virtual base class exists
- **Constructor Responsibility**: Most derived class must initialize virtual base
- **Memory Overhead**: Virtual base pointer adds small overhead
- **Access Efficiency**: Member access may be slightly slower due to indirection
- **Destructor Order**: Virtual base destructor called last

### 5 Memory Layout

Understanding how objects are laid out in memory is crucial for performance optimization, debugging, and low-level programming. C++ memory layout follows specific rules for alignment, padding, and inheritance.

#### 5.1 Basic Object Layout

C++ objects are laid out in memory according to specific rules that ensure proper alignment and efficient access.

##### Simple Class Layout

```c++
class Simple {
    int a;      // Offset 0, size 4 bytes
    char b;     // Offset 4, size 1 byte
    double c;   // Offset 8 (after 7 bytes padding), size 8 bytes
    // Total size: 16 bytes (8 bytes padding after b)
};

class WithVirtual {
    int a;              // Offset 0, size 4 bytes
    virtual void foo() {}  // Adds vptr
    // vptr: pointer to virtual function table (typically 8 bytes on 64-bit)
    // Total size: 16 bytes (4 bytes padding after int)
};

class Empty {
    // Size is 1 byte (guarantees unique address for each object)
};

void demonstrateBasicLayout() {
    std::cout << "Simple: " << sizeof(Simple) << " bytes\n";
    std::cout << "WithVirtual: " << sizeof(WithVirtual) << " bytes\n";
    std::cout << "Empty: " << sizeof(Empty) << " bytes\n";
    
    // Memory layout visualization
    Simple s;
    std::cout << "Simple object address: " << &s << "\n";
    std::cout << "Member a address: " << &s.a << " (offset: " << offsetof(Simple, a) << ")\n";
    std::cout << "Member b address: " << &s.b << " (offset: " << offsetof(Simple, b) << ")\n";
    std::cout << "Member c address: " << &s.c << " (offset: " << offsetof(Simple, c) << ")\n";
}
```

##### Memory Alignment and Padding

```c++
class AlignmentExample {
    char a;     // Offset 0, size 1 byte, alignment 1
    int b;      // Offset 4 (after 3 bytes padding), size 4 bytes, alignment 4
    char c;     // Offset 8, size 1 byte, alignment 1
    // 3 bytes padding to align to 4-byte boundary
    // Total size: 12 bytes
};

class ComplexAlignment {
    char a;         // Offset 0, size 1 byte, alignment 1
    double b;       // Offset 8 (after 7 bytes padding), size 8 bytes, alignment 8
    int c;          // Offset 16, size 4 bytes, alignment 4
    char d;         // Offset 20, size 1 byte, alignment 1
    // 3 bytes padding to align to 4-byte boundary
    // Total size: 24 bytes
};

void demonstrateAlignment() {
    std::cout << "AlignmentExample: " << sizeof(AlignmentExample) << " bytes\n";
    std::cout << "ComplexAlignment: " << sizeof(ComplexAlignment) << " bytes\n";
    
    // Check alignment requirements
    std::cout << "char alignment: " << alignof(char) << "\n";
    std::cout << "int alignment: " << alignof(int) << "\n";
    std::cout << "double alignment: " << alignof(double) << "\n";
    std::cout << "AlignmentExample alignment: " << alignof(AlignmentExample) << "\n";
}
```

#### 5.2 Data Member Ordering and Optimization

The order of data members affects object size due to alignment requirements.

##### Optimal Member Ordering

```c++
// Suboptimal ordering - lots of padding
class Suboptimal {
    char a;     // 1 byte, offset 0
    int b;      // 4 bytes, offset 4 (3 bytes padding)
    char c;     // 1 byte, offset 8
    double d;   // 8 bytes, offset 16 (7 bytes padding)
    // Total: 24 bytes (10 bytes padding)
};

// Optimal ordering - minimal padding
class Optimal {
    double d;   // 8 bytes, offset 0
    int b;      // 4 bytes, offset 8
    char a;     // 1 byte, offset 12
    char c;     // 1 byte, offset 13
    // 6 bytes padding to align to 8-byte boundary
    // Total: 24 bytes (6 bytes padding)
};

// Even better ordering - no padding
class Best {
    double d;   // 8 bytes, offset 0
    int b;      // 4 bytes, offset 8
    // 4 bytes padding here for alignment
    char a;     // 1 byte, offset 16
    char c;     // 1 byte, offset 17
    // 6 bytes padding to align to 8-byte boundary
    // Total: 24 bytes (10 bytes padding)
};

// Actually best - group by size
class ActuallyBest {
    double d;   // 8 bytes, offset 0
    int b;      // 4 bytes, offset 8
    char a;     // 1 byte, offset 12
    char c;     // 1 byte, offset 13
    // 2 bytes padding to align to 4-byte boundary
    // Total: 16 bytes (2 bytes padding)
};
```

##### Memory Layout Analysis Tools

```c++
template<typename T>
void analyzeMemoryLayout() {
    std::cout << "Type: " << typeid(T).name() << "\n";
    std::cout << "Size: " << sizeof(T) << " bytes\n";
    std::cout << "Alignment: " << alignof(T) << " bytes\n";
    std::cout << "Padding analysis:\n";
    
    // This is a simplified analysis - real tools would be more comprehensive
    T obj;
    char* base = reinterpret_cast<char*>(&obj);
    char* current = base;
    
    // Analyze member offsets (simplified for demonstration)
    std::cout << "  Base address: " << static_cast<void*>(base) << "\n";
    std::cout << "\n";
}

void demonstrateMemoryAnalysis() {
    analyzeMemoryLayout<Suboptimal>();
    analyzeMemoryLayout<Optimal>();
    analyzeMemoryLayout<ActuallyBest>();
}
```

#### 5.3 Inheritance Layout

Inheritance affects memory layout through base class subobjects and virtual function tables.

##### Single Inheritance Layout

```c++
class Base {
    int baseData;
    char baseChar;
public:
    virtual void baseFunc() {}
    virtual ~Base() = default;
};

class Derived : public Base {
    int derivedData;
    char derivedChar;
public:
    void derivedFunc() {}
    void baseFunc() override {}  // Override virtual function
};

void demonstrateInheritanceLayout() {
    std::cout << "Base size: " << sizeof(Base) << " bytes\n";
    std::cout << "Derived size: " << sizeof(Derived) << " bytes\n";
    
    Derived d;
    Base* basePtr = &d;
    
    std::cout << "Derived object layout:\n";
    std::cout << "  Base subobject at: " << static_cast<void*>(basePtr) << "\n";
    std::cout << "  Derived data at: " << &d.derivedData << "\n";
    std::cout << "  Offset of derivedData: " << offsetof(Derived, derivedData) << "\n";
}
```

##### Multiple Inheritance Layout

```c++
class Base1 {
    int data1;
public:
    virtual void func1() {}
};

class Base2 {
    int data2;
public:
    virtual void func2() {}
};

class MultipleDerived : public Base1, public Base2 {
    int derivedData;
public:
    void func1() override {}
    void func2() override {}
};

void demonstrateMultipleInheritanceLayout() {
    std::cout << "Base1 size: " << sizeof(Base1) << " bytes\n";
    std::cout << "Base2 size: " << sizeof(Base2) << " bytes\n";
    std::cout << "MultipleDerived size: " << sizeof(MultipleDerived) << " bytes\n";
    
    MultipleDerived md;
    Base1* b1 = &md;
    Base2* b2 = &md;
    
    std::cout << "MultipleDerived object layout:\n";
    std::cout << "  Base1 subobject at: " << static_cast<void*>(b1) << "\n";
    std::cout << "  Base2 subobject at: " << static_cast<void*>(b2) << "\n";
    std::cout << "  Derived data at: " << &md.derivedData << "\n";
    std::cout << "  Offset of derivedData: " << offsetof(MultipleDerived, derivedData) << "\n";
}
```

#### 5.4 Virtual Function Table (vtable) Layout

Virtual functions are implemented using a virtual function table (vtable) that contains pointers to the appropriate function implementations.

##### vtable Structure Analysis

```c++
class Base {
public:
    virtual void func1() { std::cout << "Base::func1\n"; }
    virtual void func2() { std::cout << "Base::func2\n"; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void func1() override { std::cout << "Derived::func1\n"; }
    // func2 inherited from Base
};

class FurtherDerived : public Derived {
public:
    void func2() override { std::cout << "FurtherDerived::func2\n"; }
};

void demonstrateVtableLayout() {
    std::cout << "Base vtable structure:\n";
    std::cout << "  [0] Base::func1\n";
    std::cout << "  [1] Base::func2\n";
    std::cout << "  [2] Base::~Base\n";
    
    std::cout << "\nDerived vtable structure:\n";
    std::cout << "  [0] Derived::func1 (overrides Base::func1)\n";
    std::cout << "  [1] Base::func2 (inherited)\n";
    std::cout << "  [2] Derived::~Derived (calls Base::~Base)\n";
    
    std::cout << "\nFurtherDerived vtable structure:\n";
    std::cout << "  [0] Derived::func1 (inherited)\n";
    std::cout << "  [1] FurtherDerived::func2 (overrides Base::func2)\n";
    std::cout << "  [2] FurtherDerived::~FurtherDerived (calls Derived::~Derived)\n";
}
```

##### vtable Memory Layout

```c++
// Note: This is implementation-specific and may not work on all compilers
class VtableLayout {
public:
    virtual void func1() {}
    virtual void func2() {}
    virtual ~VtableLayout() = default;
};

void demonstrateVtableMemoryLayout() {
    VtableLayout obj;
    
    // Access vtable pointer (implementation-specific)
    void** vptr = *reinterpret_cast<void***>(&obj);
    
    std::cout << "Object address: " << &obj << "\n";
    std::cout << "Vtable pointer: " << vptr << "\n";
    std::cout << "Vtable entries:\n";
    std::cout << "  [0] " << vptr[0] << " (func1)\n";
    std::cout << "  [1] " << vptr[1] << " (func2)\n";
    std::cout << "  [2] " << vptr[2] << " (destructor)\n";
}
```

### 6 Virtual Table (vtable) and Virtual Function Mechanism

Virtual functions enable polymorphism through a runtime dispatch mechanism implemented via virtual function tables (vtables). Understanding this mechanism is crucial for performance optimization and debugging.

#### 6.1 How vtable Works

The vtable mechanism allows the correct function to be called based on the actual object type at runtime, not the static type of the pointer or reference.

##### Basic vtable Implementation

```c++
class Shape {
protected:
    double x, y;
public:
    Shape(double x, double y) : x(x), y(y) {}
    virtual ~Shape() = default;
    
    virtual double area() const = 0;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    
    void move(double dx, double dy) {
        x += dx; y += dy;
    }
    
    void printInfo() const {
        std::cout << "Type: " << getType() << ", Area: " << area() 
                  << ", Position: (" << x << ", " << y << ")\n";
    }
};

class Circle : public Shape {
    double radius;
public:
    Circle(double x, double y, double r) : Shape(x, y), radius(r) {}
    
    double area() const override {
        return 3.14159 * radius * radius;
    }
    
    void draw() const override {
        std::cout << "Drawing circle at (" << x << ", " << y 
                  << ") with radius " << radius << "\n";
    }
    
    std::string getType() const override {
        return "Circle";
    }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double x, double y, double w, double h) 
        : Shape(x, y), width(w), height(h) {}
    
    double area() const override {
        return width * height;
    }
    
    void draw() const override {
        std::cout << "Drawing rectangle at (" << x << ", " << y 
                  << ") with width " << width << " and height " << height << "\n";
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
};

class Triangle : public Shape {
    double base, height;
public:
    Triangle(double x, double y, double b, double h) 
        : Shape(x, y), base(b), height(h) {}
    
    double area() const override {
        return 0.5 * base * height;
    }
    
    void draw() const override {
        std::cout << "Drawing triangle at (" << x << ", " << y 
                  << ") with base " << base << " and height " << height << "\n";
    }
    
    std::string getType() const override {
        return "Triangle";
    }
};
```

##### vtable Structure Visualization

```c++
void demonstrateVtableStructure() {
    std::cout << "Shape vtable:\n";
    std::cout << "  [0] Shape::area (pure virtual - may be nullptr)\n";
    std::cout << "  [1] Shape::draw (pure virtual - may be nullptr)\n";
    std::cout << "  [2] Shape::getType (pure virtual - may be nullptr)\n";
    std::cout << "  [3] Shape::~Shape\n";
    
    std::cout << "\nCircle vtable:\n";
    std::cout << "  [0] Circle::area\n";
    std::cout << "  [1] Circle::draw\n";
    std::cout << "  [2] Circle::getType\n";
    std::cout << "  [3] Circle::~Circle (calls Shape::~Shape)\n";
    
    std::cout << "\nRectangle vtable:\n";
    std::cout << "  [0] Rectangle::area\n";
    std::cout << "  [1] Rectangle::draw\n";
    std::cout << "  [2] Rectangle::getType\n";
    std::cout << "  [3] Rectangle::~Rectangle (calls Shape::~Shape)\n";
    
    std::cout << "\nTriangle vtable:\n";
    std::cout << "  [0] Triangle::area\n";
    std::cout << "  [1] Triangle::draw\n";
    std::cout << "  [2] Triangle::getType\n";
    std::cout << "  [3] Triangle::~Triangle (calls Shape::~Shape)\n";
}
```

#### 6.2 Runtime Dispatch Mechanism

The virtual function call mechanism involves several steps at runtime to determine the correct function to call.

##### Virtual Function Call Process

```c++
void demonstrateRuntimeDispatch() {
    // Create objects
    Circle circle(0, 0, 5.0);
    Rectangle rect(10, 10, 4.0, 6.0);
    Triangle tri(20, 20, 8.0, 6.0);
    
    // Array of base class pointers
    Shape* shapes[] = {&circle, &rect, &tri};
    
    std::cout << "Runtime dispatch demonstration:\n";
    for (Shape* shape : shapes) {
        std::cout << "\nCalling virtual functions on " << shape->getType() << ":\n";
        
        // Each call involves:
        // 1. Access object's vptr
        // 2. Look up function pointer in vtable
        // 3. Call the function through the pointer
        shape->draw();
        std::cout << "Area: " << shape->area() << "\n";
        shape->printInfo();
    }
}
```

#### 6.3 Pure Virtual Functions and Abstract Classes

Pure virtual functions define interfaces that derived classes must implement, creating abstract base classes that cannot be instantiated directly.

##### Abstract Base Class Design

```c++
class AbstractBase {
public:
    virtual void pureVirtual() = 0;  // Pure virtual
    virtual void regularVirtual() {  // Regular virtual
        std::cout << "AbstractBase::regularVirtual\n";
    }
    virtual ~AbstractBase() = default;
};

class Concrete : public AbstractBase {
public:
    void pureVirtual() override {  // Must implement
        std::cout << "Concrete::pureVirtual\n";
    }
};

class PartiallyImplemented : public AbstractBase {
public:
    void pureVirtual() override {
        std::cout << "PartiallyImplemented::pureVirtual\n";
    }
    // Still inherits regularVirtual from AbstractBase
};

void demonstrateAbstractClasses() {
    // AbstractBase obj;  // Error: cannot instantiate abstract class
    Concrete obj;  // OK
    obj.pureVirtual();
    obj.regularVirtual();
    
    PartiallyImplemented partial;
    partial.pureVirtual();
    partial.regularVirtual();
}
```

##### Interface Design with Pure Virtual Functions

```c++
// Define clear interfaces using pure virtual functions
class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw() const = 0;
    virtual void setColor(const std::string& color) = 0;
    virtual std::string getColor() const = 0;
};

class Serializable {
public:
    virtual ~Serializable() = default;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

class Shape : public Drawable, public Serializable {
protected:
    double x, y;
    std::string color;
public:
    Shape(double x, double y, const std::string& color) : x(x), y(y), color(color) {}
    virtual ~Shape() = default;
    
    // Drawable interface
    void setColor(const std::string& newColor) override { color = newColor; }
    std::string getColor() const override { return color; }
    
    // Serializable interface
    std::string serialize() const override {
        return std::to_string(x) + ":" + std::to_string(y) + ":" + color;
    }
    void deserialize(const std::string& data) override {
        // Simple parsing implementation
        size_t pos1 = data.find(':');
        size_t pos2 = data.find(':', pos1 + 1);
        x = std::stod(data.substr(0, pos1));
        y = std::stod(data.substr(pos1 + 1, pos2 - pos1 - 1));
        color = data.substr(pos2 + 1);
    }
};
```

##### Template Method Pattern with Virtual Functions

```c++
class GameCharacter {
public:
    // Template method - defines algorithm structure
    void performAction() {
        prepare();
        execute();
        cleanup();
    }
    
    virtual ~GameCharacter() = default;
    
protected:
    // Pure virtual methods that subclasses must implement
    virtual void prepare() = 0;
    virtual void execute() = 0;
    virtual void cleanup() = 0;
};

class Warrior : public GameCharacter {
protected:
    void prepare() override { std::cout << "Warrior draws sword\n"; }
    void execute() override { std::cout << "Warrior attacks with sword\n"; }
    void cleanup() override { std::cout << "Warrior sheathes sword\n"; }
};

class Mage : public GameCharacter {
protected:
    void prepare() override { std::cout << "Mage chants spell\n"; }
    void execute() override { std::cout << "Mage casts fireball\n"; }
    void cleanup() override { std::cout << "Mage lowers hands\n"; }
};

void demonstrateTemplateMethod() {
    std::vector<std::unique_ptr<GameCharacter>> characters;
    characters.push_back(std::make_unique<Warrior>());
    characters.push_back(std::make_unique<Mage>());
    
    for (auto& character : characters) {
        character->performAction();
        std::cout << "\n";
    }
}
```

**Key Principles for Virtual Functions:**
1. **Use virtual functions for polymorphism**: When you need runtime type-based behavior
2. **Prefer pure virtual for interfaces**: Define clear contracts that must be implemented
3. **Always make base class destructors virtual**: Prevent undefined behavior during cleanup
4. **Use override keyword**: Improves code clarity and catches errors at compile time
5. **Consider performance implications**: Virtual calls have overhead compared to direct calls
6. **Use final for optimization**: Prevent unnecessary virtual dispatch when inheritance isn't needed
7. **Design with inheritance hierarchies in mind**: Plan your class hierarchy carefully
8. **Consider alternatives**: CRTP, std::variant, or function pointers for zero-cost abstractions

### 7 Summary and Best Practices

**Lifetime Management:**
- `static`: Controls storage duration and scope
- `mutable`: Allows modification of const objects for implementation details

**Class Behavior Control:**
- `final`: Prevents inheritance and enables optimizations
- `default`: Explicitly requests compiler-generated functions
- `delete`: Prevents dangerous operations and conversions
- `explicit`: Prevents implicit conversions

**Type Safety and Casting:**
- `static_cast`: Safe compile-time conversions between related types
- `const_cast`: Removes const/volatile qualifiers (use with extreme caution)
- `reinterpret_cast`: Low-level bit reinterpretation (use only when necessary)
- `dynamic_cast`: Safe runtime type checking for polymorphic types

**Inheritance and Memory Layout:**
- Multiple inheritance enables flexible interface design
- Virtual inheritance solves the diamond problem
- Memory layout follows alignment and padding rules
- Virtual function tables enable runtime polymorphism

##### 1. Memory Management

```c++
// Prefer RAII and smart pointers
class Resource {
    std::unique_ptr<int> data;
    std::shared_ptr<std::string> sharedData;
public:
    Resource() : data(std::make_unique<int>(42)) {}
    // Automatic cleanup through destructors
};

// Use stack allocation when possible
void stackAllocationExample() {
    std::vector<int> vec;  // Stack allocated, automatic cleanup
    // No manual memory management needed
}
```

##### 2. Type Safety

```c++
// Use strong typing and avoid C-style casts
enum class Color { Red, Green, Blue };

class SafeClass {
    Color color;
    explicit SafeClass(Color c) : color(c) {}  // Explicit constructor
public:
    static SafeClass createRed() { return SafeClass(Color::Red); }
};

// Prefer static_cast over C-style casts
int value = 42;
double d = static_cast<double>(value);  // Clear and safe
```

##### 3. Const-Correctness

```c++
class ConstCorrect {
    mutable std::string cache;
    mutable bool cacheValid = false;
    int data;
public:
    int getData() const { return data; }  // Logical const
    void setData(int d) { data = d; }     // Modifies logical state
    
    std::string getExpensiveData() const {
        if (!cacheValid) {
            // Safe to modify mutable members in const method
            const_cast<ConstCorrect*>(this)->updateCache();
        }
        return cache;
    }
private:
    void updateCache() {
        cache = "expensive computation result";
        cacheValid = true;
    }
};
```

##### 4. Interface Design

```c++
// Use pure virtual functions for clear interfaces
class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw() const = 0;
    virtual void setColor(const std::string& color) = 0;
};

// Use multiple inheritance for interface segregation
class Shape : public Drawable, public Serializable {
    // Implementation
};
```

#### Common Pitfalls to Avoid

1. **Memory Leaks**: Always use RAII and smart pointers
2. **Dangling Pointers**: Be careful with raw pointer lifetimes
3. **Slicing**: Use references/pointers for polymorphic objects
4. **Undefined Behavior**: Understand object lifetime and alignment
5. **Performance Issues**: Be mindful of virtual function overhead
6. **Type Safety**: Avoid unsafe casts and prefer strong typing

#### Migration to Modern C++

When modernizing existing C++ code, consider these steps:

1. **Replace raw pointers with smart pointers**
2. **Use range-based for loops instead of iterators**
3. **Prefer constexpr for compile-time computations**
4. **Use override and final keywords appropriately**
5. **Replace C-style arrays with std::array or std::vector**
6. **Use auto for type deduction when appropriate**
7. **Implement move semantics for resource-heavy classes**

This comprehensive review provides the foundation needed to understand and effectively use Modern C++ features. The concepts covered here are essential for writing safe, efficient, and maintainable C++ code that takes full advantage of the language's capabilities.
