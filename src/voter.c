#include "voter.h"

voter* initializeVoter(char* IDstring, char* name, char* surname, 
int age, gender givenGender, int postCode) {
    voter* newVoter = malloc(sizeof(voter));
    strcpy(newVoter->IDstring, IDstring);
    strcpy(newVoter->name, name);
    strcpy(newVoter->surname, surname);
    newVoter->age = age;
    newVoter->gender = givenGender;
    newVoter->postCode = postCode;
    newVoter->hasVoted = NOT_VOTED;
    return newVoter;
}