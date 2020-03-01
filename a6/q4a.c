int *p;

void foo() {
    int a = 1;
    int b = 2;
    bar(3,4);
    bar(a,b);
}

void bar(int a, int b) {
    p[b] = a;
}