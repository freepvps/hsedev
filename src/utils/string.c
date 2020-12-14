#include "string.h"
#include <errno.h>

void string_init(string_t* s) {
    if (s) {
        s->capacity = s->len = 0;
        s->data = NULL;
    }
}
void string_destruct(string_t* s) {
    if (s && s->data) {
        free(s->data);
        s->data = NULL;
        s->capacity = 0;
        s->len = 0;
    }
}

int string_reserve(string_t* s, size_t n) {
    if (!s) {
        errno = EBADF;
        return -1;
    }
    if (n <= s->capacity) {
        return -1;
    }
    void* memory = realloc(s->data, n + 1);
    if (memory == NULL) {
        return -1;
    }
    s->data = (char*)memory;
    s->capacity = n;
    return 0;
}

const char* string_c_str(string_t* s) {
    return s->data ? s->data : "";
}

int string_append_buffer(string_t* s, const void* src, size_t n) {
    if (!s) {
        errno = EBADF;
        return -1;
    }
    size_t required_capacity = s->len + n;
    if (required_capacity > s->capacity) {
        if (string_reserve(s, required_capacity << 1) == -1) {
            return -1;
        }
    }
    memcpy(&s->data[s->len], src, n);
    s->len += n;
    s->data[s->len] = '\0';
    return 0;
}
