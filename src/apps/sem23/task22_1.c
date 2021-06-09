#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <stdint.h>

enum {
    THREADS_COUNT = 3,
    THREAD_ITERATIONS = 1000000,
};

typedef struct {
    int thread_id;
    double* values;
    pthread_mutex_t* mutex;
} context_t;

void* thread_func(void* thread_arg) {
    context_t* ctx = (context_t*)thread_arg;
    for (size_t i = 0; i < THREAD_ITERATIONS; i++) {
        int next = (ctx->thread_id + 1) % THREADS_COUNT;
        double x = (ctx->thread_id + 1) * 100;

        pthread_mutex_lock(ctx->mutex);

        ctx->values[ctx->thread_id] += x;
        ctx->values[next] -= x + 1;

        pthread_mutex_unlock(ctx->mutex);
    }
    return NULL;
}

int main(int argc, char** argv) {
    double values[THREADS_COUNT] = {0};
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    context_t contexts[THREADS_COUNT];
    pthread_t threads[THREADS_COUNT];

    for (size_t i = 0; i < THREADS_COUNT; i++) {
        contexts[i].mutex = &mutex;
        contexts[i].thread_id = (int)i;
        contexts[i].values = values;

        pthread_create(&threads[i], NULL, &thread_func, &contexts[i]);
    }
    for (size_t i = 0; i < THREADS_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    for (size_t i = 0; i < THREADS_COUNT; i++) {
        printf("%.10g\n", values[i]);
    }

    return 0;
}
