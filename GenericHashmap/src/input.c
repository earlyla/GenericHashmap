/**
 * @file input.c
 * @author Luke Early
 * Source file for input component.
 */

#include "input.h"

/**
 * Reads in a single line of input from given stream.
 * 
 * Returns line as a string in a dynamically allocated block of memory
 * 
 * @param fp pointer to input stream
 * @return input stream as a string
 */
char *readLine( FILE *fp )
{
  int count = 0;
  int capacity = INIT_STR_CAP;
  char *str = malloc( INIT_STR_CAP * sizeof( char ) );
  
  char currChar;
  
  while ( fscanf( fp, "%c", &currChar ) == 1 ) {
    if ( isspace( currChar ) && currChar != ' ' ) {
      break;
    }

    // resize string array if exceeds capacity
    if ( count + 1 >= capacity ) {
      capacity *= GROWTH_FACTOR;
      str = realloc( str, capacity*sizeof( char ) );
    }

    str[ count ] = currChar;
    count++;
  }

  if ( feof( fp ) ) {
    str = NULL;
  }

  if ( count > 0 ) {
    str[ count ] = '\0';
  }
  
  return str;
}
