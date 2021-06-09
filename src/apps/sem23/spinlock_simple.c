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
    pthread_spinlock_t spinlock;
} context_t;

void* thread_func(void* thread_arg) {
    context_t* ctx = (context_t*)thread_arg;
    for (size_t i = 0; i < THREAD_ITERATIONS; i++) {
        int tmp = (int)i;
        pthread_spin_lock(&ctx->spinlock);
        ctx->value += tmp % ctx->magic;
        pthread_spin_unlock(&ctx->spinlock);
    }
    return NULL;
}

int main(int argc, char** argv) {
    context_t ctx;
    ctx.value = 0;
    ctx.magic = argc > 1 ? atoi(argv[1]) : 17;
    pthread_spin_init(&ctx.spinlock, 1);

    pthread_t threads[THREADS_COUNT];
    for (size_t i = 0; i < THREADS_COUNT; i++) {
        pthread_create(&threads[i], NULL, &thread_func, &ctx);
    }
    for (size_t i = 0; i < THREADS_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_spin_destroy(&ctx.spinlock);

    printf("%"PRId64"\n", ctx.value);

    return 0;
}
