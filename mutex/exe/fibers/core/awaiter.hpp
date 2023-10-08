#pragma once

#include <exe/fibers/core/handle.hpp>

namespace exe::fibers {
struct IAwaiter {
 public:
  virtual void AwaitSuspend(fibers::FiberHandle) = 0;
};
}  // namespace exe::fibers
