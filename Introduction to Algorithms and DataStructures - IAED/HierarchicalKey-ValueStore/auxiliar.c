/* File:  auxiliar.c
 * Author:  Andre Rodrigues 92421
 * Description: File containing the auxiliar functions.
 */

#include "auxiliar.h"

/* Copy a string */
char *copyString(char *original)
{
    char *copy = (char *)malloc(sizeof(char) * strlen(original) + 1);
    strcpy(copy, original);
    return copy;
}

/* Auxiliar function to sort */
int myCompare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

/* Sort a string array */
void sort(char **arr, int n)
{
    qsort(arr, n, sizeof(const char *), myCompare);
}