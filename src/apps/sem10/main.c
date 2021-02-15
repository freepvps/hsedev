#include "process.h"
#include <stdio.h>

int main() {
    enum { N = 5 };
    float a[N] = {1.0, 2.0, 3.0, 2.0, 4.0};
    float b[N] = {2.0, 1.0, 3.0, 4.0, 2.0};
    float ans = dot_product(N, a, b);
    printf("%.10g (ans=%.10g)\n", ans, 29.0f);
    // 128 = 16
    return 0;
}