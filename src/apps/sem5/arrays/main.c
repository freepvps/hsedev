#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <immintrin.h>


#pragma pack(push, 1)
typedef struct {
    // 0xXXXXX0, 0xXXXXX8

    char ch1;  // 0xXXXX00-0xXXXXX1
    char ch2;  // 0xXXXX01-0xXXXX02
    uint32_t u32_1;  // 0xXXXX04-0xXXXX08
    uint64_t u64_2;  // 0xXXXX08-0xXXXX10
    char end[0];  // 0xXXXX18
} some_t;
#pragma pack(pop)

some_t arr[] = {
    {'x', 1, 't', 2},
    {'z', 2, 't', 2},
    {'u', 3, 't', 2},
    {'e', 4, 't', 2},
};


void magic() {
    union {
        char x[sizeof(uint64_t)];
        uint64_t v;
    } tmp;
    tmp.v = 0x0102030405060708ULL;
    size_t p = (size_t)&tmp;
    uint32_t* t = (uint32_t*)&tmp.x[1];
    uint32_t res;
    memcpy(&res, t, sizeof(res));

    printf("%x\n", res);
    printf("%d\n", (int)(p % sizeof(uint64_t)));
    printf("%d\n", (int)(((size_t)t) % sizeof(uint32_t)));
}

typedef char char_array_t[1024];
typedef struct {
    char_array_t c;
} array_holder_t;

void magic2(array_holder_t arg) {
    printf("%d\n", (int)sizeof(arg));
}

int main() {
    char_array_t c = "abc";
    array_holder_t h = {.c = c};
    magic2(h);
    return 0;
    // magic();
    // return 0;
    size_t s = sizeof(arr);

    some_t tmp;
    char* p1 = (char*)&tmp;
    char* p2 = &tmp.end[0];

    size_t delta = (size_t)(p2 - p1);
    printf("%d\n", (int)delta);
    printf("%d\n", (int)s);
    return 0;
}
