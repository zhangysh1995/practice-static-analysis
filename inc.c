//
// Created by yzhanghw on 2020/5/10.
//

#include "stdio.h"

int inc(int a) {
    return a+1;
}

int main() {
    // declarations
    int x, y, z, flag;

    x = 10;
    y = 0;
    z = x + y;

    // assume the value is int
    scanf("%d", &flag);

    // take care of the condition
    if (flag > 0) {
        x = inc(0);
        y = inc(-1);
    } else {
        x = inc(-5);
        y = inc(9);
    }

    flag = x + z;
    return 0;
}
