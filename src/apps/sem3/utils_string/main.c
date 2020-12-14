#include <stdio.h>
#include <utils/string.h>


int main() {
    string_t s;
    string_init(&s);
    string_append(&s, "abc");
    string_append(&s, "xyz");
    printf("%s\n", string_c_str(&s));
    string_destruct(&s);
    return 0;
}
