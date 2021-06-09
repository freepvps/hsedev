#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <stdint.h>

enum {
    THREADS_COUNT = 10,
    THREAD_ITERATIONS = 1000000,
};

typedef struct {
    int64_t value;
    int magic;
    pthread_mutex_t mutex;
} context_t;

void* thread_func(void* thread_arg) {
    context_t* ctx = (context_t*)thread_arg;
    for (size_t i = 0; i < THREAD_ITERATIONS; i++) {
        int tmp = (int)i;
        pthread_mutex_lock(&ctx->mutex);
        ctx->value += tmp % ctx->magic;
        pthread_mutex_unlock(&ctx->mutex);
    }
    return NULL;
}

int main(int argc, char** argv) {
    context_t ctx;
    ctx.value = 0;
    ctx.magic = argc > 1 ? atoi(argv[1]) : 17;
    pthread_mutex_init(&ctx.mutex, NULL);

    pthread_t threads[THREADS_COUNT];
    for (size_t i = 0; i < THREADS_COUNT; i++) {
        pthread_create(&threads[i], NULL, &thread_func, &ctx);
    }
    for (size_t i = 0; i < THREADS_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&ctx.mutex);

    printf("%"PRId64"\n", ctx.value);

    return 0;
}
