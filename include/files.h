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

bool        checkInputFileExists(char* inputFile);
uint32_t    getNumberOfVoters(FILE* input);
void        readVotersAndUpdateStructures(FILE* input, BF* bf, RBT* rbt);
int         getVotersFromFile(char* inputFile);

#endif //ERGASIA_1_FILES_H