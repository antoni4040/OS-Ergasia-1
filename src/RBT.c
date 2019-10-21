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
    redBlackTree->NIL = initializeNode(NULL);
    redBlackTree->NIL->color = BLACK;
    redBlackTree->root = redBlackTree->NIL;
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
Go through the tree and search for a given node. This node
was created only for the compare to work, so free it afterwards.
*/
node* RBTSearch(RBT* rbt, node* nodeForSearch) {
    if(rbt->root == rbt->NIL) {
        rbt->freeNode(nodeForSearch);
        return rbt->NIL;   //Tree is empty, nothing to find.
    }

    // Iteratively get to the bottom of the RBT, until we hit a null child.
    node* parentNode = rbt->NIL;
    node* currentNode = rbt->root;
    while(currentNode != rbt->NIL){
        if(rbt->compare(nodeForSearch, currentNode) < 0){
            parentNode = currentNode;
            currentNode = currentNode->leftChild;
        }
        else if(rbt->compare(nodeForSearch, currentNode) > 0) {
            parentNode = currentNode;
            currentNode = currentNode->rightChild;
        }
        else {
            rbt->freeNode(nodeForSearch);
            return currentNode; //Node found.
        }
    }
    rbt->freeNode(nodeForSearch);
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
int RBInsert(RBT* rbt, node* newNode) {
    node* parent = rbt->NIL;
    node* current = rbt->root;
    while(current != rbt->NIL) {
        parent = current;
        if(rbt->compare(newNode, current) < 0)
            current = current->leftChild;
        else if(rbt->compare(newNode, current) > 0)
            current = current->rightChild;
        else 
            return -1;
    }
    newNode->parent = parent;
    if(parent == rbt->NIL)
        rbt->root = newNode;
    else if(rbt->compare(newNode, parent) < 0)
        parent->leftChild = newNode;
    else
        parent->rightChild = newNode;
    newNode->leftChild = rbt->NIL;
    newNode->rightChild = rbt->NIL;
    newNode->color = RED;
    RBInsertFixup(rbt, newNode);
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
    node* w;
    while(current != rbt->NIL && current->color == BLACK) {
        if(current == current->parent->leftChild) {
            w = current->parent->rightChild;
            if(w->color == RED) {
                w->color = BLACK;
                current->parent->color = RED;
                leftRotation(rbt, current->parent);
                w = current->parent->rightChild;
            }
            if(w->leftChild->color == BLACK && w->rightChild->color==BLACK) {
                w->color = RED;
                current = current->parent;
            }
            else { 
                if(w->rightChild->color == BLACK) {
                    w->leftChild->color = BLACK;
                    w->color = RED;
                    rightRotation(rbt, w);
                    w = current->parent->rightChild;
                }
                w->color = current->parent->color;
                current->parent->color = BLACK;
                w->rightChild->color = BLACK;
                leftRotation(rbt, current->parent);
                current = rbt->root;
            }
        }
        else {
            w = current->parent->leftChild;
            if(w->color == RED) {
                w->color = BLACK;
                current->parent->color = RED;
                rightRotation(rbt, current->parent);
                w = current->parent->leftChild;
            }
            if(w->rightChild->color == BLACK && w->leftChild->color==BLACK) {
                w->color = RED;
                current = current->parent;
            }
            else { 
                if(w->leftChild->color == BLACK) {
                    w->rightChild->color = BLACK;
                    w->color = RED;
                    leftRotation(rbt, w);
                    w = current->parent->leftChild;
                }
                w->color = current->parent->color;
                current->parent->color = BLACK;
                w->leftChild->color = BLACK;
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
    int y_original_color = copy->color;
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
        y_original_color = copy->color;
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
    if(y_original_color == BLACK)
        RBDeleteFixup(rbt, nodeToDie);
}