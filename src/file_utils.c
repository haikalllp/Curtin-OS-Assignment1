/**
 * file_utils.c - Implementation of file handling utilities for sss
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utils.h"

/**
 * Reads integers from a file and returns them as a dynamically allocated array
 * 
 * @param filename Path to the input file
 * @param out_n    Pointer to store the number of integers read
 * @return         Dynamically allocated array of integers, or NULL on failure
 */
int *read_input(const char *filename, int *out_n) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }
    
    int buffer[200]; // Maximum array size per spec
    int count = 0;
    
    // Read integers until end of file or array is full
    while (count < 200 && fscanf(fp, "%d", &buffer[count]) == 1) {
        count++;
    }
    
    fclose(fp);
    
    // Allocate memory for the return array
    int *arr = malloc(count * sizeof(int));
    if (arr == NULL) {
        return NULL;
    }
    
    // Copy data from buffer to output array
    memcpy(arr, buffer, count * sizeof(int));
    
    // Store the count in the output parameter
    *out_n = count;
    
    return arr;
}
