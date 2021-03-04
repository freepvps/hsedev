#define _POSIX_C_SOURCE 600
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>

ssize_t copy_file(int in_fd, int out_fd) {
    char buffer[4096];
    size_t total_read = 0;
    while (1) {
        ssize_t tmp = read(in_fd, buffer, sizeof(buffer));
        if (tmp == -1) {
            if (errno == EINTR) {
                printf("intr\n");
                continue;
            }
            return -1;
        }
        if (tmp == 0) {
            break;
        }
        total_read += (size_t)tmp;
        
        const char* current = buffer;
        size_t to_write = (size_t)tmp;

        while (to_write) {
            ssize_t w_res = write(out_fd, current, to_write);
            if (w_res == -1) {
                if (errno == EINTR) {
                    printf("intr\n");
                    continue;
                }
                return -1;
            }
            current += w_res;
            to_write -= (size_t)w_res;
        }
    }
    return (ssize_t)total_read;
}

void some_signal_handler(int sig) {
    printf("receive signal: %d\n", sig);
}

int main() {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_flags = 0;
    action.sa_handler = some_signal_handler;
    sigaction(SIGUSR1, &action, NULL);

    ssize_t res = copy_file(STDIN_FILENO, STDOUT_FILENO);
    if (res == -1) {
        perror("copy file error");
    }
    return 0;
}