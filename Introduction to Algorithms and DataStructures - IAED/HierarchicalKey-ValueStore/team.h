/* File:  team.h
 * Author:  Andre Rodrigues 92421
 * Description: File containing the declaration of the functions related
 *              to the team and the type of data.
 */

#include "auxiliar.h"

typedef struct team
{
    char *name;
    int victories;
} * Team;

Team newTeam(char *name);

void printTeam(Team team);

void freeTeam(Team team);