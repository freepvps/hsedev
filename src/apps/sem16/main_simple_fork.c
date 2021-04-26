#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

int main() {
    void* x = malloc(16);
    uint64_t ptr = (uint64_t)x;
    printf("PTR: %"PRIu64"\n", ptr);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error");
        return -1;
    }
    int is_child = pid == 0;

    printf("IS_CHILD: %d PID: %d GROUP: %d PTR: %"PRIu64"\n", is_child, (int)getpid(), (int)getpgid(getpid()), ptr);
    return 0;
}