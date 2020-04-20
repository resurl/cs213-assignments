//////
// This is part of the solution to UBC CPSC 213, Assignment 11.
// Do no share this code or any portion of it with anyone in any way.
// Do not remove this comment block.
//////

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_sem.h"

#define MAX_ITEMS      10
#define NUM_ITERATIONS 100
#define NUM_THREADS    4

int histogram [MAX_ITEMS + 1];

struct Pool {
  uthread_sem_t mutex;
  uthread_sem_t freeSlots;
  uthread_sem_t fullSlots;
  uthread_sem_t done;
  int           items;
};

struct Pool* createPool() {
  struct Pool* pool = malloc (sizeof (struct Pool));
  pool->mutex     = uthread_sem_create (1);
  pool->freeSlots = uthread_sem_create (MAX_ITEMS);
  pool->fullSlots = uthread_sem_create (0);
  pool->done      = uthread_sem_create (0);
  pool->items     = 0;
  return pool;
}

void* producer (void* pv) {
  struct Pool* p = pv;
  
  for (int i=0; i<NUM_ITERATIONS; i++) {
    uthread_sem_wait (p->freeSlots);
    uthread_sem_wait (p->mutex);
      p->items++;
      assert (p->items >= 0 && p->items <= MAX_ITEMS);
      histogram [p->items] ++;
    uthread_sem_signal (p->mutex);
    uthread_sem_signal (p->fullSlots);
  }
  uthread_sem_signal (p->done);
  return NULL;
}

void* consumer (void* pv) {
  struct Pool* p = pv;
  
  for (int i=0; i<NUM_ITERATIONS; i++) {
    uthread_sem_wait (p->fullSlots);
    uthread_sem_wait (p->mutex);
      p->items--;
      assert (p->items >= 0 && p->items <= MAX_ITEMS);
      histogram [p->items] ++;
    uthread_sem_signal (p->mutex);
    uthread_sem_signal (p->freeSlots);
  }
  uthread_sem_signal (p->done);
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (4);
  struct Pool* p = createPool();
  
  for (int i = 0; i < NUM_THREADS; i++)
    uthread_detach (uthread_create((i & 1)? producer : consumer, p));
  for (int i = 0; i < NUM_THREADS; i++)
    uthread_sem_wait (p->done);
  
  printf ("items value histogram:\n");
  int sum=0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf ("  items=%d, %d times\n", i, histogram [i]);
    sum += histogram [i];
  }
  assert (sum == NUM_THREADS * NUM_ITERATIONS);
}
