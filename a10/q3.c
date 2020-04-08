#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_THREADS 3
uthread_t threads[NUM_THREADS];
int counter = 0;

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void waitForAllOtherThreads() {
  counter++;
}

void* p(void* v) {
  randomStall();
  printf("a\n");
  uthread_mutex_lock(*(uthread_mutex_t*)v);
  waitForAllOtherThreads();
  uthread_mutex_unlock(*(uthread_mutex_t*)v);
  while(counter != NUM_THREADS);
  printf("b\n");
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  uthread_mutex_t mut = uthread_mutex_create();
  for (int i=0; i<NUM_THREADS; i++)
    threads[i] = uthread_create(p, &mut);
  for (int i=0; i<NUM_THREADS; i++)
    uthread_join (threads[i], NULL);
  printf("------\n");
}