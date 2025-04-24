/**
 * fileHandling.h - File handling utilities for the sss multi-threaded sorting program
 */

#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

/**
 * Reads integers from a file and returns them as a dynamically allocated array
 * 
 * @param filename Path to the input file
 * @param outSize  Pointer to store the number of integers read
 * @return         Dynamically allocated array of integers, or NULL on failure
 */
int *readInputFile(const char *filename, int *outSize);

#endif /* FILE_HANDLING_H */
