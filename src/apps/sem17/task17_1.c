#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

void move_fd(int from_fd, int to_fd) {
    if (from_fd != to_fd) {
        dup2(from_fd, to_fd);
        close(from_fd);
        return;
    }
}

pid_t exec_wrapper(const char* cmd, int in_file, int out_file) {
    pid_t pid = fork();
    if (pid == -1) {
        return -1;
    }
    if (pid == 0) {
        move_fd(in_file, STDIN_FILENO);
        move_fd(out_file, STDOUT_FILENO);
        execlp(cmd, cmd, NULL);
        exit(1);
    } else {
        return pid;
    }
}

int main(int argc, char** argv) {
    int pipe_fd[2];
    pipe(pipe_fd);
    pid_t child1 = exec_wrapper(argv[1], STDIN_FILENO, pipe_fd[1]);  // how to close pipe_fd[0] before exec?
    close(pipe_fd[1]);
    pid_t child2 = exec_wrapper(argv[2], pipe_fd[0], STDOUT_FILENO);
    close(pipe_fd[0]);
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);
    return 0;
}
