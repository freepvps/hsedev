#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

struct AtomicList {
    struct AtomicList* _Atomic next;
    void* payload;
};

typedef struct AtomicList atomic_list_t;


void atomic_push(atomic_list_t* head, atomic_list_t* node) {
    while (1) {
        atomic_list_t* _Atomic current_next = __atomic_load_n(&head->next, __ATOMIC_SEQ_CST);
        node->next = current_next;
        if (__atomic_compare_exchange(&head->next, &current_next, &node, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
            break;
        }
    }
}


int main() {
    atomic_list_t x = {NULL, NULL};
    atomic_list_t y = {NULL, NULL};
    atomic_push(&x, &y);

    printf("%d\n", x.next == &y);

    return 0;
}
