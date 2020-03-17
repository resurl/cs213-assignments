for (int i = 0; i < n; i++) {
    int grades = s[i].grade[0] + s[i].grade[1] + s[i].grade[2] + s[i].grade[3];
    s[i].average = grade >> 2;
}


for (int i = n-1; i > 0; i--) {
    for (int j = 1; j <=i; j++) {
            if (s[j-1].average > s[j].average) {
                struct S t = s[j];
                t.sid = s[j-1].sid
                t.grade = s[j-1].grade
                t.average = s[j-1].average
                s[j-1] = s[j];
                s[j-1] = t;
        }
    }
}


int middle = (n + 1) / 2;
m = s[middle].sid;


