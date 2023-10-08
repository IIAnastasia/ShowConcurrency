#pragma once

#include <sure/context.hpp>
#include <sure/stack.hpp>

#include <function2/function2.hpp>
#include <exception>
namespace exe::coro {
class Coroutine : sure::ITrampoline {
 public:
  using Routine = fu2::unique_function<void()>;

  explicit Coroutine(Routine routine);

  void Resume();

  // Suspend running coroutine
  static void Suspend();

  bool IsCompleted() const;

 private:
  void Run() noexcept override;
  sure::ExecutionContext current_coroutine_;
  sure::ExecutionContext old_coroutine_;
  sure::Stack stack_;
  Routine routine_;
  bool completed_ = {false};
  std::exception_ptr exception_ptr_;
};
}  // namespace exe::coro