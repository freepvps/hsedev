#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/wait.h>

typedef struct {
    size_t begin;
    size_t end;
} shared_ctx_t;

void do_trigger_fd(int fd) {
    uint64_t value = 1;
    write(fd, &value, sizeof(value));
}

void work(int worker_id, int self_fd, int other_fd, shared_ctx_t* ctx) {
    while (1) {
        uint64_t value;
        if (read(self_fd, &value, sizeof(value)) != sizeof(value)) {
            break;
        }
        if (ctx->begin >= ctx->end) {
            break;
        }
        printf("%d %lld %lld\n", worker_id, (long long int)ctx->begin, (long long int)ctx->end);
        ctx->begin += 1;
        do_trigger_fd(other_fd);
    }
    do_trigger_fd(other_fd);
}

int main() {
    size_t page_size = (size_t)getpagesize();
    void* m = mmap(NULL, page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (m == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    
    shared_ctx_t* ctx = (shared_ctx_t*)m;
    ctx->begin = 0;
    ctx->end = 11;

    int fd1 = eventfd(0, 0);
    int fd2 = eventfd(0, 0);

    pid_t pid = fork();
    if (pid == 0) {
        work(2, fd2, fd1, ctx);
    } else {
        do_trigger_fd(fd1);
        work(1, fd1, fd2, ctx);
        wait(NULL);
    }

    return 0;
}
