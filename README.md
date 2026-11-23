*This project has been created as part of the 42 curriculum by finarako.*

# Codexion

A multi-threaded concurrency simulation inspired by the Dining Philosophers problem, developed in C using POSIX threads.

---

## Description

**Codexion** simulates $N$ coder threads sitting in a circle sharing $N$ USB dongles. To compile quantum code, each coder requires two adjacent dongles (left and right).

### Lifecycle
1. **Compiling**: Takes left and right dongles, compiles for `time_to_compile` ms.
2. **Debugging**: Releases both dongles, debugs for `time_to_debug` ms.
3. **Refactoring**: Refactors for `time_to_refactor` ms, then attempts to acquire dongles again.

### Core Rules
- **Burnout**: A coder must start compiling within `time_to_burnout` ms from simulation start or their last compilation, or they burn out and the simulation ends.
- **Dongle Cooldown**: Released dongles remain unavailable for `dongle_cooldown` ms.
- **Scheduling**: When multiple coders compete for a dongle, access is arbitrated via:
  - `fifo`: First-In, First-Out (arrival order, tie-broken by coder ID).
  - `edf`: Earliest Deadline First (`last_compile_start + time_to_burnout`, tie-broken by arrival time then ID).

---

## Instructions

### Compilation

```bash
make        # Compile executable 'codexion'
make clean  # Remove object files
make fclean # Remove object files and binary
make re     # Recompile
```

### Execution

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

#### Examples

```bash
# Standard run (5 coders, 7 compiles each, FIFO)
./codexion 5 800 200 200 0 7 0 fifo

# Real-time scheduling with cooldown (5 coders, EDF, 10ms cooldown)
./codexion 5 800 200 200 0 7 10 edf

# Burnout test (1 coder cannot acquire two dongles)
./codexion 1 800 200 200 0 1 0 fifo
```

---

## Thread Synchronization Mechanisms

The implementation coordinates shared resources using standard POSIX threading primitives:

1. **Dongle Protection (`pthread_mutex_t` & `pthread_cond_t`)**:
   - Each `t_dongle` has its own mutex and condition variable.
   - When a coder requests a dongle (`taker_dongle`), it enters the dongle's waiting queue under mutex. If the dongle is busy, cooling down, or another waiter has higher priority, the thread sleeps passively via `pthread_cond_wait()` or `pthread_cond_timedwait()`.
   - Releasing a dongle (`releaser_dongle`) updates its release timestamp and broadcasts (`pthread_cond_broadcast()`) to awaken waiting coders.

2. **Per-Coder State Isolation (`coder_lock`)**:
   - Each coder struct has a dedicated mutex (`coder_lock`) protecting `last_compile_start` and `nbr_compile`.
   - Getters and setters (`get_last_compile`, `set_last_compile`, `get_nbr_compile`, `inc_nbr_compile`) eliminate data races between coder threads and the monitor thread.

3. **Log & Termination Serialization (`config->lock`)**:
   - A global mutex protects `printf` in `action_log()`, ensuring log lines never interleave.
   - Setting `stop = 1` inside the lock immediately silences all subsequent messages once burnout or completion occurs.

---

## Blocking Cases Handled

1. **Deadlock Prevention (Coffman's Conditions)**:
   - *Circular Wait* is broken via asymmetric dongle pickup: coders $1$ to $N-1$ acquire their left dongle first, while coder $N$ acquires their right dongle first. This guarantees a directed acyclic resource graph.
2. **Single Coder**:
   - If `number_of_coders == 1`, the coder takes the single dongle and waits passively until burnout is detected, preventing self-deadlock.
3. **Starvation Prevention & Liveness**:
   - The EDF scheduler dynamically prioritizes the coder closest to burnout.
   - The FIFO scheduler preserves strict arrival order.
   - In odd-numbered rings, a dynamic refactoring offset prevents alternating pairs from indefinitely starving odd neighbors.
4. **Dongle Cooldown without Busy-Waiting**:
   - Cooldown waits use `pthread_cond_timedwait()` with an absolute timestamp (`CLOCK_REALTIME`), avoiding CPU-intensive polling loops.
5. **Precise Burnout Detection**:
   - A dedicated monitor thread polls coder timestamps every 1 ms (`usleep(1000)`), guaranteeing burnout reporting within the 10 ms deadline.

---

## Resources

### References
- **POSIX Threads (IEEE Std 1003.1c)**: Man pages for `pthread_create(3)`, `pthread_mutex(3)`, `pthread_cond(3)`.
- **Operating Systems: Three Easy Pieces (OSTEP)** by Remzi & Andrea Arpaci-Dusseau (Concurrency and Deadlock).
- **Dijkstra, E. W. (1965)**: *Hierarchical Ordering of Sequential Processes* (Dining Philosophers).
- **Liu & Layland (1973)**: *Scheduling Algorithms for Hard-Real-Time Environments* (EDF analysis).

### AI Usage
AI tools were used for:
- **Concurrency Debugging**: Analyzing race condition traces under Valgrind Helgrind/DRD to design fine-grained mutex locks.
- **Scheduler Architecture**: Developing the deterministic tie-breaker logic for FIFO and EDF priority queues.
- **Code Optimization & Norm Compliance**: Refactoring functions to satisfy 42 Norminette constraints (modular files, function line limits).
