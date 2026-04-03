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
