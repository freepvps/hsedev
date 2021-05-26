#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/poll.h>

int wait_select(int fd) {
    fd_set r, w, e;
    struct timeval timeout = {1, 0};

    FD_ZERO(&r);
    FD_ZERO(&w);
    FD_ZERO(&e);
    FD_SET(fd, &r);
    FD_SET(fd, &e);
    if (FD_ISSET(0, &w)) {
        printf("out of range\n");
    }
    if (select(fd + 1, &r, &w, &e, &timeout) == -1) {
        return -1;
    }
    if (FD_ISSET(fd, &r)) {
        return 0;
    } else {
        return -1;
    }
}

int wait_poll(int fd) {
    struct pollfd poll_fd;
    poll_fd.fd = fd;
    poll_fd.events = POLLIN;
    poll_fd.revents = 0;

    int res = poll(&poll_fd, 1, 1000);
    if (res == 1) {
        return (poll_fd.revents & POLLIN) ? 0 : -1;
    } else {
        return -1;
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s host port\n", argv[0]);
        return -1;
    }
    struct addrinfo* addr;
    if (getaddrinfo(argv[1], argv[2], NULL, &addr) == -1) {
        perror("getaddrinfo");
        return -1;
    }

    for (size_t i = 0; i < 510; i++) {
        int pipe_fd[2];
        pipe(pipe_fd);
    }

    int fd = socket(addr->ai_family, SOCK_STREAM, IPPROTO_TCP);
    printf("fd: %d\n", fd);
    if (fd == -1) {
        perror("socket");
        return -1;
    }
    
    if (connect(fd, addr->ai_addr, addr->ai_addrlen) == -1) {
        perror("connect");
        return -1;
    }
    freeaddrinfo(addr);

    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("fcntl get flags");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl set flags");
        return -1;
    }
    printf("flags: %d\n", flags);

    time_t last_print = time(NULL);
    while (1) {
        char buf[16];
        wait_poll(fd);
        ssize_t recv_size = recv(fd, buf, sizeof(buf), 0);

        if (recv_size == 0) {
            break;
        } else if (recv_size == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("recv");
                break;
            }
        }

        time_t now = time(NULL);
        if (recv_size > 0) {
            fwrite(buf, (size_t)recv_size, 1, stdout);
            fflush(stdout);
            last_print = now;
        } else if (now - last_print >= 5) {
            printf("no data\n");
            last_print = now;
        }
    }

    close(fd);

    return 0;
}
