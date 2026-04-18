Set up a new numbered C++ project for this learning repo. Creates the folder, Makefile, exercises, quiz, and answer files — all tailored to the student's progress, past quiz scores, and career path toward NVIDIA DRIVE AV.

**Arguments:** The new project's topic name (e.g. `stl_containers`). If omitted, determine the next topic automatically (see Step 4).

---

## Step 1 — Create the folder and Makefile

1. List `projects/` to find the highest existing `NNN_` prefix. Increment by 1 (zero-padded to 3 digits) to get `NNN`.
2. Create `projects/NNN_$ARGUMENTS/`.
3. Write this exact `Makefile` to that folder:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = $(wildcard *.cpp)
TARGETS = $(SRCS:.cpp=.exe)

all: $(TARGETS)

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
```

4. The `.gitignore` already has a `*.exe` rule, so no per-project entries are needed.

5. Update `compile_commands.json` at the repo root:
   - Read the current `compile_commands.json`.
   - For each `.cpp` file created by the exercises (Step 4), add an entry:
     ```json
     {
         "directory": "C:/Users/CRAIG-DSOUZA/Code/c++",
         "file": "projects/NNN_topic/filename.cpp",
         "arguments": ["g++", "-std=c++17", "-Wall", "-Wextra", "projects/NNN_topic/filename.cpp"]
     }
     ```
   - If no `.cpp` files are defined yet, add entries once the exercises file names them.

---

## Step 2 — Active Recall Review (Spaced Repetition)

Read `progress.md`. Find all dated entries that have a **Carry-Forward** section. For each, note the entry date and its carry-forward items (quiz questions and failed exercises).

Apply spaced repetition based on how old each entry is relative to today:

| Entry age | Quiz questions to include | Exercises to include |
|---|---|---|
| Most recent entry | Scored ≤ 0.5 only | All failed exercises |
| ~7 days ago | Scored ≤ 0.5 only | None |
| ~14+ days ago | Scored ≤ 0.5 only | None |

Questions scored 0.75 are not carried forward — partial credit means the concept is understood well enough to move on.

Collect all qualifying quiz questions as the **Active Recall Warm-Up** for the new exercises file, labelled by source day (e.g. *"From Day 2 — Review"*).

Collect all qualifying failed exercises as **Repeat Exercises**, labelled by source day.

If `progress.md` has no graded entries yet, skip this step.

---

## Step 2b — Read the student understanding snapshot

Read `memory.md` at the repo root. This file documents:
- What C++ concepts the student currently understands solidly
- Known gaps in CS fundamentals and "why" reasoning
- Learning patterns and what kinds of explanations land well
- Carry-forward items not yet captured in `progress.md`
- Specific recommendations for upcoming days

Use this to calibrate:
- **Background section depth** — if a CS fundamental is listed as a gap, introduce it explicitly rather than assuming it
- **Exercise scaffolding** — lean toward more or less guidance based on the student's applied vs conceptual balance
- **Quiz question framing** — target question types that address noted gaps (e.g. "why" questions if reasoning lags application)
- **Python analogies** — use them where noted to be effective
- **Concept explainers** — if `memory.md` recommends pre-reading a `concepts/` file, link to it in the Background section

---

## Step 3 — Determine the next topic

Read `progress.md` to see what has been covered and the most recent Carry-Forward state.
Read `career-nvidia-av-drive.md` for the full learning roadmap and the "Next 7 Curriculum Days" table in the Current Sprint section.

Use the roadmap to decide the next most foundational topic. Good candidates in order:
1. STL containers: `vector`, `map`, `unordered_map`, iterators, range-for
2. RAII and destructors: constructors, destructors, resource management
3. Memory management: heap vs stack, `new`/`delete`, why smart pointers exist
4. Smart pointers: `unique_ptr`, `shared_ptr`, ownership semantics
5. Move semantics: rvalue references, move constructor, move assignment, `std::move` internals
6. Templates: function templates, class templates, type deduction, `auto`
7. Lambdas + `std::algorithm`: lambda syntax, captures, `sort`/`find_if`/`transform`
8. File parsing + real data: deeper CSV/text parsing, reading real OSM or GPS data into structs
9. CMake + multi-file projects: `CMakeLists.txt`, splitting across headers/source, linking
10. Inheritance + polymorphism: base/derived classes, `virtual`, `override`, abstract classes
11. Concurrency: `std::thread`, `std::mutex`, `std::condition_variable`, producer-consumer pattern
12. Networking + serialization: Protocol Buffers, gRPC basics, why FlatBuffers over JSON

Choose the topic that best fits the student's current level and career path momentum. If $ARGUMENTS was provided, use that as the topic name and infer what to cover from it.

---

## Step 4 — Write the exercises file

**Before writing:** Check if `NNN_exercises.md` already exists in the project folder.
- If it exists: read it, check whether Repeat Exercises sections are present and match the carry-forward items from Step 2. Add or update those sections only — do not delete or recreate the file.
- If it does not exist: create it.

File path: `projects/NNN_topic/NNN_exercises.md`.

**Important — Active Recall Warm-Up placement:** Carried-over review questions go in the **quiz file only** (Step 5). Do NOT add an Active Recall Warm-Up section to the exercises file. The exercises file references the quiz file for warm-up questions with a single line: *"Before starting: answer the warm-up questions in `NNN_topic_quiz.md`."*

**Important — Exercise scaffolding rules:**
- **Exercises 1–4:** May include boilerplate, but boilerplate must be written as **comments that hint at what to write**, not as working code. Example: `// declare a unique_ptr to MapTile` or `// use std::move to transfer ownership`. Do not provide working implementations. The student writes all functional code themselves.
- **Exercise 5 — Integration (mandatory):** Must be the most demanding exercise. Requirements:
  - Integrates concepts from all four prior exercises in a single, non-trivial program
  - **No boilerplate provided** — no scaffold, no comments hinting at steps, no partial code. Just a spec describing what the program must do.
  - The student writes everything from scratch
  - Should reflect a realistic AV/mapping scenario that would feel at home in the NVIDIA DRIVE stack

**Important — Difficulty escalation rule:**
- Each exercise should introduce at most 2 new concepts beyond the previous one.
- Always introduce a concept using the simplest possible type first (e.g. `std::string`, `std::vector`) before applying it to raw pointers or complex class hierarchies. Never combine raw pointer memory management with a concept being introduced for the first time in the same exercise.
- Order exercises easy → hard within the day. If one exercise uses `std::vector` and another uses raw `float*` to demonstrate the same concept, the `std::vector` exercise comes first.

**Important — Syntax scaffolding rule (Python-background student):**
- C++ class syntax is a known barrier for this student. Any exercise that asks them to define a class must provide at minimum (as hint comments): the class opening line, the `public:` label, and member variable declarations. The student fills in method bodies and logic — not boilerplate class structure.

Structure:

```
# Day N — [Topic Name]

**Goal:** [One sentence — what the student will be able to do after this day, tied to the NVIDIA AV context]

> Before starting: answer the warm-up questions in `NNN_topic_quiz.md`.

---

## Background
[2–4 paragraphs. Explain the concept from first principles.
Use Python analogies where the student's existing knowledge helps.
Connect to the NVIDIA AV / mapping domain concretely.]

## Exercise 1 — [Name]
**File:** Create `projects/NNN_topic/filename.cpp` (new file).
[Clear task. Boilerplate as comments only — hints, not implementations.]
**What to observe:** [Connect to the broader concept and career relevance]

## Exercise 2 — [Name]
**File:** Update or create as appropriate.
[Comments-as-hints only. No working code provided.]
...

## Exercise 3 — [Name]
...

## Exercise 4 — [Name]
...

## Exercise 5 — Integration
**File:** Create `projects/NNN_topic/integration.cpp` (new file).
**No scaffold provided.** Write this program from scratch.

[Spec only — describe what the program must do, the data it operates on, and what correct output looks like. No hints, no partial code, no step-by-step structure. This should require combining all four prior concepts in a non-trivial way.]

[If there are repeat exercises from Step 2, add a clearly labeled section:]
## Repeat Exercises (from Day X)
[Re-state the exercise with a note that it was previously incomplete]

## Checkpoint
You've passed Day N when you can:
- [bullet]
- [bullet]
- [bullet]
```

---

## Step 4b — Create scaffold `.cpp` files

For each exercise named in the exercises file, create a corresponding `.cpp` file in `projects/NNN_topic/`. Do not create files that already exist.

**Scaffold ratios** — how much of the file the student must write:

| Exercise | Student writes | What the scaffold provides |
|---|---|---|
| 1 | ~1/5 | Full class skeleton (opening, `public:`, members as hint-comments), includes, `main` shell, hint-comments throughout |
| 2 | ~2/5 | Includes, key data setup (e.g. variable declarations), hint-comments for the remaining logic |
| 3 | ~3/5 | Includes, method/function signatures as hint-comments, `main` shell |
| 4 | ~4/5 | Includes only, plus hint-comments outlining the structure |
| 5 | 100% | Hint-comments only — the spec from the exercises file, no code |

**Rules for all scaffold files:**
- All scaffold is hint-comments (`// ...`), never working code
- Class-based exercises must always provide: class opening line, `public:` label, and member variable declarations as hint-comments — even at higher difficulty tiers
- Commented-out code stubs (e.g. `// SomeType foo = ...`) are acceptable as directional hints
- Add entries for each new `.cpp` file to `compile_commands.json` at the repo root (same format as Step 1)

---

## Step 5 — Write the quiz file

**Before writing:** Check if `NNN_topic_quiz.md` already exists in the project folder.
- If it exists: read it, check whether carried-over warm-up questions are present. Add missing ones only — do not delete existing questions, answers, or the Grade Log.
- If it does not exist: create it.

File path: `projects/NNN_topic/NNN_topic_quiz.md`.

Structure:

```
# Quiz — [Topic Name]

---

[If there are carried-over questions from Step 2:]
## Warm-Up (From Day X — Review)

**QR1:** [Carried-over question verbatim]

## Answer QR1
[blank for student]

---

## Question 1
[New question for today's topic]
...

## Answer 1
[blank for student]

---

[Continue for 4–5 new questions]

---

## Grade Log
```

**Quiz question guidelines:**
- Test *reasoning*, not just recall — ask "why" and "when", not only "what"
- Use realistic AV/mapping scenarios: tile nodes, sensor readings, map updates, vehicle state, etc.
- Mix question types: choose the right type (`T`, `T&`, `T*`, etc.), spot the bug, explain the concept, predict the output
- Each question should be answerable in 2–5 sentences; avoid yes/no questions

---

## Step 6 — Write the answers file

**Before writing:** Check if `NNN_topic_answers.md` already exists in the project folder.
- If it exists: read it, check whether answers for any carried-over warm-up questions are present. Add missing answers only — do not modify or delete existing content.
- If it does not exist: create it.

File path: `projects/NNN_topic/NNN_topic_answers.md`.

Structure:

```
# Answers — [Topic Name]

---

## Question [N]

**[The correct answer / type / choice]**

[Explanation of *why* — 2–5 sentences. Focus on intuition, not just correctness.
For carried-over questions, note which day they came from.
Include a correct code snippet for any question that involves code.]

---
```

Tone: same as the professor — encouraging, connects to broader C++ mental model, uses Python analogies where helpful.

---

## Final Output

After creating all files, print:
- The new project path
- The topic chosen and why (one sentence)
- Carried-over warm-up questions, if any (list by source day + question number)
- Repeated exercises, if any (list by source day + exercise number)
- The full list of files created or updated (note which were new vs modified)
