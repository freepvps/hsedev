#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


uint32_t fixed_mul(uint32_t a, uint32_t b, int n) {
    if (n == 32) {
        // a = x1 + y1 / t  // y1 < t
        // b = x2 + y2 / t  // y2 < t
        // c = x1 * x2 + x1 * y2 / t + x2 * y1 / t + (y1 * y2) / (t * t)
        // c * (t * t) = x1 * x2 * t * t + x1 * y2 * t  + x2 * y1 * t + y1 * y2

        return (uint32_t)((((uint64_t)a) * ((uint64_t)b)) >> 32);
    } else {
        uint64_t x = a;
        uint64_t y = b;
        uint64_t t = (uint64_t)(x * y);
        uint64_t r = (1 << (n - 1)) - (((t >> n) & 1) ^ 1);
        uint64_t ans = (t + r) >> n;
        if (ans > UINT32_MAX) {
            return UINT32_MAX;
        } else {
            return ans;
        }
    }
    return a >> n;
}

int main() {
    printf(
        "%u %u %u\n",
        fixed_mul(3, 3, 1) == 0x04,
        fixed_mul(0x26, 0x42, 4) == 0x9d,
        fixed_mul(0x28, 0x41, 4) == 0xa2
    );
}
