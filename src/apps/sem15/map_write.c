#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

enum {
    SIZE = 4096,
};

int main() {
    int fd = open("test2.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    printf("%d\n", getpagesize());
    void* m = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (m == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    char* c = (char*)m;
    memset(c, 'T', SIZE);
    munmap(m, SIZE);
    close(fd);

    return 0;
}