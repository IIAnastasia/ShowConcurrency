#include <exe/fibers/core/fiber.hpp>

#include <twist/ed/local/ptr.hpp>

namespace exe::fibers {
twist::ed::ThreadLocalPtr<Fiber> fiber_ptr;
twist::ed::ThreadLocalPtr<Scheduler> context_ptr;

void Fiber::Suspend(fibers::IAwaiter* ptr) {
  awaiter_ptr_ = ptr;
  coro::Coroutine::Suspend();
}
void Fiber::Switch() {
}
Fiber* Fiber::Self() {
  return fiber_ptr;
}

void Fiber::Schedule() {
  scheduler_ptr_->Submit(this);
}

Scheduler* Fiber::GetScheduler() {
  return fiber_ptr->scheduler_ptr_;
}

void Fiber::Run() noexcept {
  awaiter_ptr_ = nullptr;
  context_ptr = scheduler_ptr_;
  fiber_ptr = this;
  coroutine_.Resume();
  if (!coroutine_.IsCompleted()) {
    if (awaiter_ptr_ != nullptr) {
      awaiter_ptr_->AwaitSuspend(FiberHandle(this));
    }
  } else {
    delete this;
  }
}
}  // namespace exe::fibers
