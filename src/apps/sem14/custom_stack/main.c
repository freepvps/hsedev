#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

typedef void(*func_t)(int x1, int x2);

int main(int argc, char** argv) {
    void* lib = dlopen(argv[1], RTLD_LAZY);
    func_t run = (func_t)dlsym(lib, "run");

    union {
        double x;
        int y[2];
    } u;
    u.x = 1.5;
    run(u.y[0], u.y[1]);
    dlclose(lib);
    return 0;
}
