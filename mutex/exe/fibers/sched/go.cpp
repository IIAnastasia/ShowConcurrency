#include <exe/fibers/sched/go.hpp>
#include "exe/fibers/core/fiber.hpp"

namespace exe::fibers {

void Go(Scheduler& scheduler, Routine routine) {
  fibers::Fiber* fiber_ptr = new Fiber(std::move(routine), &scheduler);
  fiber_ptr->Schedule();
}

void Go(Routine routine) {
  fibers::Fiber* fiber_ptr =
      new Fiber(std::move(routine), Fiber::GetScheduler());
  fiber_ptr->Schedule();
}
}  // namespace exe::fibers
