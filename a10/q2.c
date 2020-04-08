#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

const int N = 100000;
int cnt = 0;

uthread_t t0, t1, t2;

void countUp() {
  for (int i=0; i<N; i++)
    cnt += 1;
}

void countDown() {
  for (int i=0; i<N; i++)
    cnt -= 1;
}

void* count(void* v) {
  uthread_mutex_lock(*(uthread_mutex_t*)v);
  countUp();
  countDown();
  uthread_mutex_unlock(*(uthread_mutex_t*)v);
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  uthread_mutex_t mut = uthread_mutex_create();
  t0 = uthread_create(count, &mut);
  t1 = uthread_create(count, &mut);
  t2 = uthread_create(count, &mut);
  uthread_join (t0, NULL);
  uthread_join (t1, NULL);
  uthread_join (t2, NULL);
  printf("cnt = %d %s\n", cnt, cnt==0? "": "XXX");
}