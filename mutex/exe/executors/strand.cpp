#include <exe/executors/strand.hpp>
namespace exe::executors {

Strand::Strand(IExecutor& underlying)
    : strand_task_(std::make_shared<StrandTask>(underlying)) {
}

void Strand::Submit(TaskBase* task) {
  strand_task_->queued_tasks_.Push(task);
  strand_task_->Submit();
}

void Strand::StrandTask::Submit() {
  while (!queued_tasks_.Empty() && !is_active_.exchange(true)) {
    if (queued_tasks_.Empty()) {
      is_active_.exchange(false);
    } else {
      Node* queued_snapshot = queued_tasks_.AtomicMove();
      while (queued_snapshot != nullptr) {
        Node* next = queued_snapshot->Next();
        scheduled_tasks_.PushFront(queued_snapshot);
        queued_snapshot = next;
      }
      executor_ptr_->Submit(this);
    }
  }
}

void Strand::StrandTask::Run() noexcept {
  std::shared_ptr<StrandTask> this_ptr = shared_from_this();
  while (!scheduled_tasks_.IsEmpty()) {
    scheduled_tasks_.PopFront()->Run();
  }
  is_active_.exchange(false);
  Submit();
}
}  // namespace exe::executors