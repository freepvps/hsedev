#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

int exec_wrapper(const char* cmd) {
    pid_t pid = fork();
    if (pid == -1) {
        exit(-1);
    }
    if (pid == 0) {
        execlp(cmd, cmd, NULL);
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            return 1024 + WTERMSIG(status);
        } else {
            return -1;
        }
    }
}


int main(int argc, char** argv) {
    printf("res: %d\n", exec_wrapper("test"));
    return -1;
    return (
        (
            exec_wrapper(argv[1]) == 0
            || exec_wrapper(argv[2]) == 0
        )
        && exec_wrapper(argv[3]) == 0
    ) ? 0 : 1;
}
