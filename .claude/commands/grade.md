Grade the student's quiz and exercises for the project specified in $ARGUMENTS (e.g. `002`). If no argument is given, ask which project to grade.

---

## Part 1 — Grade the Quiz

1. Find the quiz file at `projects/<id>_*/` matching the argument (e.g. `002_ref_and_pointers_quiz.md`).
2. Check the **Grade Log** section of the quiz file. If an entry already exists for today's date, skip grading and inform the user: "Quiz already graded today — run `/grade` again tomorrow or on the next session."
3. Find the corresponding answers file in the same folder (e.g. `002_ref_and_pointers_answers.md`).
3. Grade each question using this rubric:
   - **1.0** — correct answer + complete reasoning
   - **0.75** — correct answer + partial reasoning (key insight present but something missing)
   - **0.5** — correct answer + no reasoning
   - **0.0** — wrong answer or unanswered
4. Show a breakdown table to the user: one row per question with score and a one-line note on what was missing (if anything).
5. Show the total quiz score out of the maximum.
6. Append a new entry to the **Grade Log** section at the bottom of the quiz file with today's date, the total score, and the per-question breakdown table (Q, Score, Note). If no Grade Log section exists yet, create one.

---

## Part 2 — Grade the Exercises

1. Find the exercises file in the same project folder (e.g. `002_exercises.md`).
2. Check the **Exercise Results** section of the exercises file. If an entry already exists for today's date, skip grading and inform the user: "Exercises already graded today."
3. Read all exercises in the file.
3. For each exercise, look for evidence of completion:
   - Is there a `.cpp` file in the project folder that corresponds to this exercise?
   - Does the code in that `.cpp` file appear to address the exercise goal?
   - Is there any explicit pass/fail marker already written in the exercises file?
4. For each exercise assign **Pass** or **Fail**:
   - **Pass** — a corresponding `.cpp` exists and addresses the exercise goal, OR the exercise is explicitly marked as passed
   - **Fail** — no `.cpp` found, or the `.cpp` clearly doesn't address the exercise goal, OR explicitly marked as failed
   - **Not attempted** — treat as Fail for carry-forward purposes
5. Show an exercise summary table to the user: one row per exercise with Pass/Fail and a one-line note.
6. Append a **Exercise Results** section to the exercises file using this format (add after the last exercise, before any existing status section; if already present, update it):

```markdown
## Exercise Results — [DATE]

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — [Name] | Pass | [brief note or blank] |
| Exercise 2 — [Name] | Fail | [what was missing] |
```

---

## Part 3 — Update progress.md

Read `progress.md`. Append a new dated entry at the end of the file in this format:

```markdown
## [DATE] — Day [N]: [Topic Name]

### Quiz Score
[Total] / [Max] — [one sentence characterising performance, e.g. "Strong on reasoning, gaps in ruling out alternatives"]

| Q | Score | Note |
|---|-------|------|
| Q1 | X.X | ... |
...

### Exercises
| Exercise | Result |
|----------|--------|
| Exercise 1 — [Name] | Pass |
| Exercise 2 — [Name] | Fail |
...

### Concepts Confirmed
[2–4 bullet points of what the student demonstrably understands based on their quiz answers — not aspirational, only what the graded answers actually show]

### Carry-Forward
[List quiz questions scored ≤ 0.5 and any failed exercises as one-line items — these will be picked up by /project for the next day's warm-up. Questions scored 0.75 are NOT listed here. If nothing qualifies, write "None — clean sweep."]
```

---

## Tone

Encouraging. Point out what was done well before noting gaps. The carry-forward section is framed as "things to revisit" not "things you got wrong."
