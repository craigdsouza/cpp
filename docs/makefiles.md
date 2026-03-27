# Makefiles

A Makefile is a recipe file for the `make` tool. It tells `make` **what to build**, **how to build it**, and **when to rebuild it**.

---

## The problem it solves

When you have a C++ file, you could just run the compiler directly every time:

```bash
g++ -std=c++17 -Wall -Wextra -o references references.cpp
```

That works, but it gets tedious fast. Once you have 5 files, you'd have to remember 5 commands. A Makefile stores those commands so you just type `make`.

---

## How `make` decides what to rebuild

This is the clever part. `make` checks **file timestamps**. It only recompiles a file if the source (`.cpp`) is **newer** than the binary. So if you have 10 files and only change one, `make` rebuilds just that one.

---

## Reading your Makefile

```makefile
CXX = g++                          # the compiler to use
CXXFLAGS = -std=c++17 -Wall -Wextra  # flags passed to every compile command

SRCS = $(wildcard *.cpp)           # finds all .cpp files in the folder
TARGETS = $(SRCS:.cpp=)            # strips .cpp → gives you the binary names

all: $(TARGETS)                    # "make" with no args builds everything

%: %.cpp                           # pattern rule: any binary depends on its .cpp
    $(CXX) $(CXXFLAGS) -o $@ $<   # $@ = the target name, $< = the source file

clean:
    rm -f $(TARGETS)               # "make clean" deletes all binaries

.PHONY: all clean                  # tells make these aren't real files
```

The pattern rule `%: %.cpp` is the key line. It's saying: *"to build any binary, compile the `.cpp` file with the same name."* The `$@` and `$<` are automatic variables — shorthand for "the thing I'm building" and "the thing I'm building it from".

---

## In practice for your exercises

Your Day 2 folder will have `references.cpp` and `pointers.cpp`. Running:

```bash
make              # builds both ./references and ./pointers
make references   # builds just ./references
make clean        # deletes both binaries
./references      # run it
```

---

## As projects grow

As your projects grow (multiple files that depend on each other, external libraries), Makefiles get more complex — but the core idea stays the same: **declare what depends on what, and let make figure out the minimum work needed.**
