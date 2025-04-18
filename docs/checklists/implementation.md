Below is an exhaustive, one‑story‑point checklist—grouped into epics—that covers **every** detail needed to implement the “sss” sorting simulation per the assignment brief. An autonomous AI agent can follow each unchecked box in order.

---

## 📁 Epic: Project scaffolding & build

- [ ] Create root directory for the project  
- [ ] Inside root, create `src/` and `include/` folders  
- [ ] Add a top‑level `Makefile`  

**Makefile tasks**  
- [ ] Define `CC = gcc`  
- [ ] Define `CFLAGS = -Wall -Wextra -I include -std=c11`  
- [ ] Define `LDFLAGS = -pthread`  
- [ ] List `SRC = src/main.c src/sort.c src/file_utils.c src/sync.c`  
- [ ] Compute `OBJ = $(SRC:.c=.o)`  
- [ ] Add target  
  ```Makefile
  sss: $(OBJ)
      $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
  ```  
- [ ] Add pattern rule  
  ```Makefile
  %.o: %.c
      $(CC) $(CFLAGS) -c $< -o $@
  ```  
- [ ] Add `clean` target to remove `$(OBJ)` and the `sss` binary  

---

## 🔒 Epic: Synchronization primitives

### include/sync.h  
- [ ] Add include guard `#ifndef SYNC_H / #define SYNC_H / #endif`  
- [ ] `#include <pthread.h>` and `<stdbool.h>`  
- [ ] Define macros:
  ```c
  #define T1_ID 1
  #define T2_ID 2
  ```
- [ ] Declare shared state:
  ```c
  extern int turn;
  extern bool no_swap_t1, no_swap_t2;
  extern pthread_mutex_t cond_mutex;
  extern pthread_cond_t cond_t1, cond_t2;
  extern long swap_count;
  extern pthread_mutex_t swap_mutex;
  ```
- [ ] Define thread‑arg struct:
  ```c
  typedef struct { int thread_id; } ThreadArgs;
  ```
- [ ] Declare `void init_sync(void);`  
- [ ] Declare `void destroy_sync(void);`  

### src/sync.c  
- [ ] `#include "sync.h"`  
- [ ] Define all externs:
  ```c
  int turn;
  bool no_swap_t1, no_swap_t2;
  pthread_mutex_t cond_mutex;
  pthread_cond_t cond_t1, cond_t2;
  long swap_count;
  pthread_mutex_t swap_mutex;
  ```
- [ ] Implement `init_sync()`:
  - [ ] `pthread_mutex_init(&cond_mutex, NULL);`
  - [ ] `pthread_cond_init(&cond_t1, NULL);`
  - [ ] `pthread_cond_init(&cond_t2, NULL);`
  - [ ] `pthread_mutex_init(&swap_mutex, NULL);`
  - [ ] `turn = T1_ID;`
  - [ ] `no_swap_t1 = no_swap_t2 = false;`
  - [ ] `swap_count = 0;`
- [ ] Implement `destroy_sync()`:
  - [ ] `pthread_mutex_destroy(&cond_mutex);`
  - [ ] `pthread_cond_destroy(&cond_t1);`
  - [ ] `pthread_cond_destroy(&cond_t2);`
  - [ ] `pthread_mutex_destroy(&swap_mutex);`

---

## 📂 Epic: File I/O helper

### include/file_utils.h  
- [ ] Add include guard `#ifndef FILE_UTILS_H / #define FILE_UTILS_H / #endif`  
- [ ] Declare:
  ```c
  int *read_input(const char *filename, int *out_n);
  ```

### src/file_utils.c  
- [ ] `#include <stdio.h>`, `<stdlib.h>`, `<string.h>`, `"file_utils.h"`  
- [ ] Implement `read_input()`:
  - [ ] `FILE *fp = fopen(filename, "r");` → check `fp == NULL` → return `NULL`
  - [ ] Declare `int buffer[200]` and `int count = 0;`
  - [ ] Loop: `while (count < 200 && fscanf(fp, "%d", &buffer[count]) == 1) count++;`
  - [ ] `fclose(fp);`
  - [ ] `int *arr = malloc(count * sizeof(int));` → check `arr == NULL` → return `NULL`
  - [ ] `memcpy(arr, buffer, count * sizeof(int));`
  - [ ] `*out_n = count;`
  - [ ] `return arr;`

---

## 🔢 Epic: Sorting thread logic

### include/sort.h  
- [ ] Add include guard `#ifndef SORT_H / #define SORT_H / #endif`  
- [ ] `#include "sync.h"`  
- [ ] Declare shared globals:
  ```c
  extern int *A;
  extern int n;
  ```
- [ ] Declare:
  ```c
  void *sort(void *arg);
  ```

### src/sort.c  
- [ ] `#include <pthread.h>`, `<stdbool.h>`, `"sync.h"`, `"sort.h"`  
- [ ] At top of `sort()`, declare `int thread_swaps = 0;`  
- [ ] Inside `void *sort(void *arg)`:
  - [ ] Cast `ThreadArgs *targs = arg;`
  - [ ] `int id = targs->thread_id;`
  - [ ] Setup pointers:
    ```c
    bool *my_no_swap    = (id==T1_ID ? &no_swap_t1 : &no_swap_t2);
    bool *other_no_swap = (id==T1_ID ? &no_swap_t2 : &no_swap_t1);
    pthread_cond_t *my_cond    = (id==T1_ID ? &cond_t1 : &cond_t2);
    pthread_cond_t *other_cond = (id==T1_ID ? &cond_t2 : &cond_t1);
    ```
  - [ ] `while (true) {`
    - [ ] `pthread_mutex_lock(&cond_mutex);`
    - [ ] `while (turn != id) pthread_cond_wait(my_cond, &cond_mutex);`
    - [ ] `pthread_mutex_unlock(&cond_mutex);`
    - [ ] `int local_swaps = 0;`
    - [ ] `int start = (id==T1_ID ? 0 : 1);`
    - [ ] Loop over pairs:
      ```c
      for (int i = start; i+1 < n; i += 2) {
          if (A[i] > A[i+1]) {
              int tmp = A[i]; A[i] = A[i+1]; A[i+1] = tmp;
              local_swaps++;
          }
      }
      ```
    - [ ] `pthread_mutex_lock(&swap_mutex);`
    - [ ] `swap_count += local_swaps;`
    - [ ] `pthread_mutex_unlock(&swap_mutex);`
    - [ ] `thread_swaps += local_swaps;`
    - [ ] `pthread_mutex_lock(&cond_mutex);`
    - [ ] `*my_no_swap = (local_swaps == 0);`
    - [ ] Termination check:
      - [ ] If `*my_no_swap && *other_no_swap`:
        - [ ] `turn = (id==T1_ID ? T2_ID : T1_ID);`
        - [ ] `pthread_cond_signal(other_cond);`
        - [ ] Print thread summary:
          ```c
          printf("Thread %d: total number of swaps = %d\n", id, thread_swaps);
          ```
        - [ ] `pthread_mutex_unlock(&cond_mutex);`
        - [ ] `break;`
    - [ ] Else pass control:
      - [ ] `turn = (id==T1_ID ? T2_ID : T1_ID);`
      - [ ] `pthread_cond_signal(other_cond);`
      - [ ] `pthread_mutex_unlock(&cond_mutex);`
  - [ ] `}` (end while)
  - [ ] `return NULL;`

---

## 🎯 Epic: Main program orchestration

### src/main.c  
- [ ] `#include <stdio.h>`, `<stdlib.h>`, `<pthread.h>`, `"file_utils.h"`, `"sync.h"`, `"sort.h"`  
- [ ] Define globals:
  ```c
  int *A = NULL;
  int n    = 0;
  ```
- [ ] `int main(int argc, char **argv) {`
  - [ ] Check `argc != 2` → `fprintf(stderr, "Usage: %s ToSort\n", argv[0]); return EXIT_FAILURE;`
  - [ ] `A = read_input(argv[1], &n);` → if `A == NULL` → `perror("read_input"); return EXIT_FAILURE;`
  - [ ] `init_sync();`
  - [ ] Declare `pthread_t th1, th2;`
  - [ ] Declare and init args:
    ```c
    ThreadArgs t1_args = { .thread_id = T1_ID };
    ThreadArgs t2_args = { .thread_id = T2_ID };
    ```
  - [ ] `if (pthread_create(&th1, NULL, sort, &t1_args) != 0) { perror("pthread_create"); exit(EXIT_FAILURE); }`
  - [ ] `if (pthread_create(&th2, NULL, sort, &t2_args) != 0) { perror("pthread_create"); exit(EXIT_FAILURE); }`
  - [ ] `if (pthread_join(th1, NULL) != 0) { perror("pthread_join"); exit(EXIT_FAILURE); }`
  - [ ] `if (pthread_join(th2, NULL) != 0) { perror("pthread_join"); exit(EXIT_FAILURE); }`
  - [ ] Print sorted array:
    ```c
    for (int i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\n");
    ```
  - [ ] Print total swaps:
    ```c
    printf("Total number of swaps to sort array A = %ld.\n", swap_count);
    ```
  - [ ] `destroy_sync();`
  - [ ] `free(A);`
  - [ ] `return EXIT_SUCCESS;`
- [ ] Add file‑ and function‑level comments explaining flow  

---

## ✅ Epic: Testing & verification

- [ ] Create sample input file `sample1.txt` with the 15‑element example  
- [ ] Create edge‑case file `empty.txt` (0 integers)  
- [ ] Create edge‑case file `one.txt` (1 integer)  
- [ ] Create `max200.txt` with 200 integers  
- [ ] Run `./sss sample1.txt` → compare output to expected sorted array and swap count (29)  
- [ ] Run with each edge case → ensure no crashes, correct behavior  
- [ ] Introduce deliberate malformed input (non‑integer) → verify graceful failure or ignore  

---

## 📚 Epic: Documentation & README

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