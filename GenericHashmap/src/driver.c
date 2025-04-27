/**
 * @file driver.c
 * @author Luke Early and teaching staff
 * Contains main, drives function
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <setjmp.h>
#include <unistd.h> // Unix-specific isatty() function.

#include "map.h"
#include "value.h"
#include "input.h"

// number of tables in map
#define MAP_SIZE 10
// length of set command to help with parsing commands
#define SET_CMD_LEN 3
// length of get command to help with parsing commands
#define GET_CMD_LEN 3
// length of remove command to help with parsing commands
#define REMOVE_CMD_LEN 6
// length of size command to help with parsing commands
#define SIZE_CMD_LEN 4
// length of quit command to help with parsing commands
#define QUIT_CMD_LEN 4

/** Print out a usage message and exit unsuccessfully. */
static void usage()
{
  fprintf( stderr, "Usage: driver [-term]\n" );
  exit( EXIT_FAILURE );
}

/**
 * Parses a command from the given line of text. Will either update given map or
 * print a line to stdout.
 * 
 * @param map to update, if commands are found
 * @param line to parse
 * @param env location in main to jump back to if parsed command is invalid
 * 
 * @return false unless quit command is parsed, then it returns true.
 */
bool handleCommand( Map *map, char const *line, jmp_buf *env )
{
  if ( line == NULL ) {
    return true;
  }

  if ( strncmp( line, "set", SET_CMD_LEN ) == 0 ) {
    char key[ KEY_LIMIT ];
    Value *thisValue = NULL;
    char *str = (char *)calloc( INIT_STR_CAP, sizeof( char ) );

    /**
     * read in key string and val string
     */
    sscanf( line + SET_CMD_LEN + sizeof( char ), " %23s", key );
    
    int count = 0;
    int standOff = 4 + strlen( key ) + 1;
    int capacity = INIT_STR_CAP;
    char currChar;

    while ( sscanf( line + standOff, "%c", &currChar ) == 1 ) {
      if ( isspace( currChar ) && currChar != ' ' ) {
        break;
      }

      // resize string array if exceeds capacity
      if ( count + 1 >= capacity ) {
        capacity *= 2;
        str = (char *)realloc( str, capacity*sizeof( char ) );
      }

      str[ count ] = currChar;
      count++;
      standOff++;
    }

    str[ count ] = '\0';
    
    // parse val
    thisValue = parseInteger( str );
    if ( thisValue == NULL ) {
      thisValue = parseDouble( str );
      if ( thisValue == NULL ) {
        thisValue = parseString( str );
        if ( thisValue == NULL ) {
          longjmp( *env, 1 );
        }
      }
    }

    // add val to map
    mapSet( map, key, thisValue );
    free( str );

  } else if ( strncmp( line, "get", GET_CMD_LEN ) == 0 ) {
    char key[ KEY_LIMIT ] = "";
    Value *val;
    int offSet = 0;

    sscanf( line + GET_CMD_LEN, "%23s%n", key, &offSet );

    if ( strlen( line ) > GET_CMD_LEN + sizeof( ' ' ) + offSet )
      longjmp( *env, 1 );

    val = mapGet( map, key );

    if ( val != NULL ) {
      val->print( val );
      printf( "\n" );
    } else {
      longjmp( *env, 1 );
    }

  } else if ( strncmp( line, "remove", REMOVE_CMD_LEN ) == 0 ) {
    mapRemove( map, line + REMOVE_CMD_LEN + sizeof( char ) );

  } else if ( strncmp( line, "size", SIZE_CMD_LEN ) == 0 ) {
    printf( "%d\n", mapSize( map ) );
  } else if ( strncmp( line, "quit", QUIT_CMD_LEN ) == 0 ) {
    return true;
  } else {
    longjmp( *env, 1 );
  }

  return false;
}

/**
   Starting point for the program.
   @param argc number of command-line arguments.
   @param argv array of strings given as command-line arguments.
   @return exit status for the program.
 */
int main( int argc, char *argv[] )
{
  // See if our input is from a terminal.
  bool interactive = isatty( STDIN_FILENO );
  Map * driverMap = makeMap( MAP_SIZE );
  char * line;
  jmp_buf env;

  // Parse command-line arguments.
  int apos = 1;
  while ( apos < argc ) {
    // The -term option makes the program behave as if it's in interactive mode,
    // even if it's reading from a file.
    if ( strcmp( argv[ apos ], "-term" ) == 0 ) {
      interactive = true;
      apos += 1;
    } else
      usage();
  }

  if ( interactive ) {
    printf( "cmd> " );
  }
  line = readLine( stdin );

  while ( !handleCommand( driverMap, line, &env ) ) {
    if ( setjmp( env ) == 0 ) {
      if ( interactive ) {
        printf( "cmd> " );
      }
      line = readLine( stdin );
    } else {
      if ( !interactive ) {
        fprintf( stderr, "Invalid command: %s\n", line );
        free( line );
        freeMap( driverMap );
        exit( EXIT_FAILURE );
      }
      printf( "Invalid command\n" );
      if ( interactive ) {
        printf( "cmd> " );
      }
      line = readLine( stdin );
    }
  }

  free( line );
  freeMap( driverMap );

  return EXIT_SUCCESS;
}
