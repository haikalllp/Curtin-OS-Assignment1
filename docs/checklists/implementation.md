Below is an exhaustive, one‑story‑point checklist—grouped into epics—that covers **every** detail needed to implement the “sss” sorting simulation per the assignment brief. An autonomous AI agent can follow each unchecked box in order.

---

## 📁 Epic: Project scaffolding & build (Setup)

- [x] Create root directory for the project  
- [x] Inside root, create `src/` and `include/` folders  
- [x] Add a top‑level `Makefile`  

**Makefile tasks**  
- [x] Define `CC = gcc`  
- [x] Define `CFLAGS = -Wall -Wextra -I include -std=c11`  
- [x] Define `LDFLAGS = -pthread`  
- [x] List `SRC = src/main.c src/sort.c src/file_utils.c src/sync.c`  
- [x] Compute `OBJ = $(SRC:.c=.o)`  
- [x] Add target  
  ```Makefile
  sss: $(OBJ)
      $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
  ```  
- [x] Add pattern rule  
  ```Makefile
  %.o: %.c
      $(CC) $(CFLAGS) -c $< -o $@
  ```  
- [x] Add `clean` target to remove `$(OBJ)` and the `sss` binary  

---

## 🔒 Epic: Synchronization primitives (Task 1)

### include/sync.h  
- [x] Add include guard `#ifndef SYNC_H / #define SYNC_H / #endif`  
- [x] `#include <pthread.h>` and `<stdbool.h>`  
- [x] Define macros:
  ```c
  #define T1_ID 1
  #define T2_ID 2
  ```
- [x] Declare shared state:
  ```c
  extern int turn;
  extern bool no_swap_t1, no_swap_t2;
  extern pthread_mutex_t cond_mutex;
  extern pthread_cond_t cond_t1, cond_t2;
  extern long swap_count;
  extern pthread_mutex_t swap_mutex;
  ```
- [x] Define thread‑arg struct:
  ```c
  typedef struct { int thread_id; } ThreadArgs;
  ```
- [x] Declare `void init_sync(void);`  
- [x] Declare `void destroy_sync(void);`  

### src/sync.c  
- [x] `#include "sync.h"`  
- [x] Define all externs:
  ```c
  int turn;
  bool no_swap_t1, no_swap_t2;
  pthread_mutex_t cond_mutex;
  pthread_cond_t cond_t1, cond_t2;
  long swap_count;
  pthread_mutex_t swap_mutex;
  ```
- [x] Implement `init_sync()`:
  - [x] `pthread_mutex_init(&cond_mutex, NULL);`
  - [x] `pthread_cond_init(&cond_t1, NULL);`
  - [x] `pthread_cond_init(&cond_t2, NULL);`
  - [x] `pthread_mutex_init(&swap_mutex, NULL);`
  - [x] `turn = T1_ID;`
  - [x] `no_swap_t1 = no_swap_t2 = false;`
  - [x] `swap_count = 0;`
- [x] Implement `destroy_sync()`:
  - [x] `pthread_mutex_destroy(&cond_mutex);`
  - [x] `pthread_cond_destroy(&cond_t1);`
  - [x] `pthread_cond_destroy(&cond_t2);`
  - [x] `pthread_mutex_destroy(&swap_mutex);`

---

## 📂 Epic: File I/O helper (Task 2)

### include/file_utils.h  
- [x] Add include guard `#ifndef FILE_UTILS_H / #define FILE_UTILS_H / #endif`  
- [x] Declare:
  ```c
  int *read_input(const char *filename, int *out_n);
  ```

### src/file_utils.c  
- [x] `#include <stdio.h>`, `<stdlib.h>`, `<string.h>`, `"file_utils.h"`  
- [x] Implement `read_input()`:
  - [x] `FILE *fp = fopen(filename, "r");` → check `fp == NULL` → return `NULL`
  - [x] Declare `int buffer[200]` and `int count = 0;`
  - [x] Loop: `while (count < 200 && fscanf(fp, "%d", &buffer[count]) == 1) count++;`
  - [x] `fclose(fp);`
  - [x] `int *arr = malloc(count * sizeof(int));` → check `arr == NULL` → return `NULL`
  - [x] `memcpy(arr, buffer, count * sizeof(int));`
  - [x] `*out_n = count;`
  - [x] `return arr;`

---

## 🔢 Epic: Sorting thread logic (Task 3)

### include/sort.h  
- [x] Add include guard `#ifndef SORT_H / #define SORT_H / #endif`  
- [x] `#include "sync.h"`  
- [x] Declare shared globals:
  ```c
  extern int *A;
  extern int n;
  ```
- [x] Declare:
  ```c
  void *sort(void *arg);
  ```

### src/sort.c  
- [x] `#include <pthread.h>`, `<stdbool.h>`, `"sync.h"`, `"sort.h"`  
- [x] At top of `sort()`, declare `int thread_swaps = 0;`  
- [x] Inside `void *sort(void *arg)`:
  - [x] Cast `ThreadArgs *targs = arg;`
  - [x] `int id = targs->thread_id;`
  - [x] Setup pointers:
    ```c
    bool *my_no_swap    = (id==T1_ID ? &no_swap_t1 : &no_swap_t2);
    bool *other_no_swap = (id==T1_ID ? &no_swap_t2 : &no_swap_t1);
    pthread_cond_t *my_cond    = (id==T1_ID ? &cond_t1 : &cond_t2);
    pthread_cond_t *other_cond = (id==T1_ID ? &cond_t2 : &cond_t1);
    ```
  - [x] `while (true) {`
    - [x] `pthread_mutex_lock(&cond_mutex);`
    - [x] `while (turn != id) pthread_cond_wait(my_cond, &cond_mutex);`
    - [x] `pthread_mutex_unlock(&cond_mutex);`
    - [x] `int local_swaps = 0;`
    - [x] `int start = (id==T1_ID ? 0 : 1);`
    - [x] Loop over pairs:
      ```c
      for (int i = start; i+1 < n; i += 2) {
          if (A[i] > A[i+1]) {
              int tmp = A[i]; A[i] = A[i+1]; A[i+1] = tmp;
              local_swaps++;
          }
      }
      ```
    - [x] `pthread_mutex_lock(&swap_mutex);`
    - [x] `swap_count += local_swaps;`
    - [x] `pthread_mutex_unlock(&swap_mutex);`
    - [x] `thread_swaps += local_swaps;`
    - [x] `pthread_mutex_lock(&cond_mutex);`
    - [x] `*my_no_swap = (local_swaps == 0);`
    - [x] Termination check:
      - [x] If `*my_no_swap && *other_no_swap`:
        - [x] `turn = (id==T1_ID ? T2_ID : T1_ID);`
        - [x] `pthread_cond_signal(other_cond);`
        - [x] Print thread summary:
          ```c
          printf("Thread %d: total number of swaps = %d\n", id, thread_swaps);
          ```
        - [x] `pthread_mutex_unlock(&cond_mutex);`
        - [x] `break;`
    - [x] Else pass control:
      - [x] `turn = (id==T1_ID ? T2_ID : T1_ID);`
      - [x] `pthread_cond_signal(other_cond);`
      - [x] `pthread_mutex_unlock(&cond_mutex);`
  - [x] `}` (end while)
  - [x] `return NULL;`

---

## 🎯 Epic: Main program orchestration (Task 4)

### src/main.c  
- [x] `#include <stdio.h>`, `<stdlib.h>`, `<pthread.h>`, `"file_utils.h"`, `"sync.h"`, `"sort.h"`  
- [x] Define globals:
  ```c
  int *A = NULL;
  int n    = 0;
  ```
- [x] `int main(int argc, char **argv) {`
  - [x] Check `argc != 2` → `fprintf(stderr, "Usage: %s ToSort\n", argv[0]); return EXIT_FAILURE;`
  - [x] `A = read_input(argv[1], &n);` → if `A == NULL` → `perror("read_input"); return EXIT_FAILURE;`
  - [x] `init_sync();`
  - [x] Declare `pthread_t th1, th2;`
  - [x] Declare and init args:
    ```c
    ThreadArgs t1_args = { .thread_id = T1_ID };
    ThreadArgs t2_args = { .thread_id = T2_ID };
    ```
  - [x] `if (pthread_create(&th1, NULL, sort, &t1_args) != 0) { perror("pthread_create"); exit(EXIT_FAILURE); }`
  - [x] `if (pthread_create(&th2, NULL, sort, &t2_args) != 0) { perror("pthread_create"); exit(EXIT_FAILURE); }`
  - [x] `if (pthread_join(th1, NULL) != 0) { perror("pthread_join"); exit(EXIT_FAILURE); }`
  - [x] `if (pthread_join(th2, NULL) != 0) { perror("pthread_join"); exit(EXIT_FAILURE); }`
  - [x] Print sorted array:
    ```c
    for (int i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\n");
    ```
  - [x] Print total swaps:
    ```c
    printf("Total number of swaps to sort array A = %ld.\n", swap_count);
    ```
  - [x] `destroy_sync();`
  - [x] `free(A);`
  - [x] `return EXIT_SUCCESS;`
- [x] Add file‑ and function‑level comments explaining flow  

---

## ✅ Epic: Testing & verification (Task 5)

- [x] Create sample input file `sample1.txt` with the 15‑element example  
- [x] Create edge‑case file `empty.txt` (0 integers)  
- [x] Create edge‑case file `one.txt` (1 integer)  
- [x] Create `max200.txt` with 200 integers  
- [x] Run `./sss sample1.txt` → compare output to expected sorted array and swap count (29)  
- [x] Run with each edge case → ensure no crashes, correct behavior  
- [x] Introduce deliberate malformed input (non‑integer) → verify graceful failure or ignore  

---

## 📚 Epic: Documentation & README (Task 6)

- [ ] Create `README.md` in project root  
- [ ] Write **Project Overview** summarizing purpose and threads  
- [ ] Add **Build instructions**:
  ```markdown
  make
  ```
- [ ] Add **Run instructions**:
  ```markdown
  ./sss ToSort
  ```
- [ ] Document **Dependencies** (GCC ≥9, POSIX threads)  
- [ ] Describe **Design**:  
  - thread alternation  
  - shared swap counter  
  - termination condition  
- [ ] List **Known limitations** (max 200 ints, C11)  
- [ ] Add **License or Author** section if required  

---