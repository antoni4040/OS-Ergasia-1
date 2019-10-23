/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is an implementation of a generic red-black tree.
*/

#ifndef ERGASIA_1_RBT_H
#define ERGASIA_1_RBT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define RED 1
#define BLACK 0

struct node {
    void* element;
    unsigned int color: 1;
    struct node* parent;
    struct node* leftChild;
    struct node* rightChild;
} typedef node;

/*
The red-black tree data structure. It contains a root node and for
convenience a function pointer to a compare function to use when searching
and inserting, a function pointer to a free function that can free a node
and its containing element and a function pointer to a print function
that can print a node.
Also, we have a sentinel node, NIL, to help with the various operations.
*/
struct RBT {
    int (*compare)(node* nodeA, node* keyB);
    void (*freeNode)(node* nodeToDie, bool freeElement);
    void (*printNode)(node* nodeToPrint);
    node* root;
    node* NIL;
} typedef RBT;

node*       initializeNode(void* element);
RBT*        initializeRedBlackTree(int (*compare)(node* nodeA, node* keyB),
            void (*freeN)(node* nodeToFree, bool freeElement), void (*printN)(node* nodeToPrint));
void        freeRedBlackTree(RBT* rbt, bool freeElement);
void        leftRotation(RBT* redBlackTree, node* givenNode);
void        rightRotation(RBT* redBlackTree, node* givenNode);
node*       RBSearch(RBT* redBlackTree, node* nodeForSearch);
node*       RBInsert(RBT* rbt, node* newNode);
int         RBDelete(RBT* rbt, node* nodeToDie);
#endif //ERGASIA_1_RBT_H