int foo1() {
    int x, y, z, r;
    r = rand();
    x = 5;
    y = x / 2;

    for (; r < 200; ++r) {
        if (r > 100 && r < 180) {
            z = x - 1;
        } else {
            z = y - 1;
        }
    }
    return z;
}