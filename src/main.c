/**
 * main.c - Main entry point for the sss multi-threaded sorting program
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "file_utils.h"
#include "sync.h"
#include "sort.h"

// Global variables for the array and its size
int *A = NULL;
int n = 0;

/**
 * Main entry point for the sss program
 * 
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int main(int argc, char **argv) {
    // Check for the correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file containing numbers to be sorted>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Read input from file
    A = read_input(argv[1], &n);
    if (A == NULL) {
        perror("read_input");
        return EXIT_FAILURE;
    }
    
    // Initialize synchronization primitives
    init_sync();
    
    // Create threads
    pthread_t th1, th2;
    ThreadArgs t1_args = { .thread_id = T1_ID };
    ThreadArgs t2_args = { .thread_id = T2_ID };
    
    // Create and start Thread 1
    if (pthread_create(&th1, NULL, sort, &t1_args) != 0) {
        perror("pthread_create");
        free(A);
        destroy_sync();
        exit(EXIT_FAILURE);
    }
    
    // Create and start Thread 2
    if (pthread_create(&th2, NULL, sort, &t2_args) != 0) {
        perror("pthread_create");
        free(A);
        destroy_sync();
        exit(EXIT_FAILURE);
    }
    
    // Wait for threads to finish
    if (pthread_join(th1, NULL) != 0) {
        perror("pthread_join");
        free(A);
        destroy_sync();
        exit(EXIT_FAILURE);
    }
    
    if (pthread_join(th2, NULL) != 0) {
        perror("pthread_join");
        free(A);
        destroy_sync();
        exit(EXIT_FAILURE);
    }
    
    // Print the sorted array
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
    
    // Print the total number of swaps
    printf("Total number of swaps to sort array A = %ld.\n", swap_count);
    
    // Clean up resources
    destroy_sync();
    free(A);
    
    return EXIT_SUCCESS;
}
