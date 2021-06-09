#include <thread>
#include <mutex>
#include <iostream>
#include <cstddef>
#include <array>
#include <future>
#include <condition_variable>

int main() {
    return std::async(std::launch::async, [] { return 1; }).get();
}
