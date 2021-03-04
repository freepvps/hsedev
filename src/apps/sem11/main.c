#include <asm/unistd_32.h>
#include <stddef.h>
#include <unistd.h>

#define DEF_IMPL_STR(x) #x
#define DEF_STR(x) DEF_IMPL_STR(x)

size_t str_len(const char* s) {
    if (!s) {
        return 0;
    }
    size_t n = 0;
    while (s[n]) {
        n++;
    }
    return n;
}

ssize_t call_write(int fd, const char* buf, size_t n) {
    if (!buf) {
        buf = "";
    }
    ssize_t r;
    asm volatile (
        "movl $"DEF_STR(__NR_write)", %%eax\n\t"
        "movl %1, %%ebx\n\t"
        "movl %2, %%ecx\n\t"
        "movl %3, %%edx\n\t"
        "int $0x80\n\t"
        "movl %%eax, %[RES]\n\t"
        : [RES]"=g"(r)
        : "g"(fd), "g"(buf), "g"(n)
        : "%eax", "%ebx", "%ecx", "%edx"
    );
    return r;
}

ssize_t call_read(int fd, char* buf, size_t n) {
    if (!buf) {
        buf = "";
    }
    ssize_t r;
    asm volatile (
        "movl $"DEF_STR(__NR_read)", %%eax\n\t"
        "movl %1, %%ebx\n\t"
        "movl %2, %%ecx\n\t"
        "movl %3, %%edx\n\t"
        "int $0x80\n\t"
        "movl %%eax, %[RES]\n\t"
        : [RES]"=g"(r)
        : "g"(fd), "g"(buf), "g"(n)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    return r;
}

void write_string(const char* s) {
    call_write(STDOUT_FILENO, s, str_len(s));
    call_write(STDOUT_FILENO, "\n", 1);
}

void call_exit(int code) {
    asm volatile (
        "movl $"DEF_STR(__NR_exit)", %%eax\n\t"
        "movl %0, %%ebx\n\t"
        "int $0x80\n\t"
        :
        : "g"(code)
        : "%eax", "%ebx"
    );
}

// void _start(char* arg0, char* arg1, char* arg2) {
//     write_string(arg0);
//     write_string(arg1);
//     write_string(arg2);
//     call_exit(0);
// }
