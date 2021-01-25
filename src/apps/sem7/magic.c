#include "magic.h"
#include <stdlib.h>

static int get_value_const() { return 42; }
static int get_value_env() {
    return atoi(getenv("MAGIC"));
}

int get_value() {
    const char* env = getenv("MAGIC");
    if (env) {
        return get_value_env();
    } else {
        return get_value_const();
    }
}
