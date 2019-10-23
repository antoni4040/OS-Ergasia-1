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
    newPostcodeData->voters = initializeRedBlackTree(&alphanumericCompare, &freeVoterNode, &printVoterNode);
    return newPostcodeData;
}

/*
Free node function for the postcode data RBTs.
*/
void freePostcodeDataNode(node* nodeToFree, bool freeElement) {
    if(freeElement) {
        postcodeData* postCodeD = (postcodeData*)nodeToFree->element;
        freeRedBlackTree(postCodeD->voters, false);
        free(postCodeD);
    }
    free(nodeToFree);
}

/*
Compare function for postcode RBTs for search and insert.
*/
int comparePostcodeDataNodes(node* nodeA, node* nodeB) {
    postcodeData* nodeAData = (postcodeData*)nodeA->element;
    postcodeData* nodeBData = (postcodeData*)nodeB->element;
    if(nodeAData->postcode < nodeBData->postcode) 
        return -1;
    else if(nodeAData->postcode > nodeBData->postcode)
        return 1;
    else
        return 0;
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
        hashtable->cells[i] = initializeRedBlackTree(&comparePostcodeDataNodes, &freePostcodeDataNode, NULL);
    }
    return hashtable;
}

/*
Free hashtable, postcode RBTs and voters RBTs in postocode nodes.
*/
void freeHashTable(HT* hashtable) {
    for(int i = 0; i < hashtable->size; i++) {
        freeRedBlackTree(hashtable->cells[i], true);
    }
    free(hashtable->cells);
    free(hashtable);
}

/*
Extract voter postcode from given node and hash it.
*/
size_t hashNodeForHT(HT* hashtable, node* item) {
    voter* givenVoter = (voter*)(item->element);
    char* key = malloc(10 * sizeof(char));
    sprintf(key, "%u", givenVoter->postCode);
    size_t* hash = (size_t*)malloc(sizeof(size_t));
    *hash = 0;
    MurmurHash3_x86_32(key, strlen(key), 522, hash);
    *hash %= hashtable->size;
    size_t hashReturn = *hash;
    free(hash);
    free(key);
    return hashReturn;
}

/*
For a given postcode return voters' RBT.
*/
RBT* searchPostCodeRBT(HT* hashtable, unsigned int postcode) {
    char* key = malloc(10 * sizeof(char));
    sprintf(key, "%u", postcode);
    size_t* hash = (size_t*)malloc(sizeof(size_t));
    *hash = 0;
    MurmurHash3_x86_32(key, strlen(key), 522, hash);
    *hash %= hashtable->size;
    postcodeData* spectrePostCode = initializePostcodeData(postcode);
    node* spectrePostCodeNode = initializeNode(spectrePostCode);
    node* postcodeTree = RBSearch(hashtable->cells[*hash], spectrePostCodeNode);
    hashtable->cells[*hash]->freeNode(spectrePostCodeNode, true);
    free(hash);
    free(key);
    return ((postcodeData*)(postcodeTree->element))->voters;
}

/*
Recurse through voter RBT of a postcode and gather data.
*/
void postcodeStatisticsRecurse(node* givenNode, node* NIL, unsigned int* voters, unsigned int* voted) {
    if(givenNode != NIL) {
        voter* givenVoter = (voter*)(givenNode->element);
        if(givenVoter->hasVoted == VOTED)
            (*voted)++;
        (*voters)++;
        postcodeStatisticsRecurse(givenNode->leftChild, NIL, voters, voted);
        postcodeStatisticsRecurse(givenNode->rightChild, NIL, voters, voted);
    }
}

/*
Recurse through postcode nodes in RBT to print voting statistics.
*/
void postcodeRecurse(node* givenNode, node* NIL) {
    if(givenNode != NIL) {
        unsigned int* voters = malloc(sizeof(unsigned int));
        unsigned int* voted = malloc(sizeof(unsigned int));
        *voters = 0;
        *voted = 0;

        RBT* votersRBT = ((postcodeData*)(givenNode->element))->voters;
        postcodeStatisticsRecurse(votersRBT->root, votersRBT->NIL, voters, voted);
        float percentage = ((float)(*voted)) / ((float)(*voters));
        percentage *= 100;
        printf("# IN %u VOTERS-ARE %u FROM %u, %f%%\n", ((postcodeData*)(givenNode->element))->postcode, 
            *voted, *voters, percentage);
        free(voters);
        free(voted);

        postcodeRecurse(givenNode->leftChild, NIL);
        postcodeRecurse(givenNode->rightChild, NIL);
    }
}

/*
Go through the hashtable and print voting statistics for each postcode.
*/
void postCodeStatistics(HT* hashTable) {
    for(int i = 0; i < hashTable->size; i++) {
        postcodeRecurse(hashTable->cells[i]->root, hashTable->cells[i]->NIL);
    }
}

/*
Add new voter data. If the voter's postcode doesn't exist, create it.
*/
int insertToHashTable(HT* hashtable, node* newVoter) {
    size_t hashValue = hashNodeForHT(hashtable, newVoter);
    unsigned int postcode = ((voter*)(newVoter->element))->postCode;
    postcodeData* postcodeD = initializePostcodeData(postcode);
    node* postcodeDNode = initializeNode(postcodeD);
    node* postcodeTree = RBSearch(hashtable->cells[hashValue], postcodeDNode);
    if(postcodeTree == hashtable->cells[hashValue]->NIL) {
        RBInsert(hashtable->cells[hashValue], postcodeDNode);
        RBInsert(postcodeD->voters, newVoter);
    }
    else {
        postcodeData * foundPostcodeData = (postcodeData*)(postcodeTree->element);
        RBInsert(foundPostcodeData->voters, newVoter);
        hashtable->cells[hashValue]->freeNode(postcodeDNode, true);
    }
    return 0;
}