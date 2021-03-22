#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

enum {
    SIZE = 1024 * 1024,
};

int main() {
    int fd = open("test.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    void* m = mmap(NULL, 11, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (m == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    char* c = (char*)m;
    for (size_t i = 0; i < 11; i++) {
        printf("%c\n", c[i]);
    }

    return 0;
}