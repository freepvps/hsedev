#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <utils/floating.h>


typedef union {
    float value;
    struct {

#ifdef BIGENDIAN
        uint32_t sign : 1;
        uint32_t exp : 8;
        uint32_t mantisa : 23;
#else
        uint32_t mantisa : 23;
        uint32_t exp : 8;
        uint32_t sign : 1;
#endif
    } representation;
} float_repr_t;


void print_repr(float x) {
    float_repr_t v;
    v.value = -x;
    printf("value: %f\n", x);
    printf("sign: %" PRIu32 "\n", v.representation.sign);
    printf("exp: %" PRIu32 "\n", v.representation.exp);
    printf("mantisa: %.10" PRIu32 "\n", v.representation.mantisa);
}

void junk() {
    // x87 soprocessor - register stack
    // SSE - 8x128 registers
    // AVX256 - 16x256 registers
    // AVX512 - 32x512 registers

    float_repr_t v;
    v.value = -3;
    printf("sign: %" PRIu32 "\n", v.representation.sign);
    printf("exp: %" PRIu32 "\n", v.representation.exp);
    printf("mantisa: %.10" PRIu32 "\n", v.representation.mantisa);
    v.representation.mantisa = ~0;
    printf("value: %10.3f\n", v.value);
    printf("string: %5.3s\n", "abcxyz");

    printf("float: %d\n", (int)sizeof(float));
    printf("double: %d\n", (int)sizeof(double));
    printf("long double: %d\n", (int)sizeof(long double));

    float x = 1;
    for (size_t i = 0; i < 1000; i++) {
        x *= 3;
        printf("%.5f\n", x);
    }

    v.value = NAN;
    printf("sign: %" PRIu32 "\n", v.representation.sign);
    printf("exp: %" PRIu32 "\n", v.representation.exp);
    printf("mantisa: %.10" PRIu32 "\n", v.representation.mantisa);

    long double long_x = 0.0000000000000000010;
    printf("value: %Lg\n", long_x);
}

int main() {
    float x1 = 0.23;
    float tmp;
    scanf("%f", &tmp);
    float x2 = acosf(cosf(tmp + 0.15));
    print_repr(x1);
    print_repr(x2);
    printf("%d %d\n", x1 == x2, float32_eq(x1, x2));
    return 0;
}
