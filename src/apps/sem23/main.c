#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <x86intrin.h>

struct State {
    size_t x;
    void (*mutate)(size_t* x);
    pthread_mutex_t mutex;
    pthread_spinlock_t spin_lock;
};

void mutate(size_t* x) {
    (*x)++;
}

size_t rdtscp() {
    int tmp;
    return (size_t)__rdtscp(&tmp);
}

void* thread_func(void* input) {
    struct State* x = (struct State*)input;
    pthread_spin_lock(&x->spin_lock);
    for (size_t i = 0; i < 10000000; i++) {
        x->mutate(&x->x);
    }
    pthread_spin_unlock(&x->spin_lock);

    return NULL;
}

struct Container {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int exists;
    void* value;
};

void* container_acquire(struct Container* c) {
    pthread_mutex_lock(&c->mutex);
    while (!c->exists) {
        pthread_cond_wait(&c->cond, &c->mutex);
    }
    c->exists = 0;
    pthread_mutex_unlock(&c->mutex);
    return c->value;
}

void container_release(struct Container* c) {
    pthread_mutex_lock(&c->mutex);
    c->exists = 1;
    pthread_cond_signal(&c->cond);
    pthread_mutex_unlock(&c->mutex);
}

int main() {
    printf("main\n");
    pthread_t th1, th2;
    struct State s = {.x = 0, .mutate = &mutate};
    pthread_mutex_init(&s.mutex, NULL);
    pthread_spin_init(&s.spin_lock, 0);
    pthread_create(&th1, NULL, &thread_func, (void*)&s);
    pthread_create(&th2, NULL, &thread_func, (void*)&s);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("main %d\n", (int)s.x);

    return 0;
}
