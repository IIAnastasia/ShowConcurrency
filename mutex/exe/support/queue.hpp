#pragma once

#include <twist/ed/stdlike/mutex.hpp>
#include <twist/ed/stdlike/condition_variable.hpp>

#include <optional>
#include "semaphore.hpp"
#include <deque>
#include <iostream>

namespace exe::support {
template <typename T>
class UnboundedBlockingQueue {
 public:
  bool Put(T value) {
    std::lock_guard<twist::ed::stdlike::mutex> lock(mutex_);
    if (!closed_) {
      { buffer_.emplace_back(std::move(value)); }
      not_empty_.Release();
      return true;
    }
    return false;
  }

  std::optional<T> Take() {
    not_empty_.Acquire();
    std::lock_guard lock(mutex_);
    if (buffer_.empty()) {
      return std::nullopt;
    }
    T front = std::move(buffer_.front());
    buffer_.pop_front();
    return front;
  }

  void Close() {
    std::lock_guard lock(mutex_);
    closed_ = true;
    not_empty_.Stop();
  }

 private:
  std::deque<T> buffer_;
  bool closed_ = false;
  StopableSemaphore not_empty_{0};
  twist::ed::stdlike::mutex mutex_;
};

}  // namespace exe::support
