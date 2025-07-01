/* File:  match.h
 * Author:  Andre Rodrigues 92421
 * Description: File containing the declaration of the functions related
 *              to the match and the type of data.
 */

#include "auxiliar.h"

typedef struct match
{
    char *name;
    char *team1;
    char *team2;
    int score1;
    int score2;
} * Match;

Match newMatch(char *name, char *team1, char *team2, int score1, int score2);

void printMatch(Match match);

void freeMatch(Match match);