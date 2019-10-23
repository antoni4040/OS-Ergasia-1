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
    newVoter->IDstring = malloc(strlen(IDstring) * sizeof(char) + 1);
    strcpy(newVoter->IDstring, IDstring);
    newVoter->IDstring[strlen(IDstring)] = '\0';

    newVoter->name = malloc(strlen(name) * sizeof(char) + 1);
    strcpy(newVoter->name, name);
    newVoter->name[strlen(name)] = '\0';

    newVoter->surname = malloc(strlen(surname) * sizeof(char) + 1);
    strcpy(newVoter->surname, surname);
    newVoter->surname[strlen(surname)] = '\0';

    newVoter->age = age;
    newVoter->gender = givenGender;
    newVoter->postCode = postCode;
    newVoter->hasVoted = NOT_VOTED;
    return newVoter;
}

/*
Recurse through voter RBT and insert node element to BF.
*/
void nodeToBFRecurse(node* givenNode, node* NIL, BF* bf) {
    if(givenNode != NIL) {
        char* key = ((voter*)(givenNode->element))->IDstring;
        insertToBloomFilter(bf, key, strlen(key));
        nodeToBFRecurse(givenNode->leftChild, NIL, bf);
        nodeToBFRecurse(givenNode->rightChild, NIL, bf);
    }
}

/*
Helper function to create BF from voter RBT.
*/
BF* createBFfromRBT(RBT* rbt, uint32_t population) {
    BF* newBF = initializeBloomFilter(population);
    nodeToBFRecurse(rbt->root, rbt->NIL, newBF);
    return newBF;
}

/*
Helper function to search by key in an RBT with voters.
*/
node* searchVoterInRBT(RBT* rbt, char* key) {
    voter* spectreVoter = initializeVoter(
        key, "name", "surname", 10, MALE, 11111);
    node* spectreNode = initializeNode(spectreVoter);
    node* findInRBT = RBSearch(rbt, spectreNode);
    if(findInRBT != NULL) {
        rbt->freeNode(spectreNode, true);
        return findInRBT;
    }
    else {
        return NULL;
    }
}

/*
Given a voter ID, vote and/or print messages accordingly.
*/
void vote(RBT* rbt, char* key) {
    voter* findVoter = (voter*)(searchVoterInRBT(rbt, key)->element);
    if(findVoter == NULL) {
        printf("# REC-WITH %s NOT-in-structs\n", key);
    }
    else {
        if(findVoter->hasVoted == VOTED) {
            printf("# REC-WITH %s ALREADY-VOTED\n", key);
        }
        else {
            findVoter->hasVoted = VOTED;
            printf("# REC-WITH %s SET-VOTED\n", key);
        }
    }
}

/*
Vote from file of voters' IDs.
*/
int voteFromFile(RBT* rbt, char* filePath) {
    FILE* votes = fopen(filePath, "r");    //Attempt to open file.
    if(votes == NULL) {
        fprintf(stderr, "Problem opening input file for reading.\n");
        return 1;
    }

    //Read voter IDs and vote:
    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, votes) != EOF) {
        line = strtok(line, "\n");
        vote(rbt, line);
    }
    free(line);
    fclose(votes);
    return 0;
}

/*
Recursively count the voters who have voted and increment a given unsigned int pointer.
*/
void countVotesRecurse(node* currentNode, node* NIL, unsigned int* votes) {
    if(currentNode != NIL) {
        voter* currentVoter = (voter*)(currentNode->element);
        if(currentVoter->hasVoted == VOTED) {
            (*votes)++;
        }
        countVotesRecurse(currentNode->leftChild, NIL, votes);
        countVotesRecurse(currentNode->rightChild, NIL, votes);
    }
}

/*
Helper functions, calls the recursive countVotesRecurse.
I hate recursion, but so be it.
*/
unsigned int countVotesInRBT(RBT* rbt) {
    unsigned int* votes = malloc(sizeof(unsigned int));
    *votes = 0;
    countVotesRecurse(rbt->root, rbt->NIL, votes);
    unsigned int votesFinal = *votes;
    free(votes);
    return votesFinal;
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
void freeVoterNode(node* nodeToDie, bool freeElement) {
    if(freeElement){
        voter* voterToDie = (voter*)(nodeToDie->element);
        free(voterToDie->IDstring);
        free(voterToDie->name);
        free(voterToDie->surname);
        free(voterToDie);
    }
    free(nodeToDie);
}

/*
Helper function to print voter.
*/
void printVoter(voter* v) {
    char genderString[7];
    if(v->gender == MALE) {
        strcpy(genderString, "MALE");
    }
    else
        strcpy(genderString, "FEMALE");
    printf("%s %s %s %u %s %u\n", v->IDstring, v->name,
        v->surname, v->age, genderString, v->postCode);
}

/*
A function that prints the voter from an RBT node.
*/
void printVoterNode(node* nodeToPrint) {
    voter* v = (voter*)(nodeToPrint->element);
    printVoter(v);
}

/*
Write voter data to output file.
*/
void writeVoterToFile(voter* currentVoter, FILE* file) {
    if(currentVoter->gender == MALE)
        fprintf(file, "%s %s %s %u M %u\n", currentVoter->IDstring, currentVoter->name,
            currentVoter->surname, currentVoter->age, currentVoter->postCode);
    else
        fprintf(file, "%s %s %s %u F %u\n", currentVoter->IDstring, currentVoter->name,
            currentVoter->surname, currentVoter->age, currentVoter->postCode);
}

/*
Go through the voter RBT recursively and write each voter to output file.
*/
void writeToFileRecurse(node* currentNode, node* NIL, FILE* file) {
    if(currentNode != NIL) {
        voter* currentVoter = (voter*)(currentNode->element);
        writeVoterToFile(currentVoter, file);
        writeToFileRecurse(currentNode->leftChild, NIL, file);
        writeToFileRecurse(currentNode->rightChild, NIL, file);
    }
}

/*
Write voter RBT to file that can be used as an input file
in a different session.
*/
void exportRegistry(RBT* rbt, char* outputFile) {
    FILE* file = fopen(outputFile, "w");
    writeToFileRecurse(rbt->root, rbt->NIL, file);
    fclose(file);
}