#include <thread>
#include <mutex>
#include <iostream>
#include <cstddef>
#include <array>
#include <condition_variable>

static constexpr size_t kIterations = 1000000;
static constexpr size_t kThreads = 3;

void thread_func(size_t thread_id, std::mutex& mutex, std::array<double, kThreads>& result) {
    double base_value = 80.0 + (static_cast<double>(thread_id) * 20.0);
    size_t first_index = thread_id;
    size_t second_index = (thread_id + 1) % kThreads;
    for (size_t i = 0; i < kIterations; i++) {
        std::lock_guard guard{mutex};
        result[first_index] += base_value;
        result[second_index] -= base_value + 10;
    }
}

int main() {
    std::mutex mutex;
    std::array<double, kThreads> result;

    std::array threads1{
        std::thread{&thread_func, 0, std::ref(mutex), std::ref(result)},
        std::thread{&thread_func, 1, std::ref(mutex), std::ref(result)},
        std::thread{&thread_func, 2, std::ref(mutex), std::ref(result)},
    };
    std::array<std::thread, kThreads> threads = std::move(threads1);

    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    return 0;
}
