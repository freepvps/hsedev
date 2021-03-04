#include <asm/unistd_32.h>
#include <stddef.h>
#include <unistd.h>

#define DEF_IMPL_STR(x) #x
#define DEF_STR(x) DEF_IMPL_STR(x)

struct FileReadState {
    int fd;
    char *buf;
    int bufsize;
    int lc;

    int data_size;
    int data_offset;

    int eof;
};

enum { BUFFER_SIZE = 4096 };

static char stdin_buffer[BUFFER_SIZE];
static struct FileReadState stin_static = {
    .fd = STDIN_FILENO,
    .buf = stdin_buffer,
    .bufsize = BUFFER_SIZE,
    .lc = -1,

    .data_size = 0,
    .data_offset = 0,

    .eof = 0,
};
struct FileReadState* stin = &stin_static;



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

int lastchar(struct FileReadState* input) {
    return input->lc;
}

int nextchar(struct FileReadState* input) {
    if (input->eof) {
        return -1;
    }
    if (input->data_offset == input->data_size) {
        ssize_t r = call_read(input->fd, input->buf, input->bufsize);
        if (r <= 0) {
            input->eof = 1;
            return -1;
        }
        input->data_offset = 0;
        input->data_size = (int)r;
    }
    return (input->lc = input->buf[input->data_offset++]);
}

void _start() {
    int code;
    while ((code = nextchar(stin)) != -1) {
        char c = (char)code;
        if (c >= 'a' && c <= 'z') {
            c -= 'a';
            c += 'A';
        }
        call_write(STDOUT_FILENO, &c, sizeof(c));
    }
    uint32_t k;
    uint8_t high = (k >> 24) & 0xFF;
    call_exit(0);
}
