#include <thread>
#include <mutex>
#include <iostream>
#include <cstddef>
#include <array>
#include <future>
#include <condition_variable>

int main() {
    std::promise<int> x;
    std::future<int> res = x.get_future();
    x.set_value(1);
    res.wait();

    std::cout << res.get() << std::endl;
    return 0;
}
