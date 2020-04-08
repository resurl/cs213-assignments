#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

uthread_t t0, t1, t2;
uthread_mutex_t mx;
uthread_cond_t oneCanGo, twoCanGo;
int count = 0;

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void* p0(void* v) {
  uthread_mutex_lock(*(uthread_mutex_t*)v);
  printf("zero\n");
  count++;
  uthread_cond_signal(oneCanGo);
  uthread_mutex_unlock(*(uthread_mutex_t*)v);
  return NULL;
}

void* p1(void* v) {
  uthread_mutex_lock(*(uthread_mutex_t*)v);
  while(count != 1)
  uthread_cond_wait(oneCanGo);
  printf("one\n");
  count++;
  uthread_cond_signal(twoCanGo);
  uthread_mutex_unlock(*(uthread_mutex_t*)v);
  return NULL;
}

void* p2(void* v) {
  uthread_mutex_lock(*(uthread_mutex_t*)v);
  while(count!=2)
  uthread_cond_wait(twoCanGo);
  printf("two\n");
  uthread_mutex_unlock(*(uthread_mutex_t*)v);
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  mx = uthread_mutex_create();
  oneCanGo = uthread_cond_create (mx);
  twoCanGo = uthread_cond_create (mx);
  t0 = uthread_create(p0, &mx);
  t1 = uthread_create(p1, &mx);
  t2 = uthread_create(p2, &mx);
  uthread_join (t0, NULL);
  uthread_join (t1, NULL);
  uthread_join (t2, NULL);
  printf("three\n");
  printf("------\n");
}