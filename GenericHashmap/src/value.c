/**
 * @file value.c
 * @author Teaching Staff and Luke Early
 * This is the implementation of the value component.
 */
#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool blankString( char const *str )
{
  // Skip spaces.
  while ( isspace( *str ) )
    ++str;

  // Return false if we see non-whitespace before the end-of-string.
  if ( *str )
    return false;
  return true;
}

// Generic destroy method, suitable for most types of values.  It
// assumes the data can be freed as a single block of heap memory.
static void destroyGeneric( Value *v )
{
  free( v->data );
  free( v );
}

// print method for Integer.
static void printInteger( Value const *v )
{
  // Print the data as an integer.
  printf( "%d", * (int *) v->data );
}

/**
 * Prints the double stored in the passed value.
 * 
 * @param v the value struct from which the
 *          double will be printed
 */
static void printDouble( Value const *v )
{
  printf( "%.2f", * (double *) v->data );
}

/**
 * Prints the string stored in the passed value.
 * 
 * @param v the value struct from which the
 *          string will be printed
 */
static void printString( Value const *v )
{
  printf( "%s", (char *) v->data );
}

/** If possible, parse an integer from the given string and return a
    Value instance containing it.  Return NULL if the string isn't in the
    proper format.
    @param str String from which to parse an integer.
    @return new Value containing the integer, or NULL if it's not in the proper format.
*/
Value *parseInteger( char const *str )
{
  // Parse an integer value from the given string.
  int val, n;
  if ( sscanf( str, "%d%n", &val, &n ) != 1 )
    return NULL;

  // Make sure there's nothing extra after the integer value
  if ( ! blankString( str + n ) )
    return NULL;
  
  // Allocate space for the value struct and the integer in its data field.
  Value *this = (Value *) malloc( sizeof( Value ) );
  this->data = malloc( sizeof( int ) );
  * (int *) this->data = val;

  // Fill in function pointers and return this value.
  this->print = printInteger;
  this->destroy = destroyGeneric;
  return this;
}

/** 
 * Parses double from input str, create a pointer to a Value with the
 * double parsed from str.
 * 
 * @param str string to parse double from
 * 
 * @return instance of Value containing double, or NULL if no double
 */
Value *parseDouble( char const *str )
{
  double val;
  int n;
  if ( sscanf( str, "%lf%n", &val, &n ) != 1 )
    return NULL;

  if ( ! blankString( str + n ) )
    return NULL;
  
  Value *this = (Value *) malloc( sizeof( Value ) );
  this->data = malloc( sizeof( double ) );
  * (double *) this->data = val;

  this->print = printDouble;
  this->destroy = destroyGeneric;
  return this;
}

/** 
 * Parses string from input str, create a pointer to a Value with the
 * string parsed from str.
 * 
 * @param str string to parse string from
 * 
 * @return instance of Value containing string, or NULL if no string value in the
 *         right format
 */
Value *parseString( char const *str )
{
  char *string = ( char * )calloc( START_STR_CAP, sizeof( char ) );
  int size = 0;
  int cap = START_STR_CAP;
  char ch = 0;

  while ( sscanf( str + size, "%c", &ch ) == 1 ) {
    if ( size >= cap ) {
      cap = cap * 2;
      string = realloc( string, cap * sizeof( char ) );
    }
    
    string[ size ] = ch;
    size++;
  }

  string = realloc( string, ( size + 1 ) * sizeof( char ) );
  string[ size ] = '\0';
  
  Value *this = (Value *) malloc( sizeof( Value ) );
  this->data = (char *)calloc( ( size + 1 ),  sizeof( char ) );
  this->data = string;

  this->print = printString;
  this->destroy = destroyGeneric;

  return this;

}
