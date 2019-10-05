/* Create by Antonis Karvelas, sdi1600060.
This is an implementation of a generic red-black tree for the
1st project of K22: Operating Systems.
*/

#include "RBT.h"


// TODO: remove before the end.
int exampleCompare(node* A, node* B)
{
    int elementA = *(int*)(A->element);
    int elementB = *(int*)(B->element);

    if(elementA > elementB) return -1;
    if(elementB < elementA) return 1;
    return 0;
}

/*
Initialize a red-black tree with a given compare function.
The root node will be set to null.
*/
RBT* initializeRedBlackTree(int (*compare)(node* nodeA, node* keyB)) {
    RBT* redBlackTree = malloc(sizeof(RBT));
    redBlackTree->compare = compare;
    redBlackTree->root = NULL;
    return redBlackTree;
}

/*
A simple binary search tree insertion function. It will cause
violations in the red-black tree which we'll fix after the new
node's insertion.
*/
node* simpleBSTInsert(RBT* redBlackTree, node* newNode) {

}


int main() {
    RBT* rbt = initializeRedBlackTree(&exampleCompare);
    return 0;
}