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

voter*      initializeVoter(char* IDstring, char* name, char* surname, 
            unsigned int age, gender givenGender, unsigned int postCode);
int         alphanumericCompare(node* A, node* B);

#endif //ERGASIA_1_VOTER_H