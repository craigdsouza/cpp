Set up or maintain a numbered C++ project folder under `projects/` for this learning repo.

**Steps:**

1. List the contents of `projects/` to check if any folder already matches `*_$ARGUMENTS` (e.g. `002_ref_and_pointers`).
2. **If a matching folder exists:** use it as-is. Do not create a new folder or renumber.
3. **If no matching folder exists:** find the highest existing `NNN_` prefix, increment by 1 (zero-padded to 3 digits), and create `projects/NNN_$ARGUMENTS/`.
4. In either case, check whether a `Makefile` already exists in that folder.
   - If it does not exist, create it.
   - If it already exists, overwrite it (it's always safe to regenerate).
5. Write this exact content to the `Makefile`:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = $(wildcard *.cpp)
TARGETS = $(SRCS:.cpp=)

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
```

6. Update `.gitignore` at the repo root:
   - Read the current `.gitignore`.
   - Look for any existing entries under a comment that matches the project folder name (e.g. `# 002_ref_and_pointers`).
   - If entries for this project already exist, skip (do not duplicate).
   - If not, append a block like this at the end of `.gitignore` (using the actual project folder name and each `.cpp` file's stem as the binary name — if no `.cpp` files exist yet, leave a placeholder comment):

```
# NNN_name
projects/NNN_name/binary_name_1
projects/NNN_name/binary_name_2
```

7. Confirm what was created or updated and print the full project path.

**Note:** The Makefile automatically detects every `.cpp` file in the folder and compiles each one to its own binary. Running `make` builds all of them. Running `make clean` removes all binaries.
