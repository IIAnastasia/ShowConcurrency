#pragma once

#include <mutex>
#include <twist/ed/stdlike/mutex.hpp>
#include <wheels/intrusive/list.hpp>
#include "exe/fibers/core/fiber.hpp"
#include "exe/coro/simple.hpp"
#include "exe/threads/spinlock.hpp"
#include "exe/fibers/core/fiber_intrusive_awaiter.hpp"

namespace exe::fibers {

class Mutex {
 public:
  void Lock() {
    spinlock_.Lock();
    if (locked_) {
      FiberIntrusiveAwaiter mutex_awaiter(&list_, &spinlock_);
      Fiber::Self()->Suspend(&mutex_awaiter);
    } else {
      locked_ = true;
      spinlock_.Unlock();
    }
  }

  void Unlock() {
    std::lock_guard<threads::TASSpinLock> guard(spinlock_);
    if (list_.IsEmpty()) {
      locked_ = false;
    } else {
      FiberIntrusiveAwaiter* node = list_.PopFront();
      node->Schedule();
    }
  }

  // BasicLockable

  void lock() {  // NOLINT
    Lock();
  }

  void unlock() {  // NOLINT
    Unlock();
  }

 private:
  wheels::IntrusiveForwardList<FiberIntrusiveAwaiter> list_;
  bool locked_ = false;
  threads::TASSpinLock spinlock_;
};

}  // namespace exe::fibers
