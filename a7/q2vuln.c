struct Str {
    int size;
    char* arr;
};

struct Str s1, s2, s3;

void init() {
    s1.size = 30;
    s1.arr = "Welcome! Please enter a name:\n";
    s2.size = 11;
    s2.arr = "Good luck,";
    s3.size = 43;
    s3.arr = "The secret phrase is \"squeamish ossifrage\"\n";
}

void print(struct Str s) {
    write(1,s.arr,s.size);
}

int main() {
    init();
    print(s1);
    print(s2);
    return 0;
}

