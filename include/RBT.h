/* Create by Antonis Karvelas, sdi1600060.
This is an implementation of a generic red-black tree for the
1st project of K22: Operating Systems.
*/

#include <stdlib.h>
#include <stdio.h>

#define RED 1
#define BLACK 0

#define GET_GRANDPARENT(n) (n->parent != NULL ? (n->parent)->parent : NULL)
#define GET_SIBLING(n) (n->parent == NULL ? NULL : ( \
        (n->parent)->leftChild == n ? (n->parent)->rightChild : (n->parent)->leftChild))
#define GET_UNCLE(n) (GET_GRANDPARENT(n) == NULL ? NULL : GET_SIBLING(n->parent))

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
    node* root;
} typedef RBT;