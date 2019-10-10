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
Given an element, encapsulate it in a node. Initial color is RED.
Parent and color may change during insertion.
*/
node* initializeNode(void* element) {
    node* newNode = malloc(sizeof(node));
    newNode->element = element;
    newNode->color = RED;
    newNode->parent = NULL;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
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
Return a node's grandparent in the tree. It will return null if
the given node is a direct child of the root node.
*/
node* getGrandParent(node* givenNode) {
    return (givenNode->parent != NULL ? (givenNode->parent)->parent : NULL);
}

/*
Return a node's sibling. It will return null if the 
given node is the root node.
*/
node* getSibling(node* givenNode) {
    node* parent = givenNode->parent;
    if(parent == NULL) return NULL;
    if(parent->leftChild == givenNode) return parent->rightChild;
    return  parent->leftChild;
}

/*
Return a node's uncle, meaning the sibling of its parent.
It will return null if the given node is the root node or the 
direct child of the root node.
*/
node* getUncle(node* givenNode) {
    if(givenNode->parent == NULL) return NULL;
    return getSibling(givenNode->parent);
}

/*
A simple binary search tree insertion function. It will cause
violations in the red-black tree which we'll fix after the new
node's insertion.
*/
int simpleBSTInsert(RBT* redBlackTree, node* newNode) {
    if(redBlackTree->root == NULL) {
        redBlackTree->root = newNode;   //Tree is empty, newNode becomes the root.
    }

    // Iteratively get to the bottom of the RBT, until we hit a null child.
    node* parentNode = NULL;
    node* currentNode = redBlackTree->root;
    while(currentNode != NULL){
        if(redBlackTree->compare(newNode, currentNode) == -1){
            parentNode = currentNode;
            currentNode = currentNode->leftChild;
        }
        else if(redBlackTree->compare(newNode, currentNode) == 1) {
            parentNode = currentNode;
            currentNode = currentNode->rightChild;
        }
        else {
            return -1; //newNode already exists.
        }
    }

    // Having kept the parent, make the new node either its left or right child.
    if(redBlackTree->compare(newNode, parentNode) == -1){
        parentNode->leftChild = newNode;
    }
    else {
        parentNode->rightChild = newNode;
    }

    return 0;
}


// void* printRBT(node* currentNode) {
//     if(currentNode == NULL) printf("NULL\n");
//     else printf("%i", *(int*)(currentNode->element));
//     if(currentNode->leftChild != NULL) 
// }


int main() {
    RBT* rbt = initializeRedBlackTree(&exampleCompare);
    int* a = malloc(sizeof(int));
    *a = 10;
    node* nd = initializeNode((void*)a);
    int b = simpleBSTInsert(rbt, nd);
    return 0;
}