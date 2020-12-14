#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int main() {
    char buffer[1024] = {0};
    char* dest = buffer;
    
    for (size_t i = 0; i < 4; i++) {
        char s[32];
        if (scanf("%32s", s) != 1) {
            perror("error");
            return -1;
        }
        size_t s_len = strnlen(s, sizeof(s));
        char* new_s = s;
        size_t new_s_len = s_len;
        while (new_s_len > 0 && *new_s == '[') {
            new_s++;
            new_s_len--;
        }
        while (new_s_len > 0 && new_s[new_s_len - 1] == ']') {
            new_s_len--;
        }
        strncpy(dest, new_s, new_s_len);
        dest += new_s_len;
    }
    printf("%s\n", buffer);

    return 0;
}