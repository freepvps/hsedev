#pragma once

#define EPS 0.000001

static inline int float32_is_eps(float value) {
    return value > -EPS && value < EPS;
}

static inline int float32_eq(float x1, float x2) {
    return float32_is_eps(x1 - x2);
}

static inline int float32_gt(float x1, float x2) {
    return x1 - EPS > x2;
}

static inline int float32_ge(float x1, float x2) {
    return x1 + EPS > x2;
}
