#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

/* Feel free to change/remove these variables and/or add your own. */
struct Node {
  int size;
  struct Node *next; 
  // 16 bytes long struct
};

static struct Node *head;
// Base address of the allocated heap.
static char *heap;
// Size of the complete heap.
static int heapsize;
// Next unallocated location in the heap.
static int top;

/* Initialize your memory manager to manage the given heap. */
void mymalloc_init(char *_heap, int _heapsize) {
  heap = _heap;
  heapsize = _heapsize;
  top = 0;
  head = NULL;
  /* TODO: Any other initialization you want to do */
  /* NOTE! Each call to mymalloc_init should initialize a brand new heap
     (discarding the old one), as it will be called once per test.
     Therefore, be sure to *initialize all your variables* here! */
}

/* Allocate a block of memory of the given size, or NULL if unable. 

Returns: a pointer aligned to 8 bytes, or NULL if allocation failed. */
void *mymalloc(int size) {
  /* This is a dumb implementation of malloc, adapted slightly from the slides. 

  You will augment this solution to make a real malloc. */

  /* Round `size` up to a multiple of 8 bytes */
  /* TODO: Make room for any extra metadata you want to store */
  size = (size + 7) / 8 * 8;
  char* ptr = &heap[top];
  
  printf("initial ptr address: %ld\n", (long) ptr);
  struct Node* b = &heap[top];
  top += size + 16;
  *ptr = size;
  ptr += 16;

  /* TODO: Implement and walk a free list before allocating off `top` as a last resort */
  struct Node* n = head;

  if(size < 0 || size > heapsize) {
    /* There is not enough room in the heap - fail */
    return NULL;
  }

  if (n != NULL) {
    char *addr = ptr - 16;
    // remove head without child 
    if (n->next == NULL && n->size >= size) {
      //char *addr = (n->address - 8);
      head = NULL;
      printf("rtn free address: %ld\n", (long) addr);
      return addr;
   }
   // else if there is a child and a head
    while(n->next != NULL) {
      // delete head with next
      if (n == head && n->size >= size) {
        addr = ;
        //int* addr = head->address;
        head = n->next;
        printf("free head w/ child addr: %ld\n", (long) addr);
        return addr;
      }
      if(n->next->size >= size)  {
        struct Node* nnNode = n->next->next;
        struct Node* nNode = n->next;
        addr = (nNode->address) + 8;
        //int* addr = nNode->address;
        n->next = nnNode;
        return addr;
      }
      n = n->next;
    }

  } else {
    if (heapsize - size < top) {
      return NULL;
    }

    /* Allocate the memory from `top` to `top+size` and return it */
    printf("ptr address: %ld\n", (long) ptr);
    return (void *) ptr;
  }
  
}

/* Free the given block of memory. */
void myfree(void *ptr) {
  /* TODO: Our dumb implementation does not allow for any freeing. Implement me! */
  // if head exists
  /* if (head != NULL) {
    // allocate 8 bytes (size) + client 
    struct Node *n = malloc(sizeof(struct Node));
    // go from client to beginning of pointer memory block to get address
    n->address = ((ptr) - 8);
    // go from client to beginning of pointer memory block (deref) to get size 
    n->size = *((int* ) (ptr) - 8);
    n->next = head;
    head = n;
    printf("free child node address: %ld\n", (long) ptr - 8);
  } else {
    printf("node address: %ld\n", (long) ptr);
    head = malloc(sizeof(struct Node));
    head->address = (ptr - 8);
    printf("head node address: %ld\n", (long) (ptr - 8));
    head->size = *((int *) (ptr - 8));
    printf("head node freed");
    head->next = NULL;
  } */
  struct Node* n = ptr - 16; 

  if (head != NULL) {
    n->next = head;
    head = n;
  } else {
    head = n;
  }
  //free(((ptr) - 8));
}
