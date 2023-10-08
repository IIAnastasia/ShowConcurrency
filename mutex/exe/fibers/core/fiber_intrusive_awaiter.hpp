#pragma once
#include "exe/fibers/core/awaiter.hpp"
#include <wheels/intrusive/forward_list.hpp>
#include <twist/ed/stdlike/atomic.hpp>
#include "exe/threads/spinlock.hpp"

namespace exe::fibers {

class FiberIntrusiveAwaiter
    : public IAwaiter,
      public wheels::IntrusiveForwardListNode<FiberIntrusiveAwaiter> {
 public:
  explicit FiberIntrusiveAwaiter(
      wheels::IntrusiveForwardList<FiberIntrusiveAwaiter>* list_ptr,
      threads::TASSpinLock* spinlock_ptr)
      : list_ptr_(list_ptr),
        spinlock_ptr_(spinlock_ptr) {
  }

  void AwaitSuspend(exe::fibers::FiberHandle fiber_handle) override {
    fiber_handle_ = fiber_handle;
    list_ptr_->PushBack(this);
    spinlock_ptr_->Unlock();
  }

  void Schedule() {
    fiber_handle_.Schedule();
  }

 private:
  wheels::IntrusiveForwardList<FiberIntrusiveAwaiter>* list_ptr_;
  threads::TASSpinLock* spinlock_ptr_;
  exe::fibers::FiberHandle fiber_handle_;
};
}  // namespace exe::fibers