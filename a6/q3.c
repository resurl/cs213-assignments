int m;

for (int i = 0; i < n; i++) {
    int grades = s[i].grade[0] + s[i].grade[1] + s[i].grade[2] + s[i].grade[3];
    s[i].average = grade >> 2;
}

void sort (int* a, int n) {
    for (int i = n-1; i > 0; i--) {
        for (int j = 1; j <=i; j++) {
            if (a[j-1] > a[j]) {
                int t = a[i];
                a[j] = a[j-1];
                a[j-1] = t;
            }
        }
    }
}

int middle = (n + 1) / 2;
m = s[middle].sid;


