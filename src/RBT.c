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
    return newNode;
}

/*
Initialize a red-black tree with a given compare function.
The root node will be set to null.
*/
RBT* initializeRedBlackTree(int (*compare)(node* nodeA, node* keyB),
    void (*freeN)(node* nodeToFree, bool freeElement), void (*printN)(node* nodeToPrint)) {
    RBT* redBlackTree = malloc(sizeof(RBT));
    redBlackTree->compare = compare;
    redBlackTree->freeNode = freeN;
    redBlackTree->printNode = printN;
    redBlackTree->NIL = initializeNode(NULL);
    redBlackTree->NIL->color = BLACK;
    redBlackTree->NIL->leftChild = redBlackTree->NIL;
    redBlackTree->NIL->rightChild = redBlackTree->NIL;
    redBlackTree->root = redBlackTree->NIL;
    return redBlackTree;
}

/*
Recursively free the nodes of an RBT using the given freeNode function.
*/
void freeRBTRecurse(RBT* rbt, node* givenNode, bool freeElement) {
    if(givenNode != rbt->NIL) {
        freeRBTRecurse(rbt, givenNode->leftChild, freeElement);
        freeRBTRecurse(rbt, givenNode->rightChild, freeElement);
        rbt->freeNode(givenNode, freeElement);
    }
}

/*
Free the content of an RBT.
*/
void freeRedBlackTree(RBT* rbt, bool freeElement) {
    freeRBTRecurse(rbt, rbt->root, freeElement);
    free(rbt->NIL);
    free(rbt);
}

/*
Left rotate a node(fancy way of saying "excange places with its left child, but
keep the tree rules"). Also, make sure that the tree root changes if necessary.
*/
void leftRotation(RBT* rbt, node* givenNode) {
    node* previousRight = givenNode->rightChild;

    givenNode->rightChild = previousRight->leftChild;
    if(givenNode->rightChild != rbt->NIL)
        givenNode->rightChild->parent = givenNode;

    previousRight->parent = givenNode->parent;

    if(previousRight->parent == rbt->NIL) {
        rbt->root = previousRight;
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
void rightRotation(RBT* rbt, node* givenNode) {
    node* previousLeft = givenNode->leftChild;

    givenNode->leftChild = previousLeft->rightChild;
    if(givenNode->leftChild != rbt->NIL)
        givenNode->leftChild->parent = givenNode;

    previousLeft->parent = givenNode->parent;

    if(previousLeft->parent == rbt->NIL)
        rbt->root = previousLeft;
    else if(givenNode->parent->leftChild == givenNode)
        givenNode->parent->leftChild = previousLeft;
    else
        givenNode->parent->rightChild = previousLeft;
    
    previousLeft->rightChild = givenNode;
    givenNode->parent = previousLeft;
}

/*
Given a node (that is the root of a subtree), go through
the left children to find the minimum value.
*/
node* TreeMinimum(RBT* rbt, node* givenNode) {
    node* minNode = givenNode;
    while(minNode->leftChild != rbt->NIL)
        minNode = minNode->leftChild;
    return minNode;
}

/*
Go through the tree and search for a given node.
*/
node* RBSearch(RBT* rbt, node* nodeForSearch) {
    if(rbt->root == rbt->NIL) {
        return rbt->NIL;   //Tree is empty, nothing to find.
    }

    // Iteratively get to the bottom of the RBT, until we hit a null child.
    node* currentNode = rbt->root;
    while(currentNode != rbt->NIL){
        if(rbt->compare(nodeForSearch, currentNode) < 0){
            currentNode = currentNode->leftChild;
        }
        else if(rbt->compare(nodeForSearch, currentNode) > 0) {
            currentNode = currentNode->rightChild;
        }
        else {
            return currentNode; //Node found.
        }
    }
    return rbt->NIL; //Node not found.
}


/*
Red-Black Tree fix violations as described in CLRS.
*/
void RBInsertFixup(RBT* rbt, node* newNode) {
    node* uncle;
    while(newNode->parent->color == RED) {
        if(newNode->parent == newNode->parent->parent->leftChild) {
            uncle = newNode->parent->parent->rightChild;
            if(uncle->color == RED) {
                newNode->parent->color = BLACK;
                uncle->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            }
            else {
                if(newNode == newNode->parent->rightChild) {
                    newNode = newNode->parent;
                    leftRotation(rbt, newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                rightRotation(rbt, newNode->parent->parent);
            }
        }
        else {
            uncle = newNode->parent->parent->leftChild;
            if(uncle->color == RED) {
                newNode->parent->color = BLACK;
                uncle->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            }
            else {
                if(newNode == newNode->parent->leftChild) {
                    newNode = newNode->parent;
                    rightRotation(rbt, newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                leftRotation(rbt, newNode->parent->parent);
            }
            if(newNode == rbt->root) break;
        }
    }
    rbt->root->color = BLACK;
}

/*
Red-Black Tree Insert as described in CLRS.
*/
node* RBInsert(RBT* rbt, node* newNode) {
    node* parent = rbt->NIL;
    node* current = rbt->root;

    newNode->leftChild = rbt->NIL;
    newNode->rightChild = rbt->NIL;

    while(current != rbt->NIL) {
        parent = current;
        if(rbt->compare(newNode, current) < 0)
            current = current->leftChild;
        else if(rbt->compare(newNode, current) > 0)
            current = current->rightChild;
        else {
            rbt->freeNode(newNode, true);
            return current;
        }
    }
    newNode->parent = parent;
    if(parent == rbt->NIL)
        rbt->root = newNode;
    else if(rbt->compare(newNode, parent) < 0)
        parent->leftChild = newNode;
    else
        parent->rightChild = newNode;
    newNode->color = RED;
    RBInsertFixup(rbt, newNode);

    return NULL;
}

/*
Red-Black Tree Node Transplant as described in CLRS.
*/
void RBTransplant(RBT* rbt, node* removed, node* added) {
    if(removed->parent == rbt->NIL) 
        rbt->root = added;
    else if(removed == removed->parent->leftChild)
        removed->parent->leftChild = added;
    else
        removed->parent->rightChild = added;
    added->parent = removed->parent;
}

/*
Red-Black Tree Deletion Fix Violations as described in CLRS.
*/
void RBDeleteFixup(RBT* rbt, node* current) {
    node* sibling;
    while(current != rbt->NIL && current->color == BLACK) {
        if(current == current->parent->leftChild) {
            sibling = current->parent->rightChild;
            if(sibling->color == RED) {
                sibling->color = BLACK;
                current->parent->color = RED;
                leftRotation(rbt, current->parent);
                sibling = current->parent->rightChild;
            }
            if(sibling->leftChild->color == BLACK && sibling->rightChild->color==BLACK) {
                sibling->color = RED;
                current = current->parent;
            }
            else { 
                if(sibling->rightChild->color == BLACK) {
                    sibling->leftChild->color = BLACK;
                    sibling->color = RED;
                    rightRotation(rbt, sibling);
                    sibling = current->parent->rightChild;
                }
                sibling->color = current->parent->color;
                current->parent->color = BLACK;
                sibling->rightChild->color = BLACK;
                leftRotation(rbt, current->parent);
                current = rbt->root;
            }
        }
        else {
            sibling = current->parent->leftChild;
            if(sibling->color == RED) {
                sibling->color = BLACK;
                current->parent->color = RED;
                rightRotation(rbt, current->parent);
                sibling = current->parent->leftChild;
            }
            if(sibling->rightChild->color == BLACK && sibling->leftChild->color==BLACK) {
                sibling->color = RED;
                current = current->parent;
            }
            else { 
                if(sibling->leftChild->color == BLACK) {
                    sibling->rightChild->color = BLACK;
                    sibling->color = RED;
                    leftRotation(rbt, sibling);
                    sibling = current->parent->leftChild;
                }
                sibling->color = current->parent->color;
                current->parent->color = BLACK;
                sibling->leftChild->color = BLACK;
                rightRotation(rbt, current->parent);
                current = rbt->root;
            }
        }
    }
    current->color = BLACK;
}

/*
Red-Black Tree Deletion as described in CLRS.
*/
int RBDelete(RBT* rbt, node* nodeToDie) {
    node* copy = nodeToDie;
    node* current;
    int rememberColor = copy->color;
    if(nodeToDie->leftChild == rbt->NIL) {
        current = nodeToDie->rightChild;
        RBTransplant(rbt, nodeToDie, nodeToDie->rightChild);
    }
    else if(nodeToDie->rightChild == rbt->NIL) {
        current = nodeToDie->leftChild;
        RBTransplant(rbt, nodeToDie, nodeToDie->leftChild);
    }
    else {
        copy = TreeMinimum(rbt, nodeToDie->rightChild);
        rememberColor = copy->color;
        current = copy->rightChild;
        if(copy->parent == nodeToDie)
            current->parent = copy;
        else {
            RBTransplant(rbt, copy, copy->rightChild);
            copy->rightChild = nodeToDie->rightChild;
            copy->rightChild->parent = copy;
        }
        RBTransplant(rbt, nodeToDie, copy);
        copy->leftChild = nodeToDie->leftChild;
        copy->leftChild->parent = copy;
        copy->color = nodeToDie->color;
    }
    if(rememberColor == BLACK)
        RBDeleteFixup(rbt, nodeToDie);
    return 0;
}