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

int main(int argc, const char** argv) {
    if (argc < 3) {
        printf("%s host port\n", argv[0]);
        return -1;
    }
    const char* host = argv[1];
    int port;
    if (sscanf(argv[2], "%d", &port) != 1) {
        perror("parse port");
        return -1;
    }

    struct sockaddr_in in_addr;
    if (inet_pton(PF_INET, host, &in_addr.sin_addr) > 0) {
        in_addr.sin_port = htons(port);
        in_addr.sin_family = PF_INET;
    } else {
        perror("parse addr");
        return -1;
    }

    int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        perror("socket");
        return -1;
    }
    int option = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (bind(fd, (const struct sockaddr*)&in_addr, sizeof(in_addr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(fd, 1024) == -1) {
        perror("listen");
        return -1;
    }

    int client = accept(fd, NULL, 0);
    char buffer[16];
    while (1) {
        ssize_t res = recv(client, buffer, sizeof(buffer), 0);
        if (res <= 0) {
            close(client);
            break;
        }
        fwrite(buffer, (size_t)res, 1, stdout);
    }

    close(fd);

    return 0;
}
