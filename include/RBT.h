/* Create by Antonis Karvelas, sdi1600060.
This is an implementation of a generic red-black tree for the
1st project of K22: Operating Systems.
*/

#include <stdlib.h>

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
convenience a function pointer to a compare function. This function
will be used to compare two nodes for search and insertion.
*/
struct RBT {
    int (*compare)(node* nodeA, node* keyB);
    node* root;
} typedef RBT;