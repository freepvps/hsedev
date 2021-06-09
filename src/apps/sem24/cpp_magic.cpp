#include <atomic>
#include <thread>
#include <mutex>
#include <iostream>
#include <cstddef>
#include <array>
#include <future>
#include <chrono>
#include <vector>
#include <condition_variable>

template<typename F, typename Callback, typename... Args>
void async_launch(F&& f, Callback&& cb, Args&&... args) {
    std::thread thread{
        [
            thread_f = std::forward<F>(f),
            thread_cb = std::forward<Callback>(cb),
            thread_args = std::tuple<Args...>(std::forward<Args>(args)...)
        ] {
            thread_cb(std::apply(thread_f, thread_args));
        }
    };
    thread.detach();
}

template<typename Function, typename Callback, typename Output>
struct AsyncVectorContext {
    Function function;
    Callback callback;

    std::vector<Output> output;
    std::atomic<size_t> complete_count;
    size_t required_count;

    explicit AsyncVectorContext(Function&& f, Callback&& cb, size_t input_size)
        : function(std::forward<Function>(f))
        , callback(std::forward<Callback>(cb))
        , output(input_size)
        , complete_count(0)
        , required_count(input_size) {
    }
};

template<class Input, class Function, class Callback>
void async_vector(const std::vector<Input>& ins, Function &&f, Callback &&c) {
    using Result = typename std::invoke_result_t<Function, Input>;
    using Context = AsyncVectorContext<Function, Callback, Result>;

    auto ctx = std::make_shared<Context>(
        std::forward<Function>(f),
        std::forward<Callback>(c),
        ins.size()
    );
    static auto map_wrap = [] (std::shared_ptr<Context> ctx, Input arg, size_t index) {
        ctx->output[index] = ctx->function(arg);
        return ctx;
    };
    static auto cb_wrap = [] (std::shared_ptr<Context> ctx) {
        if ((++ctx->complete_count) == ctx->required_count) {
            ctx->callback(ctx->output);
        }
    };

    for (size_t i = 0; i < ins.size(); i++) {
        async_launch(map_wrap, cb_wrap, std::shared_ptr<Context>(ctx), ins[i], size_t{i});
    }
}

int main() {
    using namespace std::chrono_literals;

    std::vector<int> x{1, 2, 3};
    auto f = [] (auto value) {
        return value + 1;
    };
    auto cb = [] (const std::vector<int>& output) {
        for (size_t i = 0; i < output.size(); i++) {
            std::cout << output[i] << std::endl;
        }
    };
    
    async_vector(x, f, cb);

    std::this_thread::sleep_for(1s);
    return 0;
}
