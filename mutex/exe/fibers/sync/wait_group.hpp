#pragma once
#include <cstdlib>
#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/wait/sys.hpp>
#include <wheels/intrusive/forward_list.hpp>
#include "exe/threads/spinlock.hpp"
#include "exe/fibers/core/fiber.hpp"
#include "event.hpp"
#include "exe/fibers/core/fiber_intrusive_awaiter.hpp"

namespace exe::fibers {

class WaitGroup {
 public:
  void Add(size_t count) {
    counter_.fetch_add(count);
  }

  void Done() {
    if (counter_.fetch_sub(1) == 1) {
      event_.Fire();
    }
  }

  void Wait() {
    event_.Wait();
  }

  ~WaitGroup() {
  }

 private:
  twist::ed::stdlike::atomic<uint32_t> counter_ = 0;
  Event event_;
};

}  // namespace exe::fibers
