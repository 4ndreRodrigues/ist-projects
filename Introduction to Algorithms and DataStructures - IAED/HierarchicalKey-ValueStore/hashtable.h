/* File:  hashtable.h
 * Author:  Andre Rodrigues 92421
 * Description: File containing the declaration of the functions related
 *              to the hashtable.
 */

#include "linkedlist.h"

#define HASHSIZE 503

typedef Link *Hash;

int hashkey(char *key);

Hash initHash();

void insertHash(Hash hashtable, char *key, Link new);

Link searchHash(Hash hashtable, char *key);

void removeHash(Hash hashtable, char *key);