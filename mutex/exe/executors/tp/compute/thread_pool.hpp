#pragma once

#include <exe/support/queue.hpp>
#include <exe/executors/task.hpp>
#include <vector>
#include <thread>
#include <twist/ed/stdlike/thread.hpp>
#include <twist/ed/stdlike/atomic.hpp>
#include "exe/support/wait_group.hpp"
#include <exe/executors/executor.hpp>
namespace exe::executors::tp::compute {

// Fixed-size pool of worker threads

class ThreadPool : public IExecutor {
 public:
  explicit ThreadPool(size_t threads);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Non-movable
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  // Launches worker threads
  void Start();

  // Schedules task for execution in one of the worker threads
  void Submit(TaskBase* task);

  // Locates current thread pool from worker thread
  static ThreadPool* Current();

  // Waits until outstanding work count reaches zero
  void WaitIdle();

  // Stops the worker threads as soon as possible
  void Stop();

 private:
  void Worker();
  const size_t num_threads_;
  std::vector<twist::ed::stdlike::thread> workers_;
  std::vector<bool> states_;
  support::UnboundedBlockingQueue<TaskBase*> tasks_;
  support::WaitGroup tasks_wg_;
};

}  // namespace exe::executors::tp::compute
