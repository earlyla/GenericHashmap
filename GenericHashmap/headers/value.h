/**
 * @file value.h
 * @author Teaching Staff and Luke Early
 * This is the header for the value component.
 */
#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

// number of char to allocate initially for an arbitrary length string
#define START_STR_CAP 5

/** Abstract type used to represent an arbitrary type of value. */
typedef struct ValueStruct {
  /** Pointer to a function that prints this value to the terminal.
      @param v Pointer to the value object to print. */
  void (*print)( struct ValueStruct const *v );

  /** Pointer to a function that frees memory for this Value instance.
      @param v Pointer to the value object to free. */
  void (*destroy)( struct ValueStruct *v );

  /** Arbitrary data used to represent the integer/string/etc for this value. */
  void *data;
} Value;

/** Return true if the given string contains only whitespace.  This
    is useful for making sure there's nothing extra at the end of a line
    of user input.
    @param str String to check for blanks.
    @return True if the string contains only blanks.
*/
bool blankString( char const *str );

/** If possible, parse an integer from the given string and return a
    Value instance containing it.  Return NULL if the string isn't in the
    proper format.
    @param str String from which to parse an integer.
    @return new Value containing the integer, or NULL if it's not in the proper format.
*/
Value *parseInteger( char const *str );

/** 
 * Parses double from input str, create a pointer to a Value with the
 * double parsed from str.
 * 
 * @param str string to parse double from
 * 
 * @return instance of Value containing double, or NULL if no double
 */
Value *parseDouble( char const *str );

/** 
 * Parses string from input str, create a pointer to a Value with the
 * string parsed from str.
 * 
 * @param str string to parse string from
 * 
 * @return instance of Value containing string, or NULL if no string value in the
 *         right format
 */
Value *parseString( char const *str );

#endif
