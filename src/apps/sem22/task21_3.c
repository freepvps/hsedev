#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* thread_func(void* thread_arg) {
    int* res = (int*)thread_arg;
    int x;
    while (scanf("%d", &x) == 1) {
        *res += x;
        sched_yield();
    }
    return NULL;
}

int main(int argc, char** argv) {
    int thread_count = atoi(argv[1]);
    pthread_t* threads = (pthread_t*)malloc(sizeof(*threads) * thread_count);
    int* sums = (int*)calloc(thread_count, sizeof(*sums));

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], NULL, &thread_func, &sums[i]);
    }
    int res = 0;
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
        printf("%d thread: %d\n", i, sums[i]);
        res += sums[i];
    }

    printf("%d\n", res);

    return 0;
}
