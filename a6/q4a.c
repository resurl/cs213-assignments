#include <stdio.h>
int p[10] = {0,0,0,0,0,0,0,0,0,0};

void foo();
void bar(int a, int b);

int main() {
    foo();
    return 0;
}

void foo() {
    int a = 1;
    int b = 2;
    bar(3,4);
    bar(a,b);
    for (int i = 0; i < 10; i++)
        printf("%d\n", p[i]);
}

void bar(int a, int b) {
    p[b] = a;
}