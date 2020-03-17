#include <stdio.h>
int array[] = {10,10,10,0};

void q2(int a, int b, int c) {
    int j;

    switch (c) {
        case 10:
            j = b + a;
            break;
        case 12:
            j = b - a;
            break;
        case 14:
            j = (b > a);
            break;
        case 16:
            j = (a > b);
            break;
        case 18:
            j = (b == a);
            break;
        default:
            j = 0;
    }
    array[3] = j;
}

int main() {
    q2(array[2], array[1], array[0]);
    printf("%d",array[3]);
    return 0;
} 