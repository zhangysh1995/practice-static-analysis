//
// Created by yzhanghw on 2020/4/8.
//
#include <cstdio>
#include <iostream>

bool compare(float lhs, float rhs) {
    return lhs > rhs;
}

using std::cin;

int main() {
    printf("this is only a test program");

    float a, b;

    cin >> a;
    cin >> b;

    compare(a, b);

    return 0;
}

