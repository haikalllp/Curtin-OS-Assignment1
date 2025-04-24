/**
 * fileHandling.c - Implementation of file handling utilities for sss
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileHandling.h"

/**
 * Reads integers from a file and returns them as a dynamically allocated array
 * 
 * @param filename Path to the input file
 * @param outSize  Pointer to store the number of integers read
 * @return         Dynamically allocated array of integers, or NULL on failure
 */
int *readInputFile(const char *filename, int *outSize) {
    FILE *filePointer;
    int numberBuffer[200]; /* Maximum array size per spec */
    int count = 0;
    int *resultArray;
    
    filePointer = fopen(filename, "r");
    if (filePointer == NULL) {
        return NULL;
    }
    
    /* Read integers until end of file or array is full */
    while (count < 200 && fscanf(filePointer, "%d", &numberBuffer[count]) == 1) {
        count++;
    }
    
    fclose(filePointer);
    
    /* Allocate memory for the return array */
    resultArray = malloc(count * sizeof(int));
    if (resultArray == NULL) {
        return NULL;
    }
    
    /* Copy data from buffer to output array */
    memcpy(resultArray, numberBuffer, count * sizeof(int));
    
    /* Store the count in the output parameter */
    *outSize = count;
    
    return resultArray;
}
