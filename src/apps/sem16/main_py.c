#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

char* read_all(int fd, size_t* r_size) {
    size_t size = 0;
    size_t capacity = 16;
    char* buffer = malloc(capacity);

    char tmp[4096];
    while (1) {
        ssize_t r = read(fd, tmp, sizeof(tmp));
        if (r == -1 || r == 0) {
            break;
        }
        size_t required_capacity = r + size + 1;
        size_t next_capacity = capacity;
        while (next_capacity < required_capacity) {
            next_capacity *= 2;
        }
        if (next_capacity != capacity) {
            char* next_buf = realloc(buffer, next_capacity);
            if (next_buf == NULL) {
                break;
            }
            buffer = next_buf;
            capacity = next_capacity;
        }
        memcpy(buffer + size, tmp, (size_t)r);
        size += (size_t)r;
    }
    buffer[size] = '\0';
    if (r_size) {
        *r_size = size;
    }
    return buffer;
}

char* exec_python(const char* cmd) {
    char* argv[] = {
        "python",
        "-c",
        (char*)cmd,
        NULL,
    };
    int fd_pair[2];
    if (pipe2(fd_pair, 0) == -1) {
        return NULL;
    }

    pid_t pid = fork();
    if (pid == -1) {
        return NULL;
    }

    if (pid == 0) {
        if (fd_pair[1] != STDOUT_FILENO) {
            dup2(fd_pair[1], STDOUT_FILENO);
            close(fd_pair[1]);
        }
        close(fd_pair[0]);
        execvp("python3", argv);
        exit(-1);
    } else {
        close(fd_pair[1]);
        char* data = read_all(fd_pair[0], NULL);
        int status;
        pid_t r = waitpid(pid, &status, 0);
        int error_exit = 0;
        if (WIFEXITED(status)) {
            error_exit |= WEXITSTATUS(status) != 0;
        }
        if (WIFSIGNALED(status)) {
            error_exit = 1;
        }

        if (error_exit) {
            free(data);
            data = NULL;
        }
        return data;
    }
}

int main() {
    // signal(SIGCHLD, SIG_IGN);

    const char* cmd = (
        "import os\n"
        "import sys\n"
        "import time\n"
        "print(os.environ.get(\'PATH\'))\n"
        "time.sleep(0.1)\n"
        "print(sys.argv)\n"
    );
    char* res = exec_python(cmd);
    if (res == NULL) {
        printf("error\n");
    } else {
        printf("%s\n", res);
    }
    return 0;
}
