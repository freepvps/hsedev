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

    //    int getaddrinfo(const char *restrict node,
    //                    const char *restrict service,
    //                    const struct addrinfo *restrict hints,
    //                    struct addrinfo **restrict res);

void dump_addr_info(struct addrinfo* addr) {
    while (addr != NULL) {
        void* sock_addr = (void*)addr->ai_addr;
        sa_family_t family = *(sa_family_t*)sock_addr;

        if (family == AF_INET) {
            char buf[INET_ADDRSTRLEN + 1];
            struct sockaddr_in* addr_v4 = (struct sockaddr_in*)sock_addr;
            // ipv4/ipv6 address
            // ip address + port
            inet_ntop(AF_INET, &addr_v4->sin_addr, buf, INET_ADDRSTRLEN);

            uint32_t ip_value = addr_v4->sin_addr.s_addr;

            printf("%s:%d\n", buf, ntohs(addr_v4->sin_port));
        } else if (family == AF_INET6) {
            char buf[INET6_ADDRSTRLEN + 1];
            struct sockaddr_in6* addr_6 = (struct sockaddr_in6*)sock_addr;
            inet_ntop(AF_INET6, &addr_6->sin6_addr, buf, INET6_ADDRSTRLEN);
            printf("%s:%d\n", buf, ntohs(addr_6->sin6_port));
        }
        addr = addr->ai_next;
    }
}

int ensure_send(int fd, const char* s) {
    size_t s_len = strlen(s);
    while (s_len != 0) {
        ssize_t res = send(fd, s, s_len, 0);
        if (res == -1) {
            return -1;
        } else {
            s_len -= (size_t)res;
            s += res;
        }
    }
    return 0;
}

int main() {
    struct addrinfo* addr;
    struct addrinfo filter;
    memset(&filter, 0, sizeof(filter));
    filter.ai_socktype = SOCK_STREAM;
    filter.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo("google.com", "80", &filter, &addr) == -1) {
        perror("getaddrinfo");
        return -1;
    }

    dump_addr_info(addr);

    int fd = socket(addr->ai_family, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        perror("socket");
        return -1;
    }
    
    if (connect(fd, addr->ai_addr, addr->ai_addrlen) == -1) {
        perror("connect");
    }
    freeaddrinfo(addr);

    int err = (
        ensure_send(fd, "GET / HTTP/1.1\n") == -1
        || ensure_send(fd, "Host: ya.ru\n") == -1
        || ensure_send(fd, "User-Agent: curl/7.68.0\n") == -1
        || ensure_send(fd, "Accept: */*\n") == -1
        || ensure_send(fd, "\n") == -1
    ) ? -1 : 0;

    if (err == -1) {
        perror("send");
        return -1;
    }

    char buffer[16];
    while (1) {
        ssize_t res = recv(fd, buffer, sizeof(buffer), 0);
        if (res == -1) {
            perror("recv");
            break;
        } else if (res == 0) {
            break;
        }
        fwrite(buffer, (size_t)res, 1, stdout);
    }

    close(fd);

    return 0;
}
