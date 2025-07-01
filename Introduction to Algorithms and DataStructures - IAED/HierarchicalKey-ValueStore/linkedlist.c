/* File:  linkedlist.c
 * Author:  Andre Rodrigues 92421
 * Description: File containing the implementation of the functions
 *              related to linked and doubly linked lists.
 */

#include "linkedlist.h"

/* Creates a new node */
Link newNode(char *key, void *data)
{
    Link node = (Link)malloc(sizeof(struct node));

    node->key = copyString(key);
    node->data = data;
    node->next = NULL;
    node->next_order = NULL;
    node->prev_order = NULL;
    return node;
}

/* Destroys a given node */
void freeNode(Link node)
{
    free(node->key);
    free(node);
}

/* Insert a given node in the list */
void insertLL(Link *head, Link new)
{
    Link aux = new;

    aux->next = *head;
    *head = aux;
}

/* Search for a node with a given list key */
Link searchLL(Link head, char *name)
{
    Link aux;

    for (aux = head; aux != NULL; aux = aux->next)
    {
        if (strcmp(aux->key, name) == 0)
        {
            return aux;
        }
    }
    return NULL;
}

/* Removes a node with a given key from the list */
void removeLL(Link *head, char *name)
{
    Link aux, prev;

    for (aux = *head, prev = NULL; aux != NULL; prev = aux, aux = aux->next)
    {
        if (strcmp(aux->key, name) == 0)
        {
            if (aux == *head)
            {
                *head = (*head)->next;
            }
            else
            {
                prev->next = aux->next;
            }
        }
    }
}

/* Insert a given node at the end of the list */
void insertDLL(Link *first, Link *last, Link new)
{

    if (*first == NULL)
    {
        *first = new;
        *last = new;
    }
    else
    {
        (*last)->next_order = new;
        new->prev_order = *last;
        *last = new;
    }
}

/* Removes a given node of the list */
void removeDLL(Link *first, Link *last, Link del)
{
    if (*first == *last)
    {
        *first = NULL;
        *last = NULL;
    }
    else if (*first == del)
    {
        *first = (*first)->next_order;
        (*first)->prev_order = NULL;
    }
    else if (*last == del)
    {
        *last = (*last)->prev_order;
        (*last)->next_order = NULL;
    }
    else
    {
        del->next_order->prev_order = del->prev_order;
        del->prev_order->next_order = del->next_order;
    }

    freeNode(del);
}
