#include <stdio.h>
int x[8] = {1,2,3,-1,-2,0,184,340057058};
int y[8] = {0,0,0,0,0,0,0,0};

int f(int n);

void main() {
    for (int i = 7; i >= 0; i--) 
        y[i] = f(x[i]);

    for (int i = 0; i < 8; i++) 
        printf("%d\n", x[i]);
    
    for (int i = 0; i < 8; i++) 
        printf("%d\n", y[i]);
}

int f(int n) {
    int j = 0;
    while (n != 0) {    
        if ((n & 0x80000000) != 0) {
            j++;
        }
        n = n << 1;
    }
    return j;
}