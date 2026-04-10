# RESULTS.md — Assignment 5: User-Level Threads and Mutex (xv6)

## Overview

This assignment implements a cooperative user-level threading library in xv6. The system supports thread creation, yielding, joining, and synchronization using a mutex. All functionality is implemented in user space without modifying the kernel.

---

## Context Switching

Context switching is handled by the `uswtch` assembly routine. Each thread maintains its own stack and saved context (EIP, ESP, and callee-saved registers). When a thread calls `thread_yield()`, its context is saved and another thread’s context is restored, effectively switching execution.

Threads begin execution through a `thread_stub` function, which runs the user-defined function and marks the thread as finished (ZOMBIE).

---

## Scheduling

A round-robin cooperative scheduler is used. Threads must explicitly call `thread_yield()` to allow others to run. The scheduler cycles through RUNNABLE threads and ensures only one thread runs at a time. There is no preemption.

---

## Mutex Implementation

A simple mutex is implemented using a spin-wait approach. If a lock is unavailable, the thread repeatedly yields until it becomes free. This works correctly because scheduling is cooperative and avoids race conditions in shared data.

---

## Testing (Producer-Consumer)

The implementation was tested using a bounded-buffer producer-consumer program with multiple producer threads and one consumer thread. A mutex was used to protect shared variables.

### Results:

* The program executed successfully and terminated normally
* Output showed consistent progress (e.g., items being produced and consumed)
* No deadlocks or crashes occurred

---

## Observations

* Threads executed correctly and shared memory safely
* Mutex ensured proper synchronization
* `thread_join()` correctly waited for thread completion

---

## Limitations

* Fixed maximum number of threads (8)
* Fixed stack size per thread
* No preemption (requires explicit yielding)

---

## Conclusion

The threading system functions correctly and demonstrates cooperative scheduling and synchronization in user space. All assignment requirements were successfully met.

