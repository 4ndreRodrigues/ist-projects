/* File:  hashtable.c
 * Author:  Andre Rodrigues 92421
 * Description: File containing the implementation of the hashtable functions.
 */

#include "hashtable.h"

/* Transforms the key in a hashtable index */
int hashkey(char *key)
{
    unsigned long hash = 0;
    int a = 127;

    for (; *key != '\0'; key++) 
    {
        hash = (a*hash + *key) % HASHSIZE;
    }
    return hash;
}

/* Initializes a given hashtable */
Hash initHash()
{
    int i;
    Hash hashtable = (Hash)malloc(sizeof(Link) * HASHSIZE);

    for (i = 0; i < HASHSIZE; i++)
    {
        hashtable[i] = NULL;
    }
    return hashtable;
}

/* Insert a new node in the hashtable */
void insertHash(Hash hashtable, char *key, Link new)
{
    int i = hashkey(key);

    insertLL(&hashtable[i], new);
}

/* Search for a node with a given key in the hashtable */
Link searchHash(Hash hashtable, char *key)
{
    int i = hashkey(key);

    return searchLL(hashtable[i], key);
}

/* Removes the node with a given key of the hashtable */
void removeHash(Hash hashtable, char *key)
{
    int i = hashkey(key);

    removeLL(&hashtable[i], key);
}