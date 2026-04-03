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
TARGETS = $(SRCS:.cpp=)

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
```

4. Update `.gitignore` at the repo root:
   - Read the current `.gitignore`.
   - If an entry block for this project already exists, skip.
   - Otherwise append at the end:

```
# NNN_name
projects/NNN_name/binary_name_1
projects/NNN_name/binary_name_2
```

(Use the actual folder name. If no `.cpp` files exist yet, leave a placeholder comment.)

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
Read `career-nvidia-av-drive.md` for the full learning roadmap.

Use the roadmap to decide the next most foundational topic. Good candidates in order:
1. STL containers: `vector`, `map`, `unordered_map`, iterators, range-for
2. RAII and destructors: constructors, destructors, resource management
3. Memory management: heap vs stack, `new`/`delete`, why smart pointers exist
4. Smart pointers: `unique_ptr`, `shared_ptr`, ownership semantics
5. Templates: function templates, class templates, the `auto` keyword

Choose the topic that best fits the student's current level and career path momentum. If $ARGUMENTS was provided, use that as the topic name and infer what to cover from it.

---

## Step 4 — Write the exercises file

**Before writing:** Check if `NNN_exercises.md` already exists in the project folder.
- If it exists: read it, check whether the Active Recall Warm-Up and Repeat Exercises sections are present and match the carry-forward items from Step 2. Add or update those sections only — do not delete or recreate the file.
- If it does not exist: create it.

File path: `projects/NNN_topic/NNN_exercises.md`.

Structure:

```
# Day N — [Topic Name]

**Goal:** [One sentence — what the student will be able to do after this day, tied to the NVIDIA AV context]

## Active Recall Warm-Up
[Only if there are carried-over questions from Step 2. Label each with its source day.
Direct the student to write answers in the previous day's answers file before reading on.]

---

## Background
[2–4 paragraphs. Explain the concept from first principles.
Use Python analogies where the student's existing knowledge helps.
Connect to the NVIDIA AV / mapping domain concretely.]

## Exercise 1 — [Name]
**File:** Create `projects/NNN_topic/filename.cpp` (new file).
[Clear task. Code scaffold or example where helpful.]
**What to observe:** [Connect to the broader concept and career relevance]

## Exercise 2 — [Name]
**File:** Update `filename.cpp` (add to existing file) OR Create `filename2.cpp` (new file) — whichever fits.
...

## Exercise 3 — [Name]
**File:** Update `filename.cpp` (add to existing file) OR Create `filename3.cpp` (new file) — whichever fits.
...

[Add Exercise 4 and 5 if the topic warrants it]

[If there are repeat exercises from Step 3, add a clearly labeled section:]
## Repeat Exercises (from Day X)
[Re-state the exercise with a note that it was previously incomplete]

## Checkpoint
You've passed Day N when you can:
- [bullet]
- [bullet]
- [bullet]
```

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
