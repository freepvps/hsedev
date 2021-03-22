#include <dlfcn.h>
#include <stdio.h>

void _init() {
    printf("init\n");
}

void _fini() {
    printf("fini\n");
}

void run() {
    printf("run 2\n");
}
