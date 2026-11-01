*This project has been created as part of the 42 curriculum by finarako.*

# Codexion

A multi-threaded concurrency simulation inspired by the classic Dining Philosophers problem, developed in C using POSIX threads.

---

## Table of Contents
- [Description](#description)
- [Instructions](#instructions)
- [Thread Synchronization Mechanisms](#thread-synchronization-mechanisms)
- [Blocking Cases Handled](#blocking-cases-handled)
- [Resources & AI Usage](#resources)

---

## Description

**Codexion** is an advanced multithreading and concurrency project within the 42 curriculum. The simulation models a circular cluster of independent software developer threads ("coders") who share a finite set of hardware authentication keys ("USB dongles"). 

In order to perform their work, coders perpetually transition through three distinct states:
1. **Compiling**: A coder must simultaneously hold two dongles (their left and right dongles) to compile code.
2. **Debugging**: Once compilation completes, the coder releases both dongles and spends time debugging.
3. **Refactoring**: After debugging, the coder refactors code before attempting to acquire the dongles again.

### Core Challenge & Constraints
- **Burnout Timer (`time_to_burnout`)**: Each coder has an internal timer. If a coder does not begin compiling within `time_to_burnout` milliseconds from their previous compilation start (or from simulation start), they experience a fatal "burnout" and the simulation immediately terminates.
- **Dongle Cooldown (`dongle_cooldown`)**: Once a dongle is released, it enters a cooldown phase during which no coder may acquire it until the cooldown period has elapsed.
- **Schedulers (`fifo` vs `edf`)**:
  - **FIFO (First In, First Out)**: Prioritizes coders strictly according to their request timestamp (first arrival).
  - **EDF (Earliest Deadline First)**: Prioritizes coders whose burnout deadline is closest, ensuring dynamic real-time liveness.

The objective is to coordinate all threads with zero data races, zero memory leaks, and zero deadlocks, ensuring that no coder starves or burns out when given viable timing parameters.

---

## Instructions

### Compilation

The project is compiled using `cc` with strict compilation flags (`-Wall -Wextra -Werror -pthread`):

```bash
# Standard compilation
make

# Clean object files
make clean

# Full clean (removes objects and binary)
make fclean

# Recompile from scratch
make re
```

### Execution Syntax

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

| Argument | Description | Example |
| :--- | :--- | :--- |
| `number_of_coders` | Total number of coder threads (and dongles) | `5` |
| `time_to_burnout` | Milliseconds before a coder burns out without compiling | `800` |
| `time_to_compile` | Milliseconds spent compiling (holding both dongles) | `200` |
| `time_to_debug` | Milliseconds spent debugging | `200` |
| `time_to_refactor` | Base milliseconds spent refactoring | `0` |
| `number_of_compiles_required` | Simulation stops when all coders compile this many times (0 for infinite/until burnout) | `7` |
| `dongle_cooldown` | Cooldown period in ms before a released dongle can be taken again | `10` |
| `scheduler` | Dongle queue policy: `fifo` or `edf` | `fifo` |

### Execution Examples

1. **Standard Safe Run (5 coders, 7 compilation rounds, FIFO)**:
   ```bash
   ./codexion 5 800 200 200 0 7 0 fifo
   ```
   *Expected outcome*: All coders compile 7 times, no burnout occurs, simulation exits cleanly.

2. **Real-Time Priority Run (5 coders, EDF scheduler, with dongle cooldown)**:
   ```bash
   ./codexion 5 800 200 200 0 7 10 edf
   ```

3. **Single Coder Burnout Test**:
   ```bash
   ./codexion 1 800 200 200 0 1 0 fifo
   ```
   *Expected outcome*: Coder 1 takes the only available dongle, cannot take a second one, burns out at 800 ms, and the program exits without deadlock.

4. **Thread Sanitizer / Concurrency Checks**:
   ```bash
   # Valgrind Helgrind (Thread Error Detector)
   valgrind --tool=helgrind ./codexion 10 800 200 200 50 3 10 fifo

   # Valgrind DRD (Data Race Detector)
   valgrind --tool=drd ./codexion 10 800 200 200 50 3 10 edf

   # Valgrind Memcheck (Memory Leak Detector)
   valgrind --tool=memcheck --leak-check=full ./codexion 5 800 200 200 0 2 0 fifo
   ```

---

## Thread Synchronization Mechanisms

The implementation relies exclusively on standard POSIX thread primitives (`pthread_mutex_t`, `pthread_cond_t`) organized around fine-grained access control:

### 1. Dongle Coordination (`t_dongle`)
Each dongle contains:
- `pthread_mutex_t mutex`: Protects the internal state of the dongle (`is_taken`, `last_released_time`, `waiters` queue).
- `pthread_cond_t cond`: Condition variable used to put competing coders to sleep without busy-waiting.
- `t_coder *waiters[4]`: Queue of coders currently waiting to acquire this dongle.

When a coder enters `taker_dongle()`:
1. It locks `dongle->mutex` and registers itself in `dongle->waiters`.
2. It evaluates priority via `has_highest_priority()`.
3. If the dongle is occupied or another waiter has higher priority, the coder calls `pthread_cond_wait(&dongle->cond, &dongle->mutex)`, relinquishing the lock and sleeping passively.
4. When `releaser_dongle()` is called, it updates `last_released_time` and broadcasts via `pthread_cond_broadcast(&dongle->cond)`. All waiting coders awaken to re-evaluate eligibility under the active scheduler.

### 2. State & Meal Protection (`coder_lock`)
Each coder struct possesses an independent mutex `coder_lock` that serializes access to:
- `last_compile_start`: Written by the coder thread when compilation starts; read concurrently by the monitor thread to detect burnout, and by the scheduler to compute remaining deadlines.
- `nbr_compile`: Incremented by the coder; read by `all_coders_done()`.

Encapsulated helper functions (`get_last_compile()`, `set_last_compile()`, `get_nbr_compile()`, `inc_nbr_compile()`) guarantee strict mutual exclusion, eliminating data races reported by Helgrind and DRD.

### 3. Log Serialization & Global Stop State (`config->lock`)
A centralized mutex `config->lock` manages:
- **Terminal Output**: `action_log()` locks `config->lock`, checks if `config->stop` is zero, and emits formatted timestamps.
- **Immediate Output Termination**: When a coder burns out, `action_log()` sets `config->stop = 1` inside the same critical section as `printf()`. Any subsequent messages from other threads are suppressed.
- **Broadcast on Termination**: When `set_stop(config, 1)` is triggered, all dongle condition variables are broadcast to immediately wake up any waiting threads so they can terminate cleanly.

---

## Blocking Cases Handled

### 1. Deadlock Prevention & Coffman's Conditions
Deadlock can only occur if all four Coffman conditions hold simultaneously:
1. *Mutual Exclusion*: Hardware dongles cannot be shared simultaneously.
2. *Hold and Wait*: A coder holds one dongle while waiting for another.
3. *No Preemption*: Dongles cannot be forcibly confiscated.
4. *Circular Wait*: A closed loop of threads exists where each thread waits for a resource held by the next.

**Resolution**: We eliminate condition 4 (*Circular Wait*) by enforcing **asymmetric resource acquisition**:
- Coders `1` through `N - 1` acquire their `left_dongle` first, then their `right_dongle`.
- The last coder (`N`) breaks the cycle by acquiring their `right_dongle` first, then their `left_dongle`.
- This ensures a directed acyclic resource allocation graph, making a circular deadlock mathematically impossible.

### 2. Single Coder Edge Case (`number_of_coders == 1`)
With one coder, both `left_dongle` and `right_dongle` reference the same hardware dongle (`dongles[0]`). If unhandled, the coder acquires `left_dongle`, then attempts to acquire `right_dongle`, deadlocking with itself.
- **Resolution**: `coder_behaviour()` explicitly detects `num_coders == 1`, acquires the single dongle, logs the action, and enters a passive sleep loop until the monitor announces burnout.

### 3. Starvation Prevention & Real-Time Scheduling
In circular rings with an odd number of threads, alternating pairs can form a resonant cycle that starves the odd neighbor.
- **Resolution**:
  - **FIFO Mode**: Tracks exact arrival times in `request_time`. A newly arrived coder cannot barge in front of an earlier waiter.
  - **EDF Mode**: Coders with the smallest remaining lifespan (`last_compile_start + time_to_burnout`) are always served first.
  - **Fair Refactoring**: In odd topologies without artificial refactoring times, a dynamic pause (`(compile * 2 - debug)`) is applied to allow the starved neighbor to capture both dongles.

### 4. Dongle Cooldown Handling without Spin-Locks
A naive cooldown implementation spins on `pthread_cond_timedwait` with past timestamps, consuming 100% CPU.
- **Resolution**: When a dongle is free but under cooldown, `wait_cooldown()` calculates the exact remaining cooldown duration, adds it to `CLOCK_REALTIME`, and executes a single passive `pthread_cond_timedwait`.

### 5. Precise Burnout Detection
The monitor thread polls all coder timestamps at a high-frequency interval (`1000 µs` / 1 ms). This guarantees that a burnout is detected and reported within 1 millisecond of the limit, rather than drifting due to coarse sleeping intervals.

---

## Resources

### References
- **POSIX Threads (IEEE Std 1003.1c)**: Man pages for `pthread_create(3)`, `pthread_mutex_init(3)`, `pthread_cond_wait(3)`.
- **Operating Systems: Three Easy Pieces (OSTEP)** — Remzi H. Arpaci-Dusseau & Andrea C. Arpaci-Dusseau (Chapters on Concurrency, Locks, Condition Variables, Semaphores, and Deadlock).
- **Dijkstra, E. W. (1965)**: *Hierarchical Ordering of Sequential Processes* (Classic Dining Philosophers solution and resource hierarchy).
- **Liu, C. L., & Layland, J. W. (1973)**: *Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment* (Earliest Deadline First analysis).
- **Valgrind Documentation**: *Helgrind: a thread error detector* and *DRD: a thread error detector*.

### AI Usage
Artificial Intelligence (LLM) was utilized during the development of this project for the following specific engineering tasks:
1. **Concurrency Diagnosis & Thread Profiling**:
   - Diagnosing data races reported by Valgrind Helgrind and DRD on shared variables (`stop`, `last_compile_start`, `nbr_compile`).
   - Identifying the busy-spin bug in the original cooldown implementation caused by invalid `timespec` timestamps.
2. **Architecture & Scheduler Design**:
   - Designing the `waiters` queue and priority comparator for `FIFO` and `EDF` scheduling in `scheduler.c`.
   - Designing the thread-safe encapsulation model with dedicated `coder_lock` mutexes.
3. **Refactoring & Code Quality**:
   - Refactoring the codebase into modular compilation units (`utils.c`, `scheduler.c`) to satisfy 42 Norminette limits (max 5 functions per file, max 25 lines per function).
   - Validating edge cases such as the single coder deadlock and broadcast wakeups upon termination.

