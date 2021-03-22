#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main() {
    const char* s = "Hello, World!\n";
    // write(STDOUT_FILENO, s, strlen(s));
    printf("%s", s);
    return 0;
}
