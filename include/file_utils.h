/**
 * file_utils.h - File handling utilities for the sss multi-threaded sorting program
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/**
 * Reads integers from a file and returns them as a dynamically allocated array
 * 
 * @param filename Path to the input file
 * @param out_n    Pointer to store the number of integers read
 * @return         Dynamically allocated array of integers, or NULL on failure
 */
int *read_input(const char *filename, int *out_n);

#endif /* FILE_UTILS_H */
