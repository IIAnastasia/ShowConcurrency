#include <exe/executors/manual.hpp>
#include <twist/ed/stdlike/mutex.hpp>
namespace exe::executors {

void ManualExecutor::Submit(TaskBase* task) {
  queue_.PushBack(task);
}

// Run tasks

size_t ManualExecutor::RunAtMost(size_t limit) {
  size_t count = std::min(limit, queue_.Size());
  for (size_t i = 0; i < count; ++i) {
    queue_.PopFront()->Run();
  }
  return count;
}

size_t ManualExecutor::Drain() {
  size_t counter = 0;
  while (RunNext()) {
    ++counter;
  }
  return counter;
}

}  // namespace exe::executors
