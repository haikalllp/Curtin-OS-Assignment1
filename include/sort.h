/**
 * sort.h - Sorting thread logic for the sss multi-threaded sorting program
 */

#ifndef SORT_H
#define SORT_H

#include "sync.h"

extern int *A;
extern int n;

/**
 * Thread function that performs the sorting operation
 * 
 * @param arg Pointer to ThreadArgs struct containing thread_id
 * @return NULL
 */
void *sort(void *arg);

#endif /* SORT_H */
