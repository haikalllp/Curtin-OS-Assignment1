/**
 * main.c - Main entry point for the sss multi-threaded sorting program
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "fileHandling.h"
#include "sync.h"
#include "sort.h"

/* Global variables for the array and its size */
int *sortArray = NULL;
int arraySize = 0;

/**
 * Main entry point for the sss program
 * 
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int main(int argc, char **argv) {
    /* Variable declarations at the beginning of the function */
    pthread_t thread1, thread2;
    ThreadArgs thread1Args;
    ThreadArgs thread2Args;
    int index;
    
    /* Check for the correct number of arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file containing numbers to be sorted>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    /* Read input from file */
    sortArray = readInputFile(argv[1], &arraySize);
    if (sortArray == NULL) {
        perror("readInputFile");
        return EXIT_FAILURE;
    }
    
    /* Initialize synchronization primitives */
    initSynchronization();
    
    /* Initialize thread arguments */
    thread1Args.threadId = THREAD_ONE_ID;
    thread2Args.threadId = THREAD_TWO_ID;
    
    /* Create and start Thread 1 */
    if (pthread_create(&thread1, NULL, performSort, &thread1Args) != 0) {
        perror("pthread_create");
        free(sortArray);
        destroySynchronization();
        exit(EXIT_FAILURE);
    }
    
    /* Create and start Thread 2 */
    if (pthread_create(&thread2, NULL, performSort, &thread2Args) != 0) {
        perror("pthread_create");
        free(sortArray);
        destroySynchronization();
        exit(EXIT_FAILURE);
    }
    
    /* Wait for threads to finish */
    if (pthread_join(thread1, NULL) != 0) {
        perror("pthread_join");
        free(sortArray);
        destroySynchronization();
        exit(EXIT_FAILURE);
    }
    
    if (pthread_join(thread2, NULL) != 0) {
        perror("pthread_join");
        free(sortArray);
        destroySynchronization();
        exit(EXIT_FAILURE);
    }
    
    /* Print the sorted array */
    for (index = 0; index < arraySize; index++) {
        printf("%d ", sortArray[index]);
    }
    printf("\n");
    
    /* Print the total number of swaps */
    printf("Total number of swaps to sort array sortArray = %ld.\n", swapCount);
    
    /* Clean up resources */
    destroySynchronization();
    free(sortArray);
    
    return EXIT_SUCCESS;
}
