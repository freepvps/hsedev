#include "sample.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <limits.h>
#include <inttypes.h>
#include <openacc.h>

// [11][00][FF][00][00][00][00][00] - little endian
// [00][00][00][00][00][FF][00][11] - big endian
// uint64_t x = 0xFF0011;

enum {
    Positive = 10,
    Negative = -5,
    Zero = 11,
};

struct S {
    uint32_t a;
    uint64_t b;
};

int main() {
    int64_t a, b;
    int scan_res = scanf(
        "%" SCNd64
        "%" SCNd64,
        &a,
        &b
    );
    int64_t ans = a + b;
    int print_res = printf("%" PRId64 "\n", ans) / ((int)ans);
    printf(
        "%d\n",
        (
            ans < 0 ? Negative :
            ans > 0 ? Positive :
            Zero
        )
    );
    printf("%d\n", func123());
    return 0;
}
