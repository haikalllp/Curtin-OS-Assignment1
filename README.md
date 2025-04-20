# OS Assignment 1 - Shared Sort Simulator (sss)

## Project Overview

This program implements a simple sorting algorithm using two concurrent threads. The algorithm works as follows:

- Thread T1 compares and swaps pairs of integers at positions A[2i] and A[2i+1]
- Thread T2 compares and swaps pairs of integers at positions A[2i-1] and A[2i]
- The threads strictly alternate, with T1 always executing first in each iteration
- Both threads terminate when no swaps occur in consecutive passes by both threads

This implementation demonstrates the use of POSIX threads, mutexes, and condition variables for thread synchronization. The shared array and swap counter are protected with appropriate mutual exclusion mechanisms.

## Build Instructions

```
make
```

## Run Instructions

```
./sss <file.txt>
```

Where `<file.txt>` is the input file containing integers to be sorted. The program will read the integers from the file, sort them using the shared sort algorithm, and print the sorted array along with the number of swaps performed by each thread.

Where `ToSort` is a file containing integers separated by whitespace.

## Memory Leak Detection

To check for memory leaks, you can use the Valgrind target:

```
make valgrind
```

This will run the program through Valgrind with the following settings:
- Full leak checking
- Display all types of memory leaks
- Track origins of uninitialized values
- Verbose output
- Uses sample1.txt as default input file

You can also specify a different input file:

```
make valgrind INPUT=<file.txt>
```

Alternatively, you can run a specific file through Valgrind:

```
make
valgrind --leak-check=full ./sss <file.txt>
```

## Clean build
```
make clean
```

## Dependencies

- GCC 9 or higher (with C11 support)
- POSIX threads library

## Design

### Thread Alternation

The program implements strict alternation between the two threads using:
- A shared `turn` variable to track which thread can execute
- Two condition variables (`cond_t1`, `cond_t2`) for thread signaling
- A mutex (`cond_mutex`) to protect access to shared synchronization variables

### Shared Swap Counter

The total number of swaps is tracked in a shared variable protected by:
- A separate mutex (`swap_mutex`) to prevent race conditions when updating
- Each thread maintains a local counter for its own swaps before updating the shared count

### Termination Condition

The threads terminate when:
- Both threads have consecutive passes with no swaps
- This is tracked through the `no_swap_t1` and `no_swap_t2` flags
- When both flags are true, both threads print their individual swap counts and terminate

## Known Limitations

- Maximum input size is 200 integers
- Requires C11 standard support
- Assumes input file contains only valid integers

## Author

Name: Haikal Putryaudha
Student ID Number: 21604483
Email: 21604483@student.curtin.edu.au