#ifndef THREADPOOL
#define THREADPOOL

#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

class ThreadPool {
public:
    explicit ThreadPool(size_t thread_count);
    ThreadPool() = default;
    ThreadPool(ThreadPool&&) = default;
    ~ThreadPool();

    template <class F>
    void execute(F&& task) {
    {
      std::lock_guard<std::mutex> lk(data_->mtx_);
      data_->tasks_.emplace(std::forward<F>(task));
    }
    data_->cond_.notify_one();
    }

private:
    struct data {
        std::mutex mtx_;
        std::condition_variable cond_;
        bool is_shutdown_ = false;
        std::queue<std::function<void()> > tasks_;
    };
    
    std::shared_ptr<data> data_;
    };

#endif