#include <exe/fibers/sched/yield.hpp>
#include "exe/fibers/core/awaiter.hpp"
#include "exe/fibers/core/fiber.hpp"
#include "suspend.hpp"
namespace exe::fibers {

class YieldAwaiter : public IAwaiter {
 public:
  void AwaitSuspend(fibers::FiberHandle fiber_handle) override {
    fiber_handle.Schedule();
  }
};

void Yield() {
  YieldAwaiter awaiter;
  Suspend(&awaiter);
}

}  // namespace exe::fibers
