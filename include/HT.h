/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is an implementation of a custom hash table, with each cell
in the table being the root of a red black tree.
These red-black trees have nodes with postcodeData elements, that
also have red-black trees with voter data for each postcode.
I thought that would be interesting, as I've already implemented
red-black trees for this project.
It's also pretty to imagine.
*/

#ifndef ERGASIA_1_HT_H
#define ERGASIA_1_HT_H

/*
The better way to do it is to find a prime number that is
at least n*1.33 times the number of items in the hashtable,
but counting the number of distinct postcodes is overkill and this
is fast enough. 521 is a prime number.
*/
#define HASHTABLE_SIZE 521

#include "RBT.h"
#include "murmur3.h"
#include "voter.h"

struct HT {
    RBT** cells;
    int size;
} typedef HT;

struct postcodeData {
    unsigned int postcode;
    RBT* voters;
} typedef postcodeData;

postcodeData*   initializePostcodeData(unsigned int postcode);
HT*             initializeHashtable();
void            freeHashTable(HT* hashtable);
size_t          hashNodeForHT(HT* hashtable, node* item);
RBT*            searchPostCodeRBT(HT* hashtable, unsigned int postcode);
int             insertToHashTable(HT* hashtable, node* newItem);
void            postCodeStatistics(HT* hashTable);

#endif //ERGASIA_1_HT_H