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

/**
 * Thread function that performs the sorting operation
 * 
 * @param arg Pointer to ThreadArgs struct containing thread_id
 * @return NULL
 */
void *sort(void *arg) {
    int thread_swaps = 0;
    ThreadArgs *targs = (ThreadArgs *)arg;
    int id = targs->thread_id;
    
    /* Setup pointers based on thread ID for easier access */
    bool *my_no_swap = (id == T1_ID ? &no_swap_t1 : &no_swap_t2);
    bool *other_no_swap = (id == T1_ID ? &no_swap_t2 : &no_swap_t1);
    pthread_cond_t *my_cond = (id == T1_ID ? &cond_t1 : &cond_t2);
    pthread_cond_t *other_cond = (id == T1_ID ? &cond_t2 : &cond_t1);
    
    while (true) {
        int local_swaps;
        int start;
        int i;
        
        pthread_mutex_lock(&cond_mutex);
        while (turn != id) {
            pthread_cond_wait(my_cond, &cond_mutex);
        }
        pthread_mutex_unlock(&cond_mutex);
        
        local_swaps = 0;
        start = (id == T1_ID ? 0 : 1);
        
        /* Process pairs based on thread ID */
        for (i = start; i + 1 < n; i += 2) {
            if (A[i] > A[i + 1]) {
                int tmp = A[i];
                A[i] = A[i + 1];
                A[i + 1] = tmp;
                local_swaps++;
            }
        }
        
        /* Update global swap count */
        pthread_mutex_lock(&swap_mutex);
        swap_count += local_swaps;
        pthread_mutex_unlock(&swap_mutex);
        
        /* Update thread's total swaps */
        thread_swaps += local_swaps;
        
        pthread_mutex_lock(&cond_mutex);
        *my_no_swap = (local_swaps == 0);
        
        /* Check termination condition */
        if (*my_no_swap && *other_no_swap) {
            turn = (id == T1_ID ? T2_ID : T1_ID);
            pthread_cond_signal(other_cond);
            printf("Thread %d: total number of swaps = %d\n", id, thread_swaps);
            pthread_mutex_unlock(&cond_mutex);
            break;
        }
        
        /* Pass control to the other thread */
        turn = (id == T1_ID ? T2_ID : T1_ID);
        pthread_cond_signal(other_cond);
        pthread_mutex_unlock(&cond_mutex);
    }
    
    return NULL;
}
