/**
 * sync.h - Synchronization primitives for the sss multi-threaded sorting program
 */

#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>
#include <stdbool.h>

#define THREAD_ONE_ID 1
#define THREAD_TWO_ID 2

extern int threadTurn;
extern bool noSwapThread1, noSwapThread2;
extern pthread_mutex_t conditionMutex;
extern pthread_cond_t conditionThread1, conditionThread2;
extern int swapCount;
extern pthread_mutex_t swapMutex;

typedef struct {
    int threadId;
} ThreadArgs;

void initSynchronization(void);
void destroySynchronization(void);

#endif /* SYNC_H */
