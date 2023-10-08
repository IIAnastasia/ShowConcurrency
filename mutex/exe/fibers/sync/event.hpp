#pragma once
#include "exe/coro/simple.hpp"
#include "exe/support/lock_free_stack.hpp"
#include "exe/fibers/core/fiber.hpp"
#include "twist/ed/stdlike/mutex.hpp"
#include "mutex.hpp"
#include <wheels/core/defer.hpp>
#include "exe/threads/spinlock.hpp"
#include <wheels/intrusive/forward_list.hpp>
#include "exe/fibers/core/fiber_intrusive_awaiter.hpp"

namespace exe::fibers {

class Event {
 public:
  void Wait() {
    spinlock_.Lock();
    if (fired_) {
      spinlock_.Unlock();
    } else {
      FiberIntrusiveAwaiter awaiter(&list_, &spinlock_);
      Fiber::Self()->Suspend(&awaiter);
    }
  }

  void Fire() {
    {
      spinlock_.Lock();
      fired_ = true;
      if (list_.IsEmpty()) {
        spinlock_.Unlock();
      } else {
        FiberIntrusiveAwaiter* node = list_.PopFront();
        while (!list_.IsEmpty()) {
          node->Schedule();
          node = list_.PopFront();
        }
        spinlock_.Unlock();
        node->Schedule();
      }
    }
  }

 private:
  wheels::IntrusiveForwardList<FiberIntrusiveAwaiter> list_;
  bool fired_ = false;
  threads::TASSpinLock spinlock_;
};

}  // namespace exe::fibers