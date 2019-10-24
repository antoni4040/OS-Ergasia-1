/* 
Create by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
Everything that has to do with file reading/writing is in here.
*/

#ifndef ERGASIA_1_FILES_H
#define ERGASIA_1_FILES_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h> 
#include <stdint.h>

#include "voter.h"
#include "BF.h"
#include "RBT.h"
#include "HT.h"

struct electionManager {
    BF* bloomFilter;
    RBT* redBlackTree;
    HT* hashTable;
    unsigned int numberOfUpdates;
    unsigned int updatesSoFar;
    uint32_t numberOfVoters;
    char* outputFile;
} typedef electionManager;


electionManager*    initializeElectionManager(unsigned int numberOfUpdates, char* outputFile);
void                update(electionManager* manager);
void                RBTHealth(RBT* rbt, node* currentNode, int level);
bool                checkInputFileExists(char* inputFile);
uint32_t            getNumberOfVoters(FILE* input);
int                 insertVoterToDataStructs(electionManager* manager, char* line, bool print);
void                readVotersAndUpdateStructures(FILE* input, electionManager* manager);
int                 getVotersFromFile(char* inputFile, electionManager* manager);

#endif //ERGASIA_1_FILES_H