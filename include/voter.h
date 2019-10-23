/* 
Created by Antonis Karvelas, sdi1600060.
Here lies everything that has to do with the voters for the 1st project of
K22: Operating Systems.
*/

#ifndef ERGASIA_1_VOTER_H
#define ERGASIA_1_VOTER_H

#define VOTED 1
#define NOT_VOTED 0

#include <stdlib.h>
#include <string.h>

#include "RBT.h"
#include "BF.h"

typedef enum {
    MALE,
    FEMALE
} gender;

struct voter {
    char* IDstring;
    char* name;
    char* surname;
    unsigned int age;
    gender gender;
    unsigned int postCode;
    unsigned int hasVoted: 1;
} typedef voter;

voter*          initializeVoter(char* IDstring, char* name, char* surname, 
                unsigned int age, gender givenGender, unsigned int postCode);
node*           searchVoterInRBT(RBT* rbt, char* key);
BF*             createBFfromRBT(RBT* rbt, uint32_t population);
void            vote(RBT* rbt, char* key);
int             voteFromFile(RBT* rbt, char* filePath);
unsigned int    countVotesInRBT(RBT* rbt);
int             alphanumericCompare(node* A, node* B);
void            freeVoterNode(node* nodeToDie, bool freeElement);
void            printVoter(voter* v);
void            printVoterNode(node* nodeToPrint);

#endif //ERGASIA_1_VOTER_H