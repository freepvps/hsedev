#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

int main() {
    char* argv[] = {
        "python",
        "-h",
        NULL,
    };
    char* env[] = {
        "TEST=Y",
        NULL,
    };
    // execlpe();
    execvp("python3", argv);
    perror("exec error");
    return 0;
}
