/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is the implementation of the postcode data structure.
*/

#ifndef ERGASIA_1_POSTCODE_H
#define ERGASIA_1_POSTCODE_H

#include "RBT.h"

struct postcodeData {
    unsigned int postcode;
    RBT* voters;
} typedef postcodeData;

#endif //ERGASIA_1_POSTCODE_H