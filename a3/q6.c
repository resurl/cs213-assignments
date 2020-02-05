#include <stdio.h>

int a[10] = {0,1,1,2,3,5,8,13,21,34};
int main();

int main() {
    int answer = *(a+3);
    printf("%d\n", answer);
    return 0;
}