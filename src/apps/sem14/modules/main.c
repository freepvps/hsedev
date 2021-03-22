#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

int load_lib(int argc, char** argv) {
    if (argc == 1) {
        perror("invalid args");
        return -1;
    }
    printf("before dlopen\n");
    void* lib = dlopen(argv[1], RTLD_LAZY);
    printf("after dlopen\n");
    if (lib == NULL) {
        perror("dlopen");
        return -1;
    }
    printf("before dlsym\n");
    void(*run)() = (void(*)())dlsym(lib, "run");
    printf("after dlsym\n");
    if (lib == NULL) {
        perror("dlsym");
        return -1;
    }
    run();
    char c;
    scanf("%c", &c);
    // printf("before dlclose\n");
    // dlclose(lib);
    // printf("before dlclose\n");
}

int main(int argc, char** argv) {
    load_lib(argc, argv);
    load_lib(argc, argv);
    return 0;
}
