/**
 * @file map.h
 * @author Luke Early
 * This is the header file for the map component
 */

#ifndef MAP_H
#define MAP_H

#include "value.h"
#include <stdbool.h>

/** Incomplete type for the Map representation. */
typedef struct MapStruct Map;

// Maximum length of a key.
#define KEY_LIMIT 24
// number of nodes to calloc each time
#define NUM_NODES_TO_INIT 1
// first amount of bits to shift in jenkins hash
#define FIRST_HASH_SHIFT 10
// second amount of bits to shift in jenkins hash
#define SECOND_HASH_SHIFT 6
// third amount of bits to shift in jenkins hash
#define THIRD_HASH_SHIFT 3
// fourth amount of bits to shift in jenkins hash
#define FOURTH_HASH_SHIFT 11
// fifth amount of bits to shift in jenkins hash
#define FIFTH_HASH_SHIFT 16

/**
 * Makes an empty, dynamically allocated map, initializes all fields
 * and returns a pointer to it.
 * 
 * @param len number of elements in the hash table
 * 
 * @return pointer to empty Map
 */
Map *makeMap( int len );

/**
 * Returns the number of key / value pairs currently in the map.
 * 
 * @param m map from which the number of key / value pair is reported
 * 
 * @return number of key / value pairs in m
 */
int mapSize( Map *m );

/**
 * Adds the given key / value pair to the given map, which then 
 * takes ownership of key / value pair.
 * 
 * @param m map to which key / value pair will be added
 * @param key the key part of the key / value pair
 * @param val the value part of the key / value pair
 */
void mapSet( Map *m, char const *key, Value *val );

/**
 * Returns the value if the key is present in given map.
 * 
 * If the key is not in the given map, returns NULL.
 * 
 * @param m map from which to check for the presence of given key
 * @param key key to search for in given map
 * 
 * @return value associated with the key in the map
 */
Value *mapGet( Map *m, char const *key );

/**
 * Removes the key/value pair from the given map.
 * 
 * Frees all memory associated with key / value pair.
 * 
 * @param m map to remove key / value pair from
 * @param key key to search for in the map
 * 
 * @return true if key is found / removed, false if no key found
 */
bool mapRemove( Map *m, char const *key );

/**
 * Function frees all memory used to store the given map:
 *   - hash table
 *   - Nodes
 *   - Value objects used by Nodes
 * 
 * @param m map to free
 */
void freeMap( Map *m );


#endif
