/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is an implementation of a generic hash table, with each cell
in the table being the root of a red black tree.
I thought that would be interesting, as I've already implemented
red-black trees for this project.
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

struct HT {
    RBT** cells;
    int size;
    int (*hash)(node* givenNode);
} typedef HT;

HT* initializeHashtable();
int insertToHashTable(HT* hashtable, node* newItem);

#endif //ERGASIA_1_HT_H