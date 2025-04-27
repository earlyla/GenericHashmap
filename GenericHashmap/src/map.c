/**
 * @file map.c
 * @author Luke Early
 * This is the component file for the map
 */
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Node containing a key / value pair. */
typedef struct NodeStruct {
  /** String key for this map entry. */
  char key[ KEY_LIMIT + 1 ];
  
  /** Pointer to the value part of the key / value pair. */
  Value *val;
  
  /** Pointer to the next node at the same element of this table. */
  struct NodeStruct *next;
} Node;

/** Representation of a hash table implementation of a map. */
struct MapStruct {
  /** Table of key / value pairs. */
  Node **table;

  /** Current length of the table. */
  int tlen;
  
  /** Current size of the map (number of different keys). */
  int size;
};

/**
 * Hashes the passed key using the jenkins hash alogrithm.
 * 
 * @param key key to hash
 * @param len length of the key string
 * 
 * @return unsigned int as the hash representing the key
 */
unsigned int jenkinsHash( const char * key, int len )
{
  int n = 0;
  unsigned int hash = 0;

  while ( n != len ) {
    hash += key[ n++ ];
    hash += hash << FIRST_HASH_SHIFT;
    hash ^= hash >> SECOND_HASH_SHIFT;
  }

  hash += hash << THIRD_HASH_SHIFT;
  hash ^= hash >> FOURTH_HASH_SHIFT;
  hash += hash << FIFTH_HASH_SHIFT;

  return hash;
}

/**
 * Makes an empty, dynamically allocated map, initializes all fields
 * and returns a pointer to it.
 * 
 * @param len number of elements in the hash table
 * 
 * @return pointer to empty Map
 */
Map *makeMap( int len )
{
  // allocate mem for whole map
  Map * m1 = ( Map * )malloc( sizeof( Map ) );
  m1->tlen = len; // number of table in the map
  m1->size = 0; // number of keys in the map

  // allocate space for tlen Node pointers in table
  m1->table = (Node **)calloc( m1->tlen, sizeof( Node * ) );

  for ( int i = 0; i < m1->tlen; i++ ) {
    m1->table[ i ] = (Node *)calloc( NUM_NODES_TO_INIT, sizeof( Node ) );
  }

  return m1;
}

/**
 * Returns the number of key / value pairs currently in the map.
 * 
 * @param m map from which the number of key / value pair is reported
 * 
 * @return number of key / value pairs in m
 */
int mapSize( Map *m )
{
  return m->size;
}

/**
 * Adds the given key / value pair to the given map, which then 
 * takes ownership of key / value pair.
 * 
 * @param m map to which key / value pair will be added
 * @param key the key part of the key / value pair
 * @param val the value part of the key / value pair
 */
void mapSet( Map *m, char const *key, Value *val )
{
  Node *currNode = (Node *)calloc( 1, sizeof( Node ) );
  Node *n = (Node *)calloc( NUM_NODES_TO_INIT, sizeof( Node ) );
  // index to look for and set key/value pair
  int idx = jenkinsHash( key, strlen( key ) ) % m->tlen;

  if ( m->table[ idx ]->next == NULL ) {
    // create node to add to list
    strcpy( n->key, key );
    n->val = val;
    n->next = NULL;

    m->table[ idx ]->next = n;
    m->size++;

  } else {
    currNode = m->table[ idx ]->next;

    while( currNode->next ) {
      if ( strcmp( currNode->key, key ) == 0 ) {
        // replace data
      } else {
        currNode = currNode->next;
      }
    }

    strcpy( n->key, key );
    n->val = val;
    n->next = NULL;

    currNode->next = n;
    m->size++;
  }
}

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
Value *mapGet( Map *m, char const *key )
{
  int idx = 0;
  Value * val = NULL;
  idx = jenkinsHash( key, strlen( key ) ) % m->tlen;
  Node *currNode = NULL;

  if ( m->table[ idx ]->next == NULL ) {
    return NULL;
  } else {
    currNode = m->table[ idx ]->next;

    while( currNode ) {
      if ( strcmp( currNode->key, key ) == 0 ) {
        val = currNode->val;
        break;
      }
      currNode = currNode->next;
    }
  }

  return val;
}

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
bool mapRemove( Map *m, char const *key )
{
  int idx = 0;
  idx = jenkinsHash( key, strlen( key ) ) % m->tlen;
  Node *currNode = NULL;
  bool flag = false;

  if ( m->table[ idx ]->next == NULL ) {
    flag = false;
  } else {
    currNode = m->table[ idx ]->next;
    if ( currNode->next == NULL ) {
      currNode->val->destroy( currNode->val );
      m->table[ idx ]->next = NULL;
      m->size--;
      flag = true;
    } else {
      while( currNode ) {
        if ( strcmp( currNode->next->key, key ) == 0 ) {
          Node *temp = currNode->next->next;
          currNode->next->val->destroy( currNode->next->val );
          currNode->next = temp;
          m->size--;
          flag = true;
        }
        currNode = currNode->next;
      }
    }
  }

  return flag;
}

/**
 * Function frees all memory used to store the given map:
 *   - hash table
 *   - Nodes
 *   - Value objects used by Nodes
 * 
 * @param m map to free
 */
void freeMap( Map *m )
{ 
  for ( int i = 0; i < m->tlen; i++ ) {
    free( m->table[ i ] );
  }

  free( m->table );
  free( m );
}
