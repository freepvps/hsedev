#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>

typedef ssize_t (*write_func_t)(int __fd, const void *__buf, size_t __n);

ssize_t write(int fd, const void *buf, size_t n) {
    static write_func_t f = NULL;
    if (f == NULL) {
        f = (write_func_t)dlsym(RTLD_NEXT, "write");
    }
    f(STDERR_FILENO, "test\n", 5);
    return f(fd, buf, n);
}
