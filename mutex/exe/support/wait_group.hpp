#pragma once

#include <cstdlib>
#include "twist/ed/stdlike/atomic.hpp"
#include "twist/ed/wait/sys.hpp"
namespace exe::support {
class WaitGroup {
 public:
  void Add(size_t count) {
    counter_.fetch_add(count);
  }

  void Done() {
    auto wake_key = twist::ed::PrepareWake(counter_);
    if (counter_.fetch_sub(1) == 1) {
      twist::ed::WakeAll(wake_key);
    }
  }

  void Wait() {
    int wait_id;
    while ((wait_id = counter_.load()) != 0) {
      twist::ed::Wait(counter_, wait_id);
    }
  }

 private:
  twist::ed::stdlike::atomic<uint32_t> counter_{0};
};
}  // namespace exe::support