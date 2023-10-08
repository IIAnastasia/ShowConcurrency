#pragma once

#include <function2/function2.hpp>
#include <wheels/intrusive/list.hpp>
#include <wheels/intrusive/forward_list.hpp>
#include <twist/ed/stdlike/atomic.hpp>
namespace exe::executors {

struct ITask {
  virtual ~ITask() = default;

  virtual void Run() noexcept = 0;
};

struct TaskBase : ITask,
                  public wheels::IntrusiveForwardListNode<TaskBase> {};

class LambdaTask : public TaskBase {
 public:
  template <typename Lambda>
  explicit LambdaTask(Lambda&& function)
      : function_(std::move(function)) {
  }

  void Run() noexcept override {
    function_();
    delete this;
  }

 private:
  fu2::unique_function<void()> function_;
};

using Task = TaskBase;
}  // namespace exe::executors