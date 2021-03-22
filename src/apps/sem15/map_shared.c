#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

enum {
    COUNT = 16,
};

typedef struct {
    int x;
} some_struct_t;

int main() {
    size_t page_size = (size_t)getpagesize();
    void* m = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (m == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    some_struct_t* s = (some_struct_t*)m;
    if (pid == 0) {
        // is child
        s->x = 0;
        for (size_t i = 0; i < COUNT; i++) {
            s->x++;
            sleep(1);
        }
    } else {
        // is parent
        // is child
        for (size_t i = 0; i < COUNT; i++) {
            sleep(1);
            int x = s->x;
            printf("%d\n", x);
        }
    }
    munmap(m, page_size);

    return 0;
}
