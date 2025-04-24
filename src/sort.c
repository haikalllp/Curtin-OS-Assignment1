/**
 * sort.c - Implementation of the sorting thread logic for sss
 */

#include <pthread.h>
#include <stdio.h>
#include "sync.h"
#include "sort.h"

/* Define boolean type */
#define true 1
#define false 0

/* Thread message storage */
int thread1Swaps = 0;
int thread2Swaps = 0;
int threadsFinished = 0;
pthread_mutex_t outputMutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Thread function that performs the sorting operation
 * 
 * @param arg Pointer to ThreadArgs struct containing threadId
 * @return NULL
 */
void *performSort(void *arg) {
    int threadSwaps = 0;
    ThreadArgs *threadArgs = (ThreadArgs *)arg;
    int id = threadArgs->threadId;
    
    /* Setup pointers based on thread ID for easier access */
    bool *myNoSwap = (id == THREAD_ONE_ID ? &noSwapThread1 : &noSwapThread2);
    bool *otherNoSwap = (id == THREAD_ONE_ID ? &noSwapThread2 : &noSwapThread1);
    pthread_cond_t *myCondition = (id == THREAD_ONE_ID ? &conditionThread1 : &conditionThread2);
    pthread_cond_t *otherCondition = (id == THREAD_ONE_ID ? &conditionThread2 : &conditionThread1);
    
    while (true) {
        int localSwaps;
        int startIndex;
        int i;
        
        pthread_mutex_lock(&conditionMutex);
        while (threadTurn != id) {
            pthread_cond_wait(myCondition, &conditionMutex);
        }
        pthread_mutex_unlock(&conditionMutex);
        
        localSwaps = 0;
        startIndex = (id == THREAD_ONE_ID ? 0 : 1);
        
        /* Process pairs based on thread ID */
        for (i = startIndex; i + 1 < arraySize; i += 2) {
            if (sortArray[i] > sortArray[i + 1]) {
                int temp = sortArray[i];
                sortArray[i] = sortArray[i + 1];
                sortArray[i + 1] = temp;
                localSwaps++;
            }
        }
        
        /* Update global swap count */
        pthread_mutex_lock(&swapMutex);
        swapCount += localSwaps;
        pthread_mutex_unlock(&swapMutex);
        
        /* Update thread's total swaps */
        threadSwaps += localSwaps;
        
        pthread_mutex_lock(&conditionMutex);
        *myNoSwap = (localSwaps == 0);
        
        /* Check termination condition */
        if (*myNoSwap && *otherNoSwap) {
            threadTurn = (id == THREAD_ONE_ID ? THREAD_TWO_ID : THREAD_ONE_ID);
            pthread_cond_signal(otherCondition);
            
            /* Store thread swaps to print in correct order later */
            pthread_mutex_lock(&outputMutex);
            if (id == THREAD_ONE_ID) {
                thread1Swaps = threadSwaps;
            } else {
                thread2Swaps = threadSwaps;
            }
            threadsFinished++;
            pthread_mutex_unlock(&outputMutex);
            
            pthread_mutex_unlock(&conditionMutex);
            break;
        }
        
        /* Pass control to the other thread */
        threadTurn = (id == THREAD_ONE_ID ? THREAD_TWO_ID : THREAD_ONE_ID);
        pthread_cond_signal(otherCondition);
        pthread_mutex_unlock(&conditionMutex);
    }
    
    return NULL;
}
