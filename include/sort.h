/**
 * sort.h - Sorting thread logic for the sss multi-threaded sorting program
 */

#ifndef SORT_H
#define SORT_H

#include "sync.h"

extern int *sortArray;
extern int arraySize;

/**
 * Thread function that performs the sorting operation
 * 
 * @param arg Pointer to ThreadArgs struct containing threadId
 * @return NULL
 */
void *performSort(void *arg);

#endif /* SORT_H */
