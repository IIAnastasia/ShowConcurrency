#pragma once

#include <exe/fibers/core/routine.hpp>
#include <exe/fibers/core/scheduler.hpp>

#include <exe/coro/core.hpp>
#include <twist/ed/local/ptr.hpp>
#include <wheels/intrusive/list.hpp>
#include "awaiter.hpp"

namespace exe::fibers {

// Fiber = stackful coroutine + scheduler (executor)

class Fiber : public executors::TaskBase {
 public:
  void Suspend(fibers::IAwaiter*);
  void Schedule();
  void Switch();

  // Task
  void Run() noexcept override;

  static Fiber* Self();

  explicit Fiber(Routine routine, Scheduler* scheduler_ptr)
      : coroutine_(std::move(routine)),
        scheduler_ptr_(scheduler_ptr) {
  }

  static Scheduler* GetScheduler();

 private:
  coro::Coroutine coroutine_;
  Scheduler* scheduler_ptr_;
  //  Fiber* next_ = nullptr;
  //  Fiber* prev_ = nullptr;
  twist::ed::stdlike::atomic<bool> running_ = {false};
  IAwaiter* awaiter_ptr_ = nullptr;
};
}  // namespace exe::fibers
