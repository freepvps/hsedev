#include <math.h>
#include <stdlib.h>

typedef double (*funcptr_t)(double);

struct FunctionTable {
    const char* name;
    funcptr_t func;
};

static double magic_log10(double x) {
    return log(x) / log(10);
}

static const struct FunctionTable func_table[] = {
    {"sin", &sin},
    {"log10", &magic_log10},
};

int main() {
}
