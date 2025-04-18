# Operating Systems Assignment

## Objective
The objective of this programming assignment is to give you some experiences in using 
multiple threads and inter-thread communications. You will learn how to create threads and 
solve the critical section problems.

## A. Description

1. Your assignment is to simulate a simple algorithm to sort n integers using two threads: T1 
   and T2.
   
   > Note that you don't need to know any other sorting algorithm to complete this assignment. More specifically, you just need to apply the steps described below.

2. Let the integers be stored in an array A of size n. For example, consider an array A = (5, 3, 
   11, 2, 1, 4, 5, 1, 10, 11, 21, 17, 25, 16, 6), i.e., n = 15. Assume array A starts at location 0 
   and thus it ends at location n – 1, i.e., A[0] = 5, A[1] = 3, ..., and A[14] = 6.

3. Thread T1 repeatedly checks each pair of integers at locations A[2i] and A[2i+1], for i = 0, 
   1, ..., (n-1)/2. For each pair of integers, T1 will swap them if they are not in order and 
   updates the total number of swaps in a shared variable named swap.

   > Note: [x/y] is a floor function that returns the closest integer value lower than x/y. For 
   > example, [15/2] = [7.5] = 7, [16/2] = [8.0] = 8, and [17/3] = [5.667] = 5.

   For example, for n = 15, T1 will sort the integers at A[0] and A[1], A[2] and A[3], A[4] and 
   A[5], A[6] and A[7], A[8] and A[9], A[10] and A[11], A[12] and A[13], A[14] and A[15]. 
   Note that there is NO A[15] in the example. Thus, T1 will keep the content of A[14].

4. Thread T2 repeatedly checks each pair of integers at locations A[2i – 1] and A[2i], for i = 
   1, ..., (n-1)/2. For each pair of elements, T2 will swap them if they are not in order and 
   updates the total number of swaps in a shared variable named swap.

   For example, for n = 15, T2 will sort the integers at A[1] and A[2], A[3] and A[4], A[5] and 
   A[6], A[7] and A[8], A[9] and A[10], A[11] and A[12], A[13] and A[14].

5. Threads T1 and T2 will stop their repetition, i.e., terminate, if:
   1. There is no swap in T1 followed by there is no swap in T2, or
   2. There is no swap in T2 followed by there is no swap in T1.
   
   In other words, the repetition stops when array A is already sorted.
   
   3. Once T1 and T2 terminate, the parent thread prints the contents of the sorted array A, and 
      the total number of swaps, i.e., the value of the shared variable swap to the screen.

---

## Example Execution

The following example shows how the simple sorting simulation works. Notice that in each 
iteration, T2 will start sorting only after T1 completes its iteration. Furthermore, T1 will start 
sorting only after T2 completes in the previous iteration.

### Iteration 1
**T1**: For A = (5, 3, 11, 2, 1, 4, 5, 1, 10, 11, 21, 17, 25, 16, 6), P1 will sort the array into 
A = (3, 5, 2, 11, 1, 4, 1, 5, 10, 11, 17, 21, 16, 25, 6). There are 5 swaps (see the pairs in 
bold). Thus, P1 will update variable swap = 0 + 5 = 5.

**T2**: For A = (3, 5, 2, 11, 1, 4, 1, 5, 10, 11, 17, 21, 16, 25, 6), P2 will sort the array into  
A = (3, 2, 5, 1, 11, 1, 4, 5, 10, 11, 17, 16, 21, 6, 25). There are 5 swaps. Thus, P2 will update 
swap = 5 + 5 = 10.

### Iteration 2
**T1**: For A = (3, 2, 5, 1, 11, 1, 4, 5, 10, 11, 17, 16, 21, 6, 25), P1 will sort the array into   
A = (2, 3, 1, 5, 1, 11, 4, 5, 10, 11, 16, 17, 6, 21, 25). There are 5 swaps. Thus, P1 will update 
swap = 10 + 5 = 15.

**T2**: For A = (2, 3, 1, 5, 1, 11, 4, 5, 10, 11, 16, 17, 6, 21, 25), P2 will sort the array into 
A = (2, 1, 3, 1, 5, 4, 11, 5, 10, 11, 16, 6, 17, 21, 25). There are 4 swaps. Thus, P2 will update 
swap = 15 + 4 = 19.

### Iteration 3
**T1**: For A = (2, 1, 3, 1, 5, 4, 11, 5, 10, 11, 16, 6, 17, 21, 25), P2 will sort the array into 
A = (1, 2, 1, 3, 4, 5, 5, 11, 10, 11, 6, 16, 17, 21, 25). There are 5 swaps. Thus, P1 will update 
swap = 19 + 5 = 24.

**T2**: For A = (1, 2, 1, 3, 4, 5, 5, 11, 10, 11, 6, 16, 17, 21, 25), P2 will sort the array into 
A = (1, 1, 2, 3, 4, 5, 5, 10, 11, 6, 11, 16, 17, 21, 25). There are 3 swaps. Thus, P2 will update 
swap = 24 + 3 = 27.

### Iteration 4
**T1**: For A = (1, 1, 2, 3, 4, 5, 5, 10, 11, 6, 11, 16, 17, 21, 25), P1 will sort the array into 
A = (1, 1, 2, 3, 4, 5, 5, 10, 6, 11, 11, 16, 17, 21, 25). There is 1 swap. Thus, P1 will update 
swap = 27 + 1 = 28.

**T2**: For A = (1, 1, 2, 3, 4, 5, 5, 10, 6, 11, 11, 16, 17, 21, 25), P2 will sort the array into 
A = (1, 1, 2, 3, 4, 5, 5, 6, 10, 11, 11, 16, 17, 21, 25). There is 1 swap. Thus, P2 will update 
swap = 28 + 1 = 29.

### Iteration 5
**T1**: For A = (1, 1, 2, 3, 4, 5, 5, 6, 10, 11, 11, 16, 17, 21, 25), P1 will sort the array into 
A = (1, 1, 2, 3, 4, 5, 5, 6, 10, 11, 11, 16, 17, 21, 25) → No change: 0 swap. Thus, P1 will 
update swap = 29 + 0 = 29.

**T2**: For A = (1, 1, 2, 3, 4, 5, 5, 6, 10, 11, 11, 16, 17, 21, 25), P2 will sort the array into 
A = (1, 1, 2, 3, 4, 5, 5, 6, 10, 11, 11, 16, 17, 21, 25) → No change: 0 swap. Thus, P2 will 
update swap = 29 + 0 = 29.

There are two consecutive "No Change", meaning the array is already sorted. Thus, both T1 
and T2 terminate.

Each terminating thread prints its thread ID and the total number of swaps it has completed. For 
the example, thread T1 will print the following on the screen, where ID1 is the thread ID for 
thread T1:

```
Thread ID1: total number of swaps = 16
```

Similarly, thread T2 will print the following:

```
Thread ID2: total number of swaps = 13
```

The main thread will then print the sorted array, and the total number of swaps, i.e.,

```
A = (1, 1, 2, 3, 4, 5, 5, 6, 10, 11, 11, 16, 17, 21, 25)
Total number of swaps to sort array A = 29.
```

---

## Implementation

1. Write a program in C language that implements a simple sorting simulation using two child 
   threads: T1 and T2. Both threads run the same function called `sort()`. Use `pthread_create()` 
   to create a new thread so that the thread runs function `sort()`, and if the thread is T1 or T2. 
   The integers to be sorted in stored in a file named `ToSort`. The file contains at maximum 
   of n = 200 integers, where each pair of integers is separated by a space. Use the C version 
   that is supported by the computers available in our computing laboratory.

2. Given a set of n integers in file `ToSort`, the main program / thread does the following:

   1. Store the n integers in file `ToSort` into an array A and initialize a variable swap to zero.
  
   2. Creates two child threads: T1, and T2.
 
   3. Make T1 sort the integers at each pair A[2i] and A[2i + 1], for i = 0, 1, ..., (n-1)/2, 
      and T2 sort the integers at each pair A[2i – 1] and A[2i], for i = 1, ..., (n-1)/2. Notice 
      that T1 and T2 must run concurrently.

   4. While waiting for T1 and T2 to finish their sorting steps, the main thread must block. 
      It will be unblocked by one of the child threads. Use `pthread_cond_wait()` and 
      `pthread_cond_signal()` for the block and unblock.
 
   5. Once T1 and T2 finish their sorting, i.e., they terminate, the main thread prints the 
      sorted integers in array A and the total number of swaps in the shared variable swap to 
      the screen.

3. For each iteration, T2 must block waiting for T1 to complete its sorting, which will unblock 
   T2. Similarly, T1 in each iteration k must block waiting for T2 to complete its sorting in 
   the earlier iteration k–1 that will unblock T1. Use `pthread_cond_wait()` and 
   `pthread_cond_signal()` for the block and unblock.

4. You are allowed to use any additional variables. For each shared variable, you must address 
   any possible synchronization issue.

5. Let us call the executable for the parent thread as `sss`. The program should be run as:

   ```
   sss ToSort
   ```

   where `ToSort` is the name of a file that contains n integers to be sorted.