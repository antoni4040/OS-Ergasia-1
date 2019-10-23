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
} typedef electionManager;


bool                checkInputFileExists(char* inputFile);
uint32_t            getNumberOfVoters(FILE* input);
void                readVotersAndUpdateStructures(FILE* input, electionManager* manager);
void                insertVoterToDataStructs(electionManager* manager, char* line, bool print);
int                 getVotersFromFile(char* inputFile, electionManager* manager);
electionManager*    initializeElectionManager(unsigned int numberOfUpdates);
void                update(electionManager* manager);

#endif //ERGASIA_1_FILES_H