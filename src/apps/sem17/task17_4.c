#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <stdio.h>


enum {
    CHILD1 = 1,
    CHILD2 = 2,
};


void ping_pong(int* pipe_me, int* pipe_other, int child_no, int max_n) {
    int in_fd = pipe_other[0];
    int out_fd = pipe_me[1];
    close(pipe_other[1]);
    close(pipe_me[0]);

    FILE* in_file = fdopen(in_fd, "r");
    FILE* out_file = fdopen(out_fd, "w");

    if (child_no == CHILD2) {
        fprintf(out_file, "%d\n", 1);
        fflush(out_file);
    }

    while (1) {
        int n;
        if (fscanf(in_file, "%d", &n) == 1) {
            exit(0);
        }
        printf("%d %d\n", child_no, n);
        fflush(stdout);
        if (n == max_n) {
            exit(0);
        } else {
            fprintf(out_file, "%d\n", n + 1);
            fflush(out_file);
        }
    }
}


int main(int argc, char** argv) {
    int pipe_1to2[2];
    int pipe_2to1[2];
    pipe(pipe_1to2);
    pipe(pipe_2to1);

    int max_n = atoi(argv[1]);
    if (max_n < 1) {
        return -1;
    }

    pid_t child1 = fork();
    if (child1 == 0) {
        ping_pong(pipe_1to2, pipe_2to1, CHILD1, max_n);
    }
    pid_t child2 = fork();
    if (child2 == 0) {
        ping_pong(pipe_2to1, pipe_1to2, CHILD2, max_n);
    }
    for (size_t i = 0; i < 2; i++) {
        close(pipe_1to2[i]);
        close(pipe_2to1[i]);
    }

    waitpid(child2, NULL, 0);
    waitpid(child1, NULL, 0);
    printf("Done!\n");

    return 0;
}
