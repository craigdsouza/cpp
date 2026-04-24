Grade the student's quiz and exercises for the project specified in $ARGUMENTS (e.g. `002`). If no argument is given, ask which project to grade.

---

## Part 0 — Synthesize Chat Context

Before grading, review the current conversation (everything the student and assistant said before `/grade` was invoked). Extract signals about the student's understanding that the quiz and code alone cannot reveal.

Build a **Chat Insights** object with the following fields — you will reference these in Parts 3, 4, and 5:

- **Demonstrated understanding**: Concepts the student *explained or reasoned about correctly in their own words*, without being prompted to do so. These count as evidence of genuine understanding even if the quiz answer was incomplete.
- **Received understanding**: Concepts the student was confused about and then had explained to them by the assistant. These are *not* promotable to "solid" — but if the student then correctly paraphrased or applied the concept, they may qualify.
- **Confusion signals**: Questions the student asked that revealed a missing foundation, or moments where the student's framing was wrong before correction. Note the specific misconception, not just the topic.
- **Clarifying questions**: Questions that suggest the student understood the surface but not the depth — useful for shaping the "Coming into Day N+1" note.
- **Applied mechanics**: Any C++ mechanics the student used or correctly described in conversation, beyond what appears in the `.cpp` files.

Keep each field concise — bullet points, one line each. If the conversation contains no signal for a field, write "None."

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
[2–4 bullet points of what the student demonstrably understands. Draw from quiz answers (score 1.0) AND from the Chat Insights "Demonstrated understanding" field — concepts the student explained correctly in conversation count here even if the quiz answer was incomplete.]

### Carry-Forward
[List quiz questions scored ≤ 0.5 and any failed exercises as one-line items. Also include any "Confusion signals" from the Chat Insights that were NOT resolved by the end of the conversation. Questions scored 0.75 and "Clarifying questions" that got answered are NOT listed here. If nothing qualifies, write "None — clean sweep."]
```

---

## Part 4 — Update memory.md

Read `memory.md` at the repo root. Update it to reflect what was learned and observed this session. This is not a log — it is a **living snapshot** of the student's current state. Rewrite sections in place rather than appending.

Apply the following updates:

1. **Update "Last updated"** to today's date and day number.

2. **Promote concepts to "What is solid"** if quiz answers or exercises this session demonstrated clear understanding — correct answer + complete reasoning (score 1.0), or correct code written without prompting. Also promote concepts from the Chat Insights "Demonstrated understanding" field where the student explained or applied the concept correctly in their own words.

3. **Remove gaps that are now closed** — if a gap listed under "Known gaps" was addressed this session (correct answer on a previously missed concept, demonstrated in an exercise, or correctly paraphrased in conversation after it was explained), move it to solid or remove it. Use the Chat Insights "Received understanding" field to check: if the student later showed they internalized it, close the gap; if they only passively acknowledged the explanation, leave it open.

4. **Add new gaps** surfaced this session — any concept where the student scored 0.0 or 0.5, showed confusion, or asked a clarifying question that revealed a missing foundation. Also include "Confusion signals" from the Chat Insights that were not resolved. Note *what specifically* was missing, not just the topic.

5. **Update "Curriculum position"** — mark the just-completed day as Complete and update the "Next" row.

6. **Update "Coming into Day N+1"** — summarise what the student is carrying into the next day: what's solid, what's a gap, and any specific concept from carry-forward that will be relevant.

7. **Update "Recommendations"** — revise or add recommendations based on patterns observed this session. Remove recommendations that are no longer relevant.

Do not append dated log entries — `progress.md` handles the log. `memory.md` should always read as a current snapshot, not a history.

---

---

## Part 5 — Update the Practiced Mechanics list in glossary.md

Read the **Practiced Mechanics** section of `glossary.md`.

For each exercise that **Passed** this session, scan the corresponding `.cpp` file and identify any mechanics the student implemented that are not yet listed in the Practiced Mechanics section.

Also check the Chat Insights "Applied mechanics" field for any mechanics the student correctly described or used in conversation that do not appear in the `.cpp` files.

Add any new mechanics under the appropriate category. Use the same concise format as existing entries (e.g. `- modulo index wrapping (\`(index + 1) % N\`)`).

Do not remove existing entries. Do not duplicate existing entries. Only add mechanics that were genuinely exercised or correctly demonstrated — not ones merely mentioned in comments, scaffold, or where the student showed confusion.

---

## Tone

Encouraging. Point out what was done well before noting gaps. The carry-forward section is framed as "things to revisit" not "things you got wrong."
