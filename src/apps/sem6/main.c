#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

size_t get_size(const char* s) {
    size_t ans = 0;
    while (*s) {

        if ((((uint8_t)*s) >> 6) == 2) {
            s++;
            continue;
        }
        s++;
    }
    return ans;
}

int main(int argc, char** argv) {
    int* ptr = &argc;
    auto x = *(ptr + 1);  // mov
    auto y = ptr + 1;  // lea
}