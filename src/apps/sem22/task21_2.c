#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* thread_func(void* thread_arg) {
    int x;
    if (scanf("%d", &x) != 1) {
        return NULL;
    }
    pthread_t th;
    pthread_create(&th, NULL, &thread_func, NULL);
    pthread_join(th, NULL);
    printf("%d\n", x);
    return NULL;
}

int main(int argc, char** argv) {
    thread_func(NULL);

    return 0;
}
