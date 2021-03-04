#include <asm/unistd_32.h>
#include <stddef.h>
#include <unistd.h>

#define DEF_IMPL_STR(x) #x
#define DEF_STR(x) DEF_IMPL_STR(x)

int my_write(int fd, const char* buf, size_t n);
asm (
    ".global my_write\n\t"
"my_write:\n\t"
    "pushl %ebx\n\t"
    "pushl %ecx\n\t"
    "pushl %edx\n\t"
    "pushl %ebp\n\t"
    "mov %esp, %ebp\n\t"
    "andl $-16, %esp\n\t"
    "movl $"DEF_STR(__NR_write)", %eax\n\t"
    "movl 20(%ebp), %ebx\n\t"
    "movl 24(%ebp), %ecx\n\t"
    "movl 28(%ebp), %edx\n\t"
    "int $0x80\n\t"
    "mov %ebp, %esp\n\t"
    "popl %ebp\n\t"
    "popl %edx\n\t"
    "popl %ecx\n\t"
    "popl %ebx\n\t"
    "ret\n\t"
);

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

void _start() {
    char c;
    while (call_read(STDIN_FILENO, &c, sizeof(c)) == 1) {
        if (c >= 'a' && c <= 'z') {
            c -= 'a';
            c += 'A';
        }
        my_write(STDOUT_FILENO, &c, sizeof(c));
    }
    call_exit(0);
}
