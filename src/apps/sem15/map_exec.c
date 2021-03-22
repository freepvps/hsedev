#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>


typedef int (*func_t)(int);

int some_func(int x) {
    return x * 2 + 1;
}

int main() {
    size_t page_size = (size_t)getpagesize();
    void* m = mmap(NULL, page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (m == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    uint32_t* from_f = (uint32_t*)some_func;
    uint32_t* to_f = (uint32_t*)m;

    uint32_t end = 0xd65f03c0;  // arm only

    while (1) {
        printf("%x\n", (unsigned)*from_f);

        int is_end = (*from_f) == end;
        *to_f = *from_f;
        from_f++;
        to_f++;
        if (is_end) {
            break;
        }
    }

    func_t f = (func_t)m;
    printf("%d\n", f(10));

    return 0;
}
