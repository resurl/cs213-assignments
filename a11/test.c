#include <stdio.h>
#include <stdlib.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

uthread_mutex_t mx;
uthread_cond_t cond;
int flag;

void* a(void* b) {
    uthread_mutex_lock(mx);
    if (flag==0)
        uthread_cond_wait(cond);
    printf("a\n");
    printf("b\n");
    uthread_mutex_unlock(mx);
    return NULL;
}

void* c(void* b) {
    uthread_mutex_lock(mx);
    flag = 1;
    uthread_cond_signal(cond);
    printf("c\n");
    printf("d\n");
    uthread_mutex_unlock(mx);
    return NULL;
}

int main() {
    uthread_init(4);
    mx = uthread_mutex_create();
    cond = uthread_cond_create(mx);
    flag = 0;

    uthread_t two = uthread_create(c, NULL);
    uthread_t one = uthread_create(a, NULL);

    uthread_join(one,NULL);
    uthread_join(two,NULL);
    printf("end\n");
    return 0;
}