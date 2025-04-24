/**
 * sync.c - Implementation of synchronization primitives for sss
 */

#include <stdbool.h>
#include "sync.h"

/* Shared state variables */
int threadTurn;
bool noSwapThread1, noSwapThread2;
pthread_mutex_t conditionMutex;
pthread_cond_t conditionThread1, conditionThread2;
long swapCount;
pthread_mutex_t swapMutex;

/* Define boolean type */
#define true 1
#define false 0

/**
 * Initialize all synchronization primitives
 */
void initSynchronization(void) {
    pthread_mutex_init(&conditionMutex, NULL);
    pthread_cond_init(&conditionThread1, NULL);
    pthread_cond_init(&conditionThread2, NULL);
    pthread_mutex_init(&swapMutex, NULL);
    
    /* Initialize turn to Thread 1 and set no-swap flags to false */
    threadTurn = THREAD_ONE_ID;
    noSwapThread1 = noSwapThread2 = false;
    swapCount = 0;
}

/**
 * Destroy all synchronization primitives
 */
void destroySynchronization(void) {
    pthread_mutex_destroy(&conditionMutex);
    pthread_cond_destroy(&conditionThread1);
    pthread_cond_destroy(&conditionThread2);
    pthread_mutex_destroy(&swapMutex);
}
