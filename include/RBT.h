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

#define SENTINEL(x) (x->element == NULL)

struct node {
    void* element;
    unsigned int color: 1;
    struct node* parent;
    struct node* leftChild;
    struct node* rightChild;
} typedef node;

/*
The red-black tree data structure. It contains a root node and for
convenience a function pointer to a compare function. This function
will be used to compare two nodes for search and insertion.
*/
struct RBT {
    int (*compare)(node* nodeA, node* keyB);
    void (*freeNode)(node* nodeToDie);
    void (*printNode)(node* nodeToPrint);
    node* root;
    node* NIL;
} typedef RBT;

node*       initializeNode(void* element);
RBT*        initializeRedBlackTree(int (*compare)(node* nodeA, node* keyB));
node*       getGrandParent(node* givenNode);
node*       getSibling(node* givenNode);
node*       getUncle(node* givenNode);
void        leftRotation(RBT* redBlackTree, node* givenNode);
void        rightRotation(RBT* redBlackTree, node* givenNode);
int         simpleBSTInsert(RBT* redBlackTree, node* newNode);
void        swapColors(node* nodeA, node* nodeB);
int         fixRBT(RBT* redBlackTree, node* newNode);
int         RBTInsert(RBT* redBlackTree, node* newNode);
node*       RBTSearch(RBT* redBlackTree, node* nodeForSearch);
int         RBTDelete(RBT* redBlackTree, node* nodeToDie);

int         RBInsert(RBT* rbt, node* newNode);
int         RBDelete(RBT* rbt, node* nodeToDie);
#endif //ERGASIA_1_RBT_H