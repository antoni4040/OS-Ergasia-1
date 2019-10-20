/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is an implementation of a generic red-black tree.
*/

#include "RBT.h"

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

    givenNode->rightChild = previousRight->leftChild;
    if(givenNode->rightChild != NULL)
        givenNode->rightChild->parent = givenNode;

    previousRight->parent = givenNode->parent;

    if(previousRight->parent == NULL) {
        redBlackTree->root = previousRight;
    }
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

    previousLeft->parent = givenNode->parent;

    if(previousLeft->parent == NULL)
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
        if(redBlackTree->compare(newNode, currentNode) < 0){
            parentNode = currentNode;
            currentNode = currentNode->leftChild;
        }
        else if(redBlackTree->compare(newNode, currentNode) > 0) {
            parentNode = currentNode;
            currentNode = currentNode->rightChild;
        }
        else {
            return -1; //newNode already exists.
        }
    }

    // Having kept the parent, make the new node either its left or right child.
    if(redBlackTree->compare(newNode, parentNode) < 0){
        parentNode->leftChild = newNode;
        newNode->parent = parentNode;
    }
    else {
        parentNode->rightChild = newNode;
        newNode->parent = parentNode;
    }

    return 0;
}

/*
Little helping function to swap node colors when fixing the RBT.
*/
void swapColors(node* nodeA, node* nodeB) {
    int colorA = nodeA->color;
    nodeA->color = nodeB->color;
    nodeB->color = colorA;
}

/*
Fix RBT violations after insertions.
*/
int fixRBT(RBT* redBlackTree, node* newNode) {
    if(newNode->parent == NULL) {
        newNode->color = BLACK;
        return 0;
    }

    node* uncle = getUncle(newNode);
    node* parent = newNode->parent;
    node* grandParent = getGrandParent(newNode);

    if(parent->color == RED) {
        if(uncle != NULL && uncle->color == RED) {
            uncle->color = BLACK;
            parent->color = BLACK;
            grandParent->color = RED;
            fixRBT(redBlackTree, grandParent);
        }
        else if(grandParent != NULL) {
            if(parent->leftChild == newNode) {
                if(grandParent->leftChild == parent) {
                    rightRotation(redBlackTree, grandParent);
                }
                else {
                    rightRotation(redBlackTree, parent);
                    leftRotation(redBlackTree, grandParent);
                }
            }
            else {
                if(grandParent->leftChild == parent) {
                    leftRotation(redBlackTree, parent);
                    rightRotation(redBlackTree, grandParent);
                }
                else {
                    leftRotation(redBlackTree, grandParent);
                }
            }

            swapColors(parent, grandParent);
        }
    }
}

/*
Do a simple binary search tree insertion and then fix all the
violations to make it a valid red-black tree.
*/
int RBTInsert(RBT* redBlackTree, node* newNode) {
    int insert = simpleBSTInsert(redBlackTree, newNode);
    if(insert == -1) return -1;
    fixRBT(redBlackTree, newNode);
    return 0;
}

/*
Go through the tree and search for a given node. This node
was created only for the compare to work, so free it afterwards.
*/
node* RBTSearch(RBT* redBlackTree, node* nodeForSearch) {
    if(redBlackTree->root == NULL) {
        redBlackTree->freeNode(nodeForSearch);
        return NULL;   //Tree is empty, nothing to find.
    }

    // Iteratively get to the bottom of the RBT, until we hit a null child.
    node* parentNode = NULL;
    node* currentNode = redBlackTree->root;
    while(currentNode != NULL){
        if(redBlackTree->compare(nodeForSearch, currentNode) < 0){
            parentNode = currentNode;
            currentNode = currentNode->leftChild;
        }
        else if(redBlackTree->compare(nodeForSearch, currentNode) > 0) {
            parentNode = currentNode;
            currentNode = currentNode->rightChild;
        }
        else {
            redBlackTree->freeNode(nodeForSearch);
            return currentNode; //Node found.
        }
    }
    redBlackTree->freeNode(nodeForSearch);
    return NULL; //Node not found.
}