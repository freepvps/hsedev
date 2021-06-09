#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

enum { THREAD_NUMS = 10 };

typedef struct {
    pthread_t prev_thread;
    int thread_id;
} thread_context_t;

void* thread_func(void* thread_arg) {
    thread_context_t* ctx = (thread_context_t*)thread_arg;
    if (ctx->prev_thread != -1) {
        pthread_join(ctx->prev_thread, NULL);
    }
    printf("%d\n", ctx->thread_id);
    return NULL;
}

int main(int argc, char** argv) {
    thread_context_t contexts[THREAD_NUMS];
    pthread_t last_thread = -1;

    for (size_t i = 0; i < THREAD_NUMS; i++) {
        contexts[i].prev_thread = last_thread;
        contexts[i].thread_id = (int)i;
        int res = pthread_create(&last_thread, NULL, &thread_func, &contexts[i]);
    }

    pthread_join(last_thread, NULL);

    return 0;
}
