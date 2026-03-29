# C++ Benchmark — Step-by-Step Guide

A practical how-to for running each block of the benchmark. Read this once before Day 14 so nothing surprises you on the day.

---

## Block 1: Codeforces Virtual Contest

### What Codeforces is
Codeforces is a competitive programming platform used widely in the industry and academia. It has a real ELO-style rating system (called just "rating") that goes from ~800 (complete beginner) upward. Contests are tiered: **Div. 4** is the most beginner-friendly, **Div. 1** is for elite competitors. You'll be running Div. 4 for the first several benchmarks.

### Setting up your account
1. Go to [codeforces.com](https://codeforces.com)
2. Click **Register** (top right)
3. Choose a handle (username) — this is public-facing, pick something professional
4. Verify your email
5. You don't need to configure anything else for now

### Finding a past Div. 4 contest to run virtually
1. Log in, then click **Contests** in the top navigation
2. Click **"Gym"** is NOT what you want — stay on **Contests**
3. Look for the filter or scroll through the list. Filter by **"Div. 4"** in the search/filter bar
4. Pick any recent Div. 4 contest (anything from the last 12 months is fine — avoid the very latest since your peers may have recently competed in it, but it doesn't matter much)
5. Click the contest name to open it

### Running it in Virtual Mode
1. On the contest page, look for the **"Virtual participation"** button (it appears because the contest is over)
2. Click it — it will ask you to confirm you want to start a virtual run
3. This starts a **simulated timer** (usually 2 hours for Div. 4) and shows you the problems as if the contest were live
4. **Set your own timer for 90 minutes** — you don't need the full 2 hours for a benchmark; 90 min gives you a meaningful signal without burning a morning

### The problems
Div. 4 contests typically have 7–8 problems labelled A through G/H.
- **Problem A**: Very easy — basic loops, conditionals, simple math. Should be solvable in 10–15 min once you have basic C++ down.
- **Problem B**: Easy — slightly more logic. Aim to get this by benchmark #2–3.
- **Problem C and beyond**: Medium difficulty — don't stress about these early on.

**For Day 14:** Aim for Problem A. Getting A + attempting B is a strong result at this stage.

### Submitting your solution
1. Write your code in your local editor (Cursor/WSL2 is fine)
2. On the problem page, click **Submit**
3. Paste your code, select **C++17 (GCC 9-64)** as the language
4. Click Submit — you'll see a verdict: `Accepted`, `Wrong Answer`, `Time Limit Exceeded`, etc.

### Getting your estimated rating with CF Predictor
After the virtual contest ends (or during it):
1. Install the **CF Predictor** browser extension:
   - Chrome: search "CF Predictor" in the Chrome Web Store
   - It's free and widely used in the community
2. Once installed, it adds a "predicted rating change" display to your contest standings page
3. After your virtual run, go to the standings — CF Predictor will show an estimated delta based on your result

**What the numbers mean:**
| Rating | Label | What it means |
|--------|-------|--------------|
| < 1200 | Newbie (grey) | Just starting — totally normal at Day 14 |
| 1200–1399 | Pupil (green) | Comfortable with basic problems |
| 1400–1599 | Specialist (cyan) | Solid foundations, can handle Div. 3 |
| 1600–1899 | Expert (blue) | Strong algorithm fundamentals |
| 1900+ | Candidate Master+ | Competitive programmer territory |

Target for 6 months in: reach **Pupil (1200+)**. That would be a meaningful milestone.

---

## Block 2: LeetCode C++ Sampler

### What LeetCode is
LeetCode is the standard platform for technical interview prep. Most big tech companies (including NVIDIA) use LeetCode-style problems in interviews. Problems are tagged by topic (Arrays, Trees, Dynamic Programming, etc.) and difficulty (Easy, Medium, Hard).

Unlike Codeforces, LeetCode doesn't give you a public ELO from random problem-solving — but it does have **Weekly Contests** with a real rating, which you'll start participating in later (probably benchmark #3 or #4 onward).

### Setting up your account
1. Go to [leetcode.com](https://leetcode.com)
2. Click **Sign Up**
3. You can sign up with Google/GitHub for convenience
4. The free tier is sufficient — you don't need LeetCode Premium for this benchmark system

### Picking problems for the benchmark (Easy × 2)
The goal is to pick randomly so you're not unconsciously cherry-picking topics you're comfortable with. Here's how:
1. Go to [leetcode.com/problemset](https://leetcode.com/problemset/)
2. Filter: **Difficulty → Easy**, **Topic → Array** (for your first Easy)
3. Sort by **"Acceptance Rate"** descending to avoid trick problems — pick one from the middle of the list (not the top, not the bottom)
4. For your second Easy: same process but **Topic → String**
5. For the Medium: **Difficulty → Medium**, **Topic → Array** — pick one with 40–60% acceptance rate

### Running a problem
1. Click the problem to open it
2. Read the problem description carefully — don't rush this
3. In the code editor on the right, select **C++** from the language dropdown
4. You'll see a function signature pre-filled — you write your solution inside it
5. Click **Run** to test against the provided examples
6. Click **Submit** when ready — it runs against all hidden test cases

**Time yourself from when you start reading the problem.** Stop the clock when you submit (or give up).

### What "solved" means
- **Solved**: You got `Accepted` on your first or second submit — counts fully
- **Solved with hints**: You looked something up — still counts, note it
- **Not solved**: You ran out of time or couldn't get it — record your approach and where you got stuck

### Tips for writing C++ on LeetCode
- The function signature is given to you — don't change it
- Input comes as function parameters, not `std::cin` — this is different from Codeforces
- You return the answer as the function's return value
- Common patterns you'll see early: `vector<int>`, `string`, basic loops

### LeetCode Weekly Contests (for later)
Once you're solving Mediums consistently (probably benchmark #3+):
1. Weekly contests run every **Sunday at 10:30 AM PT** (check your timezone)
2. Go to [leetcode.com/contest](https://leetcode.com/contest/) to see upcoming contests
3. Each contest has 4 problems (Easy → Hard) in 90 minutes
4. You get a real rating from these — starts around 1500 and moves with your performance
5. **Don't rush into these** — a bad early experience can be discouraging. Wait until you can reliably solve 2 Mediums.

---

## Block 3: Systems C++ Rubric (Self-Assessment)

This block takes ~20 minutes. Be honest — there's no audience, and overrating yourself just makes the signal useless.

### How to score yourself

For each **unlocked** category, go through the skills listed and ask yourself:
- Can I write this without looking anything up? → 3
- Can I write this but I'd need to think and might reference docs? → 2
- I've seen this and could follow it if reading someone else's code? → 1
- Haven't touched this yet? → 0

Then assign a single score for the whole category based on your honest average.

### Making it more rigorous (optional)
For a cleaner signal, before self-assessing: open a blank `.cpp` file and actually try to write a small snippet from memory for each skill. For example, for "References & Pointers", write:
- A function that swaps two ints using references
- A function that takes a nullable pointer and guards with `if (ptr != nullptr)`
- Deliberately create a dangling reference and explain why it's undefined behavior (in a comment)

If you can do that from memory without looking it up → score 3. If you needed to think hard → 2. This takes longer but gives a much more honest score.

---

## Day-of Checklist

Copy this into your notes on benchmark day:

```
[ ] Block 1: Codeforces
    [ ] Open past Div. 4 contest
    [ ] Start virtual participation
    [ ] Start personal 90-min timer
    [ ] Read Problem A, write solution, submit
    [ ] Attempt Problem B if time allows
    [ ] Note problems solved, time, any errors

[ ] Block 2: LeetCode
    [ ] Pick Easy #1 (Array, middle acceptance rate)
    [ ] Pick Easy #2 (String, middle acceptance rate)
    [ ] Pick Medium #1 (Array, 40-60% acceptance rate)
    [ ] Solve each with timer running
    [ ] Note: solved/not, time, what was hard

[ ] Block 3: Rubric
    [ ] Score only unlocked categories
    [ ] Optional: write snippets from memory first
    [ ] Be honest

[ ] Fill in cpp_benchmark_tracker.md
[ ] Write 2-3 sentence overall snapshot
```

---

## Interpreting Your First Results

Your Day 14 results will probably look underwhelming — and that's completely expected. Here's how to frame it:

- **CF estimated rating ~800–1100**: Normal. You've been coding C++ for 2 weeks. The rating floor is ~800 for registered users who haven't competed.
- **LeetCode Easy solved**: If you get both, great. If you get one, fine. The goal at this stage is to see what C++ syntax friction exists (does the problem feel easy but the *code* feels hard to write? That's the signal.)
- **Rubric score like 5/9**: Healthy for Day 14. The value is in watching this grow across benchmarks.

The benchmark is a **baseline**, not a grade. The only number that matters is the trend across benchmarks #1 → #2 → #3.
