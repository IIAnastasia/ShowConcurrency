#pragma once

#include <twist/ed/stdlike/mutex.hpp>
#include <twist/ed/stdlike/condition_variable.hpp>
#include <cstdlib>
namespace exe::support {

class StopableSemaphore {
 public:
  explicit StopableSemaphore(size_t tokens)
      : tokens_(tokens),
        is_stopped_(false) {
  }

  void Acquire() {
    std::unique_lock lock(mutex_);
    while (tokens_ == 0 && !is_stopped_) {
      released_.wait(lock);
    }
    tokens_ -= 1;
    lock.unlock();
  }

  void Release() {
    {
      std::unique_lock lock(mutex_);
      tokens_ += 1;
    }
    released_.notify_one();
  }

  void Stop() {
    {
      std::unique_lock lock(mutex_);
      is_stopped_ = true;
    }
    released_.notify_all();
  }

 private:
  size_t tokens_;
  bool is_stopped_;
  twist::ed::stdlike::mutex mutex_;
  twist::ed::stdlike::condition_variable released_;
};
}  // namespace exe::support
