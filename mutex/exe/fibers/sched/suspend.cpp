#include <exe/fibers/sched/suspend.hpp>
#include <exe/fibers/core/fiber.hpp>

namespace exe::fibers {

void Suspend(fibers::IAwaiter* awaiter_ptr) {
  Fiber::Self()->Suspend(awaiter_ptr);
}

}  // namespace exe::fibers
