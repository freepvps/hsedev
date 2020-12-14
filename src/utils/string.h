#include <stdlib.h>
#include <string.h>


typedef struct String {
    char* data;
    size_t len;
    size_t capacity;
} string_t;

void string_init(string_t* s);
void string_destruct(string_t* s);
int string_reserve(string_t* s, size_t n);
const char* string_c_str(string_t* s);
int string_append_buffer(string_t* s, const void* src, size_t n);
inline int string_append(string_t* s, const char* src) {
    return string_append_buffer(s, src, strlen(src));
}
inline int string_append_n(string_t* s, const char* src, size_t n) {
    return string_append_buffer(s, src, strnlen(src, n));
}
inline int string_append_string(string_t* s, const string_t* src) {
    return string_append_buffer(s, src->data, src->len);
}
