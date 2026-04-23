# C++ Glossary

## Day 1
- vector
- integer
- compile flag (`-Wall`, `-Wextra`)
- range-for loop
- `std::cout`
- `std::string`
- primitive type
- standard library type

## Day 2
- reference (`&`)
- const reference (`const T&`)
- pointer (`*`)
- dereference operator (`*p`)
- address-of operator (`&x`)
- `nullptr`
- null check
- pass by value
- pass by reference
- pass by pointer
- undefined behavior
- return value optimization (RVO)
- stack variable
- local variable
- `static` variable

## Day 3
- struct
- class
- member
- access modifier
- public
- private
- encapsulation
- aggregate initialization
- constructor
- `this` pointer
- member function
- free function
- `const` member function
- invariant
- `std::vector<T>`

## Day 4
- STL (Standard Template Library)
- `std::map<K, V>`
- `std::unordered_map<K, V>`
- iterator
- `begin()`
- `end()` (sentinel / past-the-end)
- `find()`
- `erase()`
- `operator[]` (map)
- default-constructed value
- ghost tile (silent insertion via `operator[]`)
- O(log n) vs O(1) lookup
- hash map
- red-black tree
- range-for over a map (`auto& item`)
- `it->first` / `it->second`
- sentinel value

## Day 5
- RAII (Resource Acquisition Is Initialization)
- destructor (`~ClassName()`)
- destructor firing order (reverse construction order)
- scope / block scope
- stack unwinding
- exception safety
- `std::ofstream` (output file stream)
- `std::ifstream` (input file stream)
- `file.open()` / `file.close()`
- `std::getline()`
- buffer (I/O buffering)
- flush
- heap allocation (`new`)
- heap deallocation (`delete`, `delete[]`)
- memory leak
- raw array (`int* data`, `new int[size]`)
- initializer list (`: member(value)`)
- `std::runtime_error`
- `try` / `catch` / `throw`

## Day 6
- `std::unique_ptr<T>`
- `std::make_unique<T>()`
- `std::shared_ptr<T>`
- `std::make_shared<T>()`
- ownership transfer (`std::move` with smart pointers)
- reference count (shared_ptr)
- `.get()` (raw pointer access)
- `.reset()` (release ownership)
- `= delete` (deleted copy constructor / assignment)
- factory function (returns `unique_ptr`)
- `nullptr` after move (unique_ptr source becomes null)

## Day 7
- rvalue reference (`T&&`)
- move constructor (`ClassName(ClassName&&)`)
- move assignment operator (`operator=(ClassName&&)`)
- steal-and-nullify pattern
- self-assignment guard (`if (this == &other) return *this`)
- Rule of Five (destructor + copy ctor + copy assign + move ctor + move assign)
- `std::move` as a cast (lvalue → xvalue)
- NRVO (Named Return Value Optimization)
- value categories (lvalue, prvalue, xvalue)

## Day 8
- function template (`template<typename T>`)
- class template (`template<typename T> class Foo`)
- template type deduction (compiler infers T from arguments)
- template instantiation (compiler generates concrete version per type)
- full template specialization (`template<> class Foo<bool>`)
- non-type template parameter (`template<typename T, int N>`)
- stack-allocated C array (`T data[N]`)
- `constexpr` compile-time constant
- modulo index wrapping (`(index + 1) % N`)
- head/tail ring buffer indexing
- function pointer parameter (`T (*fn)(T)`)
- `const` method on a template class

---

## Practiced Mechanics

> This section tracks C++ mechanics the student has **actually implemented** in exercises — not just read about. The `/project` command uses this to avoid introducing the same mechanic as "new" again, and to cap new mechanics per exercise at 2.

### Output and I/O
- `std::cout` with `<<` and `std::endl`
- `std::ofstream` write to file
- `std::ifstream` + `std::getline` line-by-line read
- `file.is_open()` check after `std::ifstream` construction
- `std::stringstream(line)` — wrap a string as a stream for field-by-field parsing
- `std::getline(ss, field, ',')` — extract one delimited field at a time from a stringstream
- header line skip (`std::getline(file, header)` before the parse loop)
- `std::stof` / `std::stoi` — string-to-float and string-to-int conversion
- `try/catch std::invalid_argument` inside a parse loop to skip malformed lines

### Variables and Types
- Declaring `int`, `float`, `double`, `std::string`, `bool`
- `const` local variable
- `static` local variable
- `constexpr` compile-time constant

### Control Flow
- Range-for loop with copy (`for (T x : v)`)
- Range-for loop with const reference (`for (const T& x : v)`)
- Range-for loop with mutable reference (`for (T& x : v)`)
- Index-based for loop (`for (int i = 0; i < n; i++)`)
- `while` loop
- `if` / `else if` / `else`

### Functions
- Free function definition and call
- Pass by value, by reference (`T&`), by const reference (`const T&`), by pointer (`T*`)
- Return by value
- `nullptr` check on pointer parameter
- Function pointer parameter (`T (*fn)(T)`)

### Structs and Classes
- `struct` definition with member variables
- `class` definition with `public` / `private`
- Constructor with `this->` assignment
- Constructor with initializer list (`: member(value)`)
- `const` member function
- Destructor (`~ClassName()`)
- RAII class (constructor acquires, destructor releases)
- Member function that returns a value
- `= delete` on copy constructor and copy assignment

### Memory
- Stack variable (automatic lifetime)
- Heap allocation with `new` / `delete[]`
- Raw array on stack (`T data[N]` with compile-time N)
- `std::unique_ptr` (`make_unique`, move, `.get()`, `*ptr`)
- `std::shared_ptr` (`make_shared`, reference count tracing, `.reset()`)
- Factory function returning `unique_ptr`
- Move constructor (steal-and-nullify)
- Move assignment operator (self-assignment guard, free-then-steal)
- Rule of Five implementation from scratch

### STL Containers
- `std::vector<T>`: `push_back`, `[]`, `size()`, `empty()`, `resize()`, `reserve()`
- `std::vector<T>`: range-for, index loop
- `std::map<K,V>`: insert with `[]`, `find()`, `erase()`, iterate with `it->first`/`it->second`
- `std::unordered_map<K,V>`: same operations
- `find()` + `end()` sentinel idiom (safe lookup without ghost-tile insertion)
- `std::map::insert({key, value})` — insert without triggering default construction (avoids operator[] trap)
- range-for over `std::map` with `pair.second` to copy values into a `std::vector`

### Error Handling
- `try` / `catch` / `throw`
- `std::runtime_error`

### Templates
- Function template definition and call (compiler deduces T)
- Class template with member functions in class body
- Full template specialization (`template<>`)
- Non-type template parameter (compile-time `int N`)
- Template class with `const` method

### Lambdas and Algorithms
- lambda with no capture (`[]`)
- lambda capturing by value (`[x]`, `[max_val]`)
- lambda capturing by reference (`[&x]`, `[&count]`)
- `mutable` lambda (modify captured-by-value variable inside lambda body)
- `std::for_each` with a stateless lambda (print each element)
- `std::for_each` with a capturing lambda (accumulate into outer counter via `[&count]`)
- `std::sort` with lambda comparator (ascending: `a.x < b.x`, descending: `a.x > b.x`)
- `std::find_if` with lambda predicate — iterator + `end()` sentinel guard
- `&(*it)` — convert iterator to raw pointer (dereference then take address)
- `std::transform` in-place (same vector as input and output)
- `std::transform` lambda: takes value, returns modified value
- `std::count_if` with lambda predicate
- `std::function<R(Args)>` as a stored lambda type
- class with all algorithm methods (`sort_by_intensity`, `find_above`, `normalize`, `count_above`, `print_all`) using lambdas internally
- `std::max_element` with lambda comparator (`a.field < b.field`) to find the struct with the highest field value
- `std::accumulate` with binary lambda to sum a field across a vector of structs
- chaining `->field` onto `std::max_element(...)` result to extract the value directly
- `&(*it)` to return a raw pointer from a `std::max_element` / `std::find_if` iterator result

### Multi-File Projects and Build Systems
- `#pragma once` — include guard in a header file
- header/source split — declaration in `.h`, definition in `.cpp`
- `ClassName::method()` — scope resolution prefix on member function definitions in `.cpp`
- `#include "user_header.h"` — quotes for user headers (vs angle brackets for stdlib)
- `-I include` — add directory to compiler header search path
- multi-file compilation — listing multiple `.cpp` files in one `g++` command
- `add_executable(name files...)` — declare a CMake build target
- `target_include_directories(target PRIVATE dir)` — add include path to a CMake target
- `cmake -B build` — configure and generate build files
- `cmake --build build` — compile using generated build files
- `std::map::emplace(key, value)` — insert into map without requiring default constructor on value type
- returning `&it->second` — return pointer to a map value from a const member function
- returning `nullptr` for not-found from a `const T*` function

### Data Structures
- Ring buffer with head/tail/size tracking
- Modulo index wrapping (`(index + 1) % capacity`)
- Fixed-size stack buffer (`T data[N]`, zero heap allocation)
- `push` / `pop` / `full` / `empty` / `size` interface
- Function applied to all elements via function pointer (`apply` — pop all, transform, push back)
- `peek(int i) const` — logical index access without consuming (`data[(head_ + i) % N]`)
- Full warning on overflow (`feed` prints warning and returns without pushing when buffer is full)
