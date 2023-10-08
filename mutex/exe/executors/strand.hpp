#pragma once

#include <exe/executors/executor.hpp>
#include <iostream>
#include "exe/support/lock_free_stack.hpp"
namespace exe::executors {

// Strand / serial executor / asynchronous mutex

class Strand : public IExecutor {
 public:
  explicit Strand(IExecutor& underlying);

  Strand(const Strand&) = delete;
  Strand& operator=(const Strand&) = delete;

  // Non-movable
  Strand(Strand&&) = delete;
  Strand& operator=(Strand&&) = delete;
  void Submit(TaskBase* cs) override;

 private:
  class StrandTask : public TaskBase,
                     public std::enable_shared_from_this<StrandTask> {
   public:
    explicit StrandTask(IExecutor& underlying)
        : executor_ptr_(&underlying) {
    }
    void Submit();
    void Run() noexcept override;
    uint32_t submitted_count_ = 0;
    wheels::IntrusiveForwardList<TaskBase> scheduled_tasks_;
    support::LockFreeIntrusiveStack<TaskBase> queued_tasks_;
    twist::ed::stdlike::atomic<bool> is_active_ = {false};
    IExecutor* executor_ptr_;
    twist::ed::stdlike::atomic<uint32_t> queued_count_ = {0};
  };

  std::shared_ptr<StrandTask> strand_task_;
};

}  // namespace exe::executors
