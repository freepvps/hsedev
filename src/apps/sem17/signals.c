#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/signal.h>
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

void on_sigpipe(int sig) {
    printf("receive %d signal, sigpipe=%d\n", sig, SIGPIPE);
}

void sigpipe_poc() {
    signal(SIGPIPE, &on_sigpipe);

    int pipe_fd[2];
    pipe(pipe_fd);
    pid_t child1 = exec_wrapper("cat", pipe_fd[0], STDOUT_FILENO);
    close(pipe_fd[0]);
    kill(child1, SIGKILL);
    waitpid(child1, NULL, 0);
    char c;
    write(pipe_fd[1], &c, sizeof(c));
    printf("alive\n");
    raise(SIGSEGV);
}

int main(int argc, char** argv) {
    int status = kill(getpid(), 0);
    printf("%d\n", status);
    return 0;
}
