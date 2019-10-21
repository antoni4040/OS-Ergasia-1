/* 
Created by Antonis Karvelas, sdi1600060.
Here lies everything that has to do with the voters for the 1st project of
K22: Operating Systems.
*/

#include "voter.h"

/*
Create a new voter with given data. Of course he/she hasn't voted yet.
*/
voter* initializeVoter(char* IDstring, char* name, char* surname, 
unsigned int age, gender givenGender, unsigned int postCode) {
    voter* newVoter = malloc(sizeof(voter));
    newVoter->IDstring = malloc(strlen(IDstring) * sizeof(char));
    strcpy(newVoter->IDstring, IDstring);
    newVoter->name = malloc(strlen(name) * sizeof(char));
    strcpy(newVoter->name, name);
    newVoter->surname = malloc(strlen(surname) * sizeof(char));
    strcpy(newVoter->surname, surname);
    newVoter->age = age;
    newVoter->gender = givenGender;
    newVoter->postCode = postCode;
    newVoter->hasVoted = NOT_VOTED;
    return newVoter;
}

/*
Alphanumeric compare for the RBT using strcmp.
*/
int alphanumericCompare(node* A, node* B) {
    char* elementA = ((voter*)(A->element))->IDstring;
    char* elementB = ((voter*)(B->element))->IDstring;

    return strcmp(elementA, elementB);
}

/*
Free node function for the RBT.
*/
void freeNode(node* nodeToDie) {
    voter* voterToDie = (voter*)(nodeToDie->element);
    free(voterToDie->IDstring);
    free(voterToDie->name);
    free(voterToDie->surname);
    free(voterToDie);
    free(nodeToDie);
}

/*
A function that prints the voter from an RBT node.
*/
void printNode(node* nodeToPrint) {
    voter* v = (voter*)(nodeToPrint->element);
    char genderString[7];
    if(v->gender == MALE)
        strcpy(genderString, "MALE");
    else
        strcpy(genderString, "FEMALE");
    printf("%s %s %s %u %s %u\n", v->IDstring, v->name,
        v->surname, v->age, genderString, v->postCode);
}