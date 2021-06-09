#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int exists;
    uint64_t value;
} safe_queue_t;

int safe_queue_init(safe_queue_t* c) {
    if (pthread_mutex_init(&c->mutex, NULL) == -1) {
        return -1;
    }
    if (pthread_cond_init(&c->cond, NULL) == -1) {
        int err = errno;
        pthread_mutex_destroy(&c->mutex);
        errno = err;
        return -1;
    }
    c->exists = 0;
    c->value = 0;
    return 0;
}

void safe_queue_destroy(safe_queue_t* c) {
    if (c == NULL) {
        return;
    }
    pthread_mutex_destroy(&c->mutex);
    pthread_cond_destroy(&c->cond);
}

uint64_t safe_queue_acquire(safe_queue_t* c) {
    pthread_mutex_lock(&c->mutex);
    while (!c->exists) {
        pthread_cond_wait(&c->cond, &c->mutex);
    }
    c->exists = 0;
    uint64_t value = c->value;
    pthread_cond_signal(&c->cond);
    pthread_mutex_unlock(&c->mutex);
    return value;
}

void safe_queue_release(safe_queue_t* c, uint64_t value) {
    pthread_mutex_lock(&c->mutex);
    while (c->exists) {
        pthread_cond_wait(&c->cond, &c->mutex);
    }
    c->exists = 1;
    c->value = value;
    pthread_cond_signal(&c->cond);
    pthread_mutex_unlock(&c->mutex);
}

typedef struct {
    safe_queue_t queue;
    uint64_t base;
} context_t;

int is_prime(uint64_t x) {
    if (x < 2) {
        return 0;
    }
    if (x % 2 == 0 && x != 2) {
        return 0;
    }
    for (uint64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            return 0;
        }
    }
    return 1;
}

void* thread_func(void* arg) {
    context_t* ctx = (context_t*)arg;
    uint64_t value = ctx->base;
    while (1) {
        if (is_prime(value)) {
            safe_queue_release(&ctx->queue, value);
        }
        value += 1;
    }
    return NULL;
}

int main(int argc, char** argv) {
    uint64_t base;
    uint64_t count;
    scanf("%"SCNu64" %"SCNu64, &base, &count);

    context_t ctx;
    ctx.base = base;
    safe_queue_init(&ctx.queue);
    pthread_t thread;
    pthread_create(&thread, NULL, &thread_func, &ctx);

    for (uint64_t i = 0; i < count; i++) {
        uint64_t value = safe_queue_acquire(&ctx.queue);
        printf("%"PRIu64"\n", value);
    }

    pthread_cancel(thread);
    pthread_join(thread, NULL);

    return 0;
}
