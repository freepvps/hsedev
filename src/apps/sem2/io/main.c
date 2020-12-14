#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

void sig_handler(int sig) {
    while (1) {
        printf("%d signal", sig);
        sleep(1);
    }
}
void empty_sig_handler(int sig) {
    printf("%d\n", sig);
}

void generate_error() {
    int x;
    if (scanf("%d", &x) != 1) {
        perror("scanf error");
        return;
    }
    signal(SIGFPE, &sig_handler);
    int y = 10 / x;
    printf("%d\n", y);
}

int main() {
    FILE* f = fopen("./test.txt", "r");
    char format[16];
    size_t n = fread(format, 1, sizeof(format), f);
    format[n] = '\0';
    const char* s = format;
    int a = 12324234;
    int b = 123234234;
    size_t size = (size_t)snprintf(NULL, 0, s, a, b);
    char* buffer = malloc(size + 1);
    snprintf(buffer, size, s, a, b);

    printf("required size: %d\n", (int)size);
    printf("string: %s\n", buffer);
    printf("format: %s\n", format);
    fclose(f);

    return 0;
}
