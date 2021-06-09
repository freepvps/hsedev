#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int bind_cpu(int cpu_id) {
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(cpu_id, &cpu_set);

    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set);
}

void* thread_func(void* thread_arg) {
    bind_cpu(15);
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
    int res = 0;

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], NULL, &thread_func, &res);
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d\n", res);

    return 0;
}
