My thoughts + notes from "A philosophy of software design"

Identify the requirements of a program.
Is latency important, how fast do responses need to be?

How many edge cases do we need to consider? to try and catch errors.

Is readability and maintenance a priority, how much so?

How important is memory allocation efficiency? How tolerant is mem allocation to failures to free memory or double free errors.
  - who should hold the responsibility to free memory?

---


Commandments
- Be obvious
  - Obvious means less documentation required.

Signs of good software
- time taken to understand how to make a change.
- fewer unknown unknowns, more clearly documented knowns.
- complexity abstracted away, with customization optional

Red flags
- need for extensive documentation
- too many dependencies. tought to assess what is too much . dependencies are the result of functions which reduce complexity