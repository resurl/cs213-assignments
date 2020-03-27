void q2(int a, int b, int c) {
    int j;
    switch (a) {
        case 10:
            j = b + a;
            break;
        case 12:
            j = b - a;
            break;
        case 14:
            j = (b > a); // if array[1] > array[2] j = 1
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