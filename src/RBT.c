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
Left rotate a node(fancy way of saying "excange places with its left child, but
keep the tree rules"). Also, make sure that the tree root changes if necessary.
*/
void leftRotation(RBT* redBlackTree, node* givenNode) {
    node* previousRight = givenNode->rightChild;

    givenNode->rightChild = previousRight->rightChild;
    if(givenNode->rightChild != NULL)
        givenNode->rightChild->parent = givenNode;

    if(redBlackTree->root == givenNode)
        redBlackTree->root = previousRight;
    else if(givenNode->parent->leftChild == givenNode)
        givenNode->parent->leftChild = previousRight;
    else
        givenNode->parent->rightChild = previousRight;
    
    previousRight->leftChild = givenNode;
    givenNode->parent = previousRight;
}

/*
Right rotate a node(fancy way of saying "excange places with its right child, but
keep the tree rules"). Also, make sure that the tree root changes if necessary.
*/
void rightRotation(RBT* redBlackTree, node* givenNode) {
    node* previousLeft = givenNode->leftChild;

    givenNode->leftChild = previousLeft->rightChild;
    if(givenNode->leftChild != NULL)
        givenNode->leftChild->parent = givenNode;

    if(redBlackTree->root == givenNode)
        redBlackTree->root = previousLeft;
    else if(givenNode->parent->leftChild == givenNode)
        givenNode->parent->leftChild = previousLeft;
    else
        givenNode->parent->rightChild = previousLeft;
    
    previousLeft->rightChild = givenNode;
    givenNode->parent = previousLeft;
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


void* printRBT(node* currentNode) {
    if(currentNode == NULL) printf("NULL ");
    else printf("%i", *(int*)(currentNode->element));
    if(currentNode != NULL) {
        printRBT(currentNode->leftChild);
        printRBT(currentNode->rightChild);
        printf("\n");
    }
}
