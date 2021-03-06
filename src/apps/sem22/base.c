#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* thread_func(void* v) {
    int* x = (int*)v;
    *x = 12345;
    return v;
}

int main(int argc, char** argv) {
    pthread_t th;
    int x = 0;
    int res = pthread_create(&th, NULL, &thread_func, &x);
    if (res != 0) {
        perror("pthraed_create");
        return -1;
    }

    int* y;
    pthread_join(th, (void**)&y);

    printf("%d %d\n", x, (&x) == y);

    return 0;
}
