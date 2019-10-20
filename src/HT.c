/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is an implementation of a generic hash table, with each cell
in the table being the root of a red black tree.
I thought that would be interesting, as I've already implemented
red-black trees for this project.
*/

#include "HT.h"

/*
Create a hashtable with HASHTABLE_SIZE number of cells,
each being a future root of an RBT. Set them all to NULL.
*/
HT* initializeHashtable() {
    HT* hashtable = malloc(sizeof(HT));
    hashtable->cells = malloc(HASHTABLE_SIZE * sizeof(RBT*));
    for(int i = 0; i < HASHTABLE_SIZE; i++) {
        hashtable->cells[i] = NULL;
    }
    return hashtable;
}

int insertToHashTable(HT* hashtable, node* newItem) {
    int hashValue = hashtable->hash(newItem);
}