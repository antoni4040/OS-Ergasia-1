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
Create new postcode and prepare a red-black tree for the
postcode's voters' data.
*/
postcodeData* initializePostcodeData(unsigned int postcode) {
    postcodeData* newPostcodeData = malloc(sizeof(postcodeData));
    newPostcodeData->postcode = postcode;
    newPostcodeData->voters = initializeRedBlackTree(&alphanumericCompare);
    newPostcodeData->voters->freeNode = &freeNode;
    newPostcodeData->voters->printNode = &printNode;
    return newPostcodeData;
}

/*
Create a hashtable with HASHTABLE_SIZE number of cells,
each being a future root of an RBT. Set them all to NULL.
*/
HT* initializeHashtable() {
    HT* hashtable = malloc(sizeof(HT));
    hashtable->size = HASHTABLE_SIZE;
    hashtable->cells = malloc(HASHTABLE_SIZE * sizeof(RBT*));
    for(int i = 0; i < HASHTABLE_SIZE; i++) {
        hashtable->cells[i] = initializeRedBlackTree(NULL);
    }
    return hashtable;
}

/*
Extract voter postcode from given node and hash it.
*/
size_t hashNodeForHT(HT* hashtable, node* item) {
    voter* givenVoter = (voter*)(item->element);
    char* key = malloc(sizeof(10) * sizeof(char));
    sprintf(key, "%u", givenVoter->postCode);
    size_t* hash = malloc(sizeof(size_t));
    MurmurHash3_x86_32(key, strlen(key), 522, hash);
    *hash %= hashtable->size;
    return *hash;
}

/*
If a postcode exists, return it, else create it and return it.
*/
node* RBTSearchOrAddPostcode(RBT* rbt, unsigned int postcode) {
    if(rbt->root == NULL) {
        postcodeData* newPostCodeData = initializePostcodeData(postcode);
        node* newNode = initializeNode(newPostCodeData);
        rbt->root = newNode;
        return rbt->root;
    }

    // Iteratively get to the bottom of the RBT, until we hit a null child.
    node* parentNode = NULL;
    node* currentNode = rbt->root;
    while(currentNode != NULL){
        if(((voter*)(currentNode->element))->postCode < postcode){
            parentNode = currentNode;
            currentNode = currentNode->leftChild;
        }
        else if(((voter*)(currentNode->element))->postCode > postcode) {
            parentNode = currentNode;
            currentNode = currentNode->rightChild;
        }
        else {
            return currentNode;
        }
    }

    // Having kept the parent, make the new node either its left or right child.
    if(((voter*)(parentNode->element))->postCode > postcode){
        postcodeData* newPostCodeData = initializePostcodeData(postcode);
        node* newNode = initializeNode(newPostCodeData);
        parentNode->leftChild = newNode;
        parentNode->leftChild->parent = parentNode;
        return parentNode->leftChild;
    }
    else {
        postcodeData* newPostCodeData = initializePostcodeData(postcode);
        node* newNode = initializeNode(newPostCodeData);
        parentNode->rightChild = newNode;
        parentNode->rightChild->parent = parentNode;
        return parentNode->rightChild;
    }
}

/*
Add new voter data. If the voter's postcode doesn't exist, create it.
*/
int insertToHashTable(HT* hashtable, node* newVoter) {
    size_t hashValue = hashNodeForHT(hashtable, newVoter);
    unsigned int postcode = ((voter*)(newVoter->element))->postCode;
    node* postcodeTreeNode = RBTSearchOrAddPostcode(hashtable->cells[hashValue], postcode);
    RBT* postcodeTree = ((postcodeData*)(postcodeTreeNode->element))->voters;
    RBTInsert(postcodeTree, newVoter);
}