#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

void sigusr1_handler(int sig) {
    int tmp;
    printf("sigusr1 received: %d %llu\n", sig, (unsigned long long)&tmp);
    raise(sig == SIGUSR2 ? SIGUSR1 : SIGUSR2);
    printf("post handler\n");
}

void signal_recursion_test() {
    signal(SIGUSR1, &sigusr1_handler);
    signal(SIGUSR2, &sigusr1_handler);
    while (1) {
        raise(SIGUSR1);
        int a = 1;
        int b = 2;
        int c = a + b;
        sleep(1);
    }
}

void empty_handler(int sig) {}

void sa_restart_test() {
    struct sigaction action;
    int status = sigaction(SIGUSR1, NULL, &action);
    if (action.sa_flags & SA_RESTART) {
        action.sa_flags ^= SA_RESTART;
    }
    action.sa_flags |= SA_RESTART;
    action.sa_handler = &empty_handler;
    sigaction(SIGUSR1, &action, NULL);
    char c;
    read(STDIN_FILENO, &c, sizeof(c));
    printf("read complete %s\n", strerror(errno));
}

void recursive_handler(int sig) {
    int tmp;
    printf("received signal: %d %llu\n", sig, (unsigned long long)&tmp);
    raise(sig);
}

void sa_recursive_test() {
    struct sigaction action;
    int status = sigaction(SIGUSR1, NULL, &action);
    action.sa_flags |= SA_NODEFER;
    action.sa_handler = &recursive_handler;

    sigaddset(&action.sa_mask, SIGUSR1);  // emulate ~SA_NODEFER

    sigaction(SIGUSR1, &action, NULL);
    raise(SIGUSR1);
}

int defer_counter = 0;
int defer_flag = 0;

void defer_handler(int sig) {
    static int sig_counter = 0;
    int current_id = sig_counter++;
    defer_counter++;
    if (defer_flag == 0) {
        int res = sleep(3);
        defer_flag = 1;
        printf("defer handler id=%d sig=%d error=%s\n", current_id, res, strerror(errno));
        raise(sig);
        raise(sig);
    }
}

void sa_defer_test() {
    struct sigaction action;
    int status = sigaction(SIGUSR1, NULL, &action);
    action.sa_flags |= SA_NODEFER;
    action.sa_handler = &defer_handler;

    sigaction(SIGUSR1, &action, NULL);
    // raise(SIGUSR1);
    while (1) {
        sleep(1);
    }
    printf("%d %d\n", defer_counter, defer_flag);
}

void sig_mask_test_handler(int sig) {
    printf("received signal: %d\n", sig);
}

void sig_mask_tests() {
    struct sigaction action;
    sigaction(SIGUSR1, NULL, &action);
    action.sa_handler = &sig_mask_test_handler;
    sigaction(SIGUSR1, &action, NULL);

    sigset_t set;
    sigfillset(&set);
    sigprocmask(SIG_SETMASK, &set, NULL);

    while (1) {
        sigemptyset(&set);
        sigpending(&set);
        printf("%d\n", sigismember(&set, SIGUSR1));
        sleep(1);

        if (sigismember(&set, SIGUSR1)) {
            sigemptyset(&set);
            sigprocmask(SIG_SETMASK, &set, NULL);
            break;
        }
    }

    sigfillset(&set);
    sigprocmask(SIG_SETMASK, &set, NULL);

    sigdelset(&set, SIGUSR1);
    sigsuspend(&set);
    printf("finish\n");
}

int main(int argc, char** argv) {
    struct sigaction action;
    int status = sigaction(SIGUSR1, NULL, &action);
    if (status == -1) {
        perror("sigaction error");
        return -1;
    }
    sig_mask_tests();
    return 0;
}
