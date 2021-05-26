#define _GNU_SOURCE 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <sys/timerfd.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>


void handle(int sig) {
    printf("signal %d\n", sig);
}

void run_timer_signal() {
    signal(SIGALRM, &handle);
    struct itimerval s = {{1, 0}, {1, 0}};
    setitimer(ITIMER_REAL, &s, NULL);
    while (1) {
        sleep(1);
    }
}

void run_timefd() {
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    struct itimerspec s = {{0, 500000000}, {0, 500000000}};
    timerfd_settime(fd, 0, &s, NULL);

    sleep(2);

    while (1) {
        uint64_t x;
        ssize_t s = read(fd, (void*)&x, sizeof(x));
        if (s != sizeof(x)) {
            continue;
        }
        printf("%"PRIu64"\n", x);
    }
}

// int get_task();
// void execute_task(int x);

// void fork_queue(int semaphore) {
//     while (1) {
//         int task = get_task();
//         uint64_t value = 0;
//         while (value != 1) {
//             read(semaphore, &value, sizeof(value));
//         }
//         pid_t pid = fork();
//         if (pid == -1) {
//             exit(-1);
//         }
//         if (pid) {
//             continue;
//         }
//         execute_task(task);
//         write(semaphore, &value, sizeof(value));
//     }
// }

void run_semaphore() {
    int fd = eventfd(2, EFD_SEMAPHORE);
    uint64_t to_write = 3;
    if (write(fd, (void*)&to_write, sizeof(to_write)) != sizeof(to_write)) {
        printf("write error %s\n", strerror(errno));
        exit(-1);
    }
    for (size_t i = 0; i < 10; i++) {
        uint64_t value;
        ssize_t res = read(fd, (void*)&value, sizeof(value));
        if (res != sizeof(value)) {
            printf("read error %s\n", strerror(errno));
            exit(-1);
        }
        printf("%"PRIu64"\n", value);
    }
}

void run_poll() {
    int fd = eventfd(2, EFD_SEMAPHORE);
    struct pollfd req = {
        fd,
        POLLIN | POLLOUT | POLLERR,
        0,
    };
    while (1) {
        if (poll(&req, 1, 0) == -1) {
            continue;
        }
        printf(
            "flags r=%d w=%d e=%d\n",
            (req.revents & POLLIN) > 0,
            (req.revents & POLLOUT) > 0,
            (req.revents & POLLERR) > 0
        );
        if (req.revents & POLLIN) {
            uint64_t x;
            ssize_t res = read(fd, (void*)&x, sizeof(x));
            if (res != sizeof(x)) {
                exit(-1);
            }
            printf("%"PRIu64"\n", x);
        }
    }
}

void open_many_fd() {
    for (size_t i = 0; i < 1024; i++) {
        while (eventfd(2, EFD_SEMAPHORE) == -1) {
            printf("open many fd error %s\n", strerror(errno));
        }
    }
}

void run_select() {
    int fd = eventfd(2, EFD_SEMAPHORE);
    fd_set r, w, e;
    struct timeval timeout = {0, 0};
    while (1) {
        FD_ZERO(&r);
        FD_ZERO(&w);
        FD_ZERO(&e);
        FD_SET(fd, &r);
        FD_SET(fd, &w);
        FD_SET(fd, &e);
        if (select(fd + 1, &r, &w, &e, &timeout) == -1) {
            continue;
        }
        printf(
            "flags r=%d w=%d e=%d\n",
            FD_ISSET(fd, &r),
            FD_ISSET(fd, &w),
            FD_ISSET(fd, &e)
        );
        if (FD_ISSET(fd, &r)) {
            uint64_t x;
            ssize_t res = read(fd, (void*)&x, sizeof(x));
            if (res != sizeof(x)) {
                exit(-1);
            }
            printf("%"PRIu64"\n", x);
        }
    }
}

void run_epoll() {
    int fds[2];
    if (pipe2(fds, O_NONBLOCK) == -1) {
        printf("error %s\n", strerror(errno));
        exit(-1);
    }
    int fd = fds[0];

    int efd = epoll_create1(0);
    struct epoll_event ev;
    ev.data.u32 = 1234;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLET;

    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        printf("error %s\n", strerror(errno));
        exit(-1);
    }

    for (size_t i = 0; i < 4; i++) {
        uint64_t x = 123;
        if (write(fds[1], (void*)&x, sizeof(x)) != sizeof(x)) {
            printf("error %s\n", strerror(errno));
            exit(-1);
        }
    }

    int has_data = 0;
    while (1) {
        printf("step\n");
        if (has_data) {
            uint64_t x;
            ssize_t res = read(ev.data.fd, (void*)&x, sizeof(x));
            if (res == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    has_data = 0;
                } else {
                    printf("%s\n", strerror(errno));
                }
                continue;
            }
            if (res == 0) {
                printf("closed\n");
                break;
            }
            printf("%"PRIu64"\n", x);
            fflush(stdout);
            continue;
        }

        int res = epoll_wait(efd, &ev, 1, 1000);
        if (res <= 0) {
            printf("evres: %d\n", res);

            int x = 456;
            if (write(fds[1], (void*)&x, sizeof(x)) != sizeof(x)) {
                printf("error %s\n", strerror(errno));
                exit(-1);
            }
            continue;
        }

        printf(
            "flags r=%d w=%d e=%d\n",
            (ev.events & EPOLLIN) > 0,
            (ev.events & EPOLLOUT) > 0,
            (ev.events & EPOLLERR) > 0
        );
        if (ev.events & EPOLLIN) {
            has_data = 1;
        }
    }
}

int main(int argc, char** argv) {
    run_epoll();
    return 0;
}