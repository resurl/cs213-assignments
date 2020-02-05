#include <stdlib.h>
#include <stdio.h>

/**
 * A node of the binary tree containing the node's integer value
 * and pointers to its right and left children (or null).
 */
struct Node {
  // TODO
};

/**
 * Create a new node with no children.
 */
struct Node* create (int value) {
  // TODO
}

/**
 * Insert the node n into the binary tree rooted by toNode.
 */
void insert (struct Node* toNode, struct Node* n) {
  // TODO
}

/**
 * Print the contents entire binary tree in order of ascending integer value.
 */
void printInOrder (struct Node* node) {
  // TODO
}

/**
 * Create a new tree populated with values provided on the command line and
 * print it in depth-first order.
 */
int main (int argc, char* argv[]) {
  // read values from command line and add them to the tree
  for (int i=1; i<argc; i++) {
    int value = atoi (argv [i]);
    printf ("%d\n", value);
  }
  // TODO
}