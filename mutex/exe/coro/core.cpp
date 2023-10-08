#include "core.hpp"

#include <twist/ed/local/ptr.hpp>
#include <iostream>

namespace exe::coro {
static twist::ed::ThreadLocalPtr<Coroutine> current_coroutine_ptr;

Coroutine::Coroutine(Routine routine)
    : stack_(sure::Stack::AllocateBytes(64 * 1024 * 8)),
      routine_(std::move(routine)) {
  current_coroutine_.Setup(stack_.MutView(), this);
}

void Coroutine::Resume() {
  Coroutine* old_coroutine_ptr = current_coroutine_ptr;
  current_coroutine_ptr = this;
  old_coroutine_.SwitchTo(current_coroutine_);
  current_coroutine_ptr = old_coroutine_ptr;

  if (exception_ptr_ != std::exception_ptr(nullptr)) {
    rethrow_exception(exception_ptr_);
  }
}

void Coroutine::Suspend() {
  current_coroutine_ptr->current_coroutine_.SwitchTo(
      current_coroutine_ptr->old_coroutine_);
}

bool Coroutine::IsCompleted() const {
  return completed_;
}

void Coroutine::Run() noexcept {
  try {
    routine_();
  } catch (...) {
    exception_ptr_ = std::current_exception();
  }
  completed_ = true;
  current_coroutine_.ExitTo(old_coroutine_);
}
}  // namespace exe::coro