/**
 * sync.h - Synchronization primitives for the sss multi-threaded sorting program
 */

#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>
#include <stdbool.h>

#define T1_ID 1
#define T2_ID 2

extern int turn;
extern bool no_swap_t1, no_swap_t2;
extern pthread_mutex_t cond_mutex;
extern pthread_cond_t cond_t1, cond_t2;
extern long swap_count;
extern pthread_mutex_t swap_mutex;

typedef struct {
    int thread_id;
} ThreadArgs;

void init_sync(void);
void destroy_sync(void);

#endif /* SYNC_H */
