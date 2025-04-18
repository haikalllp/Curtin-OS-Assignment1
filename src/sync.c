/**
 * sync.c - Implementation of synchronization primitives for sss
 */

#include "sync.h"

// Shared state variables
int turn;
bool no_swap_t1, no_swap_t2;
pthread_mutex_t cond_mutex;
pthread_cond_t cond_t1, cond_t2;
long swap_count;
pthread_mutex_t swap_mutex;

/**
 * Initialize all synchronization primitives
 */
void init_sync(void) {
    pthread_mutex_init(&cond_mutex, NULL);
    pthread_cond_init(&cond_t1, NULL);
    pthread_cond_init(&cond_t2, NULL);
    pthread_mutex_init(&swap_mutex, NULL);
    
    // Initialize turn to T1 and set no-swap flags to false
    turn = T1_ID;
    no_swap_t1 = no_swap_t2 = false;
    swap_count = 0;
}

/**
 * Destroy all synchronization primitives
 */
void destroy_sync(void) {
    pthread_mutex_destroy(&cond_mutex);
    pthread_cond_destroy(&cond_t1);
    pthread_cond_destroy(&cond_t2);
    pthread_mutex_destroy(&swap_mutex);
}
