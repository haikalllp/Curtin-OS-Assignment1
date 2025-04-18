```markdown
# Rules for Achieving Full Marks on COMP2006 “sss” Assignment

Follow these rules **exactly**—they map 1:1 to the assignment brief’s requirements and the unit outline’s marking criteria.

---

## 1. Specification Compliance

1. **Program name & invocation**  
   - Executable must be called `sss`.  
   - Must run as:  
     ```bash
     ./sss ToSort
     ```
2. **Input handling**  
   - Read up to 200 integers from a file named by the single CLI argument.  
   - Whitespace‑separated, any valid integer.  
   - On failure (e.g. file not found), print an error via `perror()` and exit with non‑zero.
3. **Data structures**  
   - Store integers in a dynamically allocated array `int *A` of length `n`.  
   - Use a `long swap_count` shared across threads.
4. **Threads & sorting logic**  
   - Create two POSIX threads (T1, T2) via `pthread_create()`, both running `sort()`.  
   - T1 compares/swaps pairs `(A[2i], A[2i+1])` for `i=0…⌊(n−1)/2⌋`.  
   - T2 compares/swaps pairs `(A[2i−1], A[2i])` for `i=1…⌊(n−1)/2⌋`.  
   - Each thread accumulates its own swap count and adds to the global `swap_count`.
5. **Inter‑thread coordination**  
   - Strict alternation: T1 runs a full pass, signals T2; T2 runs a full pass, signals T1.  
   - Use `pthread_mutex_t cond_mutex` plus two `pthread_cond_t` variables to implement turn‑taking.
6. **Termination condition**  
   - Track two flags `no_swap_t1`, `no_swap_t2`.  
   - If one thread’s pass yields zero swaps **and** the other’s previous pass also yielded zero swaps, both must terminate.  
   - Each thread, on exit, prints:  
     ```
     Thread <ID>: total number of swaps = <its_swap_count>
     ```
7. **Final output**  
   - After joining both threads, the main thread prints the sorted array on one line, then:  
     ```
     Total number of swaps to sort array A = <swap_count>.
     ```

---

## 2. Code Organization & File Structure

- **Project root**  
  - `Makefile`  
  - `README.md`  
  - `report.pdf`  
- **Source & headers**  
  ```
  include/
    ├─ sync.h
    ├─ file_utils.h
    └─ sort.h
  src/
    ├─ main.c
    ├─ sync.c
    ├─ file_utils.c
    └─ sort.c
  ```
- **README.md**  
  - Must include build/run instructions, dependencies, and a brief design summary.

---

## 3. Build & Dependencies

- **Compiler**: `gcc` (version ≥ 9, supports C11)  
- **Flags**:  
  ```make
  CFLAGS = -Wall -Wextra -std=c11 -I include
  LDFLAGS = -pthread
  ```
- **Makefile**  
  - Targets: `sss`, `clean`.  
  - Pattern rule for `.c → .o`.  
  - Linking must include `-pthread`.

---

## 4. Coding Standards & Style

- **Language standard**: ISO C11 (or C99 minimum)  
- **Indentation**: 4 spaces per level, no tabs.  
- **Comments**:  
  - File header comment with name, student ID, brief purpose.  
  - Function header comments: description, parameters, return values.  
  - Inline comments for complex logic (e.g. turn‑taking, termination test).  
- **Naming**:  
  - `snake_case` for variables and functions.  
  - `ThreadArgs`, `T1_ID`, `swap_count`, etc.  
- **No debug cruft**: remove all `printf()` left over from debugging.

---

## 5. Synchronization & Concurrency Rules

- **Mutex granularity**:  
  - One `cond_mutex` for `turn` and `no_swap_*` flags.  
  - One `swap_mutex` for protecting `swap_count`.
- **Condition variables**:  
  - `cond_t1` for T1, `cond_t2` for T2.  
  - Always wait in a loop:  
    ```c
    pthread_mutex_lock(&cond_mutex);
    while (turn != MY_ID)
        pthread_cond_wait(&my_cond, &cond_mutex);
    pthread_mutex_unlock(&cond_mutex);
    ```
- **Termination signal**:  
  - When detecting two consecutive no‑swap passes, set `turn` to the other thread, signal its condvar, then exit.

---

## 6. File I/O Rules

- Use `fopen()`, `fscanf()` in `read_input()` (in `file_utils.c`).  
- Do not leak file descriptors: always `fclose()`.  
- Dynamically allocate output array via `malloc()`, copy from local buffer, return pointer.

---

## 7. Testing & Validation

- **Sample files**:  
  - `sample1.txt` (the 15‑element example).  
  - `empty.txt`, `one.txt`, `max200.txt`.  
  - Malformed input (non‑integer tokens).  
- **Test steps**:  
  1. `make clean && make`  
  2. `./sss sample1.txt` → compare to expected sorted output & swap count (29).  
  3. Repeat for edge cases; ensure graceful behavior.  
- Document test results in `report.pdf`.

---

## 8. Report & Submission (ignore until project fully done)

- **report.pdf** (< 6 pages of content + cover page) must include:  
  1. **Cover page** (signed declaration of originality).  
  2. **Discussion of shared data structures & synchronization** (what is shared, which thread accesses it, which mutex/cond protects it).  
  3. **Limitations & unhandled cases**.  
  4. **Sample inputs & outputs** with correctness commentary.  
  5. **Full source code listing** (identical to what is in `src/` and `include/`).  
- **ZIP package** named `FirstName_FamilyName_ID.zip` containing:  
  ```
  src/
  include/
  Makefile
  README.md
  report.pdf
  ```
- **Upload** to Blackboard—before **5 May 2025, 17:00** local time.

---

By following **every** rule in this document, you will satisfy **all** assignment requirements and secure full marks. Good luck!