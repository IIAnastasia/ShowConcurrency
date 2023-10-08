#include <exe/executors/tp/compute/thread_pool.hpp>

#include <twist/ed/local/ptr.hpp>
#include <iostream>

namespace exe::executors::tp::compute {
static twist::ed::ThreadLocalPtr<ThreadPool> pool;

ThreadPool::ThreadPool(size_t threads)
    : num_threads_(threads) {
}

void ThreadPool::Start() {
  for (size_t i = 0; i < num_threads_; ++i) {
    workers_.emplace_back([this] {
      Worker();
    });
  }
}

void ThreadPool::Worker() {
  pool = this;
  while (true) {
    auto task = tasks_.Take();
    if (task.has_value()) {
      task.value()->Run();
      tasks_wg_.Done();
    } else {
      break;
    }
  }
}

ThreadPool::~ThreadPool() {
  assert(workers_.empty());
}

void ThreadPool::Submit(TaskBase* task) {
  tasks_wg_.Add(1);
  tasks_.Put(task);
}

ThreadPool* ThreadPool::Current() {
  return pool;
}

void ThreadPool::WaitIdle() {
  tasks_wg_.Wait();
}

void ThreadPool::Stop() {
  tasks_.Close();
  for (size_t i = 0; i < num_threads_; ++i) {
    workers_[i].join();
  }
  workers_.clear();
  auto task = tasks_.Take();
  while (task.has_value()) {
    task = tasks_.Take();
    tasks_wg_.Done();
  }
}

}  // namespace exe::executors::tp::compute