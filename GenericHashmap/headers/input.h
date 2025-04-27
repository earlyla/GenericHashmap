/**
 * @file input.h
 * @author Luke Early
 * Header file with function prototypes for input.c.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_STR_CAP 5
// factor by which to grow string each time it runs out of space
#define GROWTH_FACTOR 2

/**
 * Reads in a single line of input from given stream.
 * 
 * Returns line as a string in a dynamically allocated block of memory
 * 
 * @param fp pointer to input stream
 * @return input stream as a string
 */
char *readLine( FILE *fp );
