#include <stdio.h>

int* a = (int[]) {10, 12, 14, 16, 18, 20};
int* i;
int main();

int main() { 
    i = (int[]) {10,11,12};
    int answer = (int) (__intptr_t) &i[0];
    printf("%d\n", answer);
    return 0;
}