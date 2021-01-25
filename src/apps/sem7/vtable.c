#include <stdint.h>
#include <stdlib.h>

// Base class

struct RandomGenerator;

typedef struct {
    int (*next)(struct RandomGenerator* generator);
    void (*destroy)(struct RandomGenerator* generator);
} RandomOperations;

typedef struct RandomGenerator {
    const RandomOperations* ops;
} RandomGenerator;

// Verison 1
struct RandomVersion1Generator {
    struct RandomGenerator base;
    int current_state;
};

static int random_version1_next(struct RandomGenerator* generator) {
    struct RandomVersion1Generator* v = (struct RandomVersion1Generator*)generator;
    v->current_state++;
    return v->current_state;
}

static void random_version1_destroy(struct RandomGenerator* generator) {
    free(generator);
}

const RandomOperations random_version1_vtable = {
    .next = &random_version1_next,
    .destroy = &random_version1_destroy,
};

RandomGenerator* random_create(int seed) {
    struct RandomVersion1Generator* res = (struct RandomVersion1Generator*)malloc(sizeof(*res));
    if (res != NULL) {
        res->base.ops = &random_version1_vtable;
        res->current_state = seed;
    }
    return (RandomGenerator*)res;
}


// Verison 2
struct RandomVersion2Generator {
    struct RandomGenerator base;
    int current_state;
};

static int random_version2_next(struct RandomGenerator* generator) {
    struct RandomVersion2Generator* v = (struct RandomVersion2Generator*)generator;
    v->current_state += 2;
    return v->current_state;
}

static void random_version2_destroy(struct RandomGenerator* generator) {
    free(generator);
}

const RandomOperations random_version2_vtable = {
    .next = &random_version2_next,
    .destroy = &random_version2_destroy,
};

RandomGenerator* random2_create(int seed) {
    struct RandomVersion2Generator* res = (struct RandomVersion2Generator*)malloc(sizeof(*res));
    if (res != NULL) {
        res->base.ops = &random_version2_vtable;
        res->current_state = seed;
    }
    return (RandomGenerator*)res;
}

int main() {
    RandomGenerator* r1 = random_create(123);
    RandomGenerator* r2 = random2_create(123);
}

