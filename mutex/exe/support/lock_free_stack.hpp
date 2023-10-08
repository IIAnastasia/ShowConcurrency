#include <twist/ed/stdlike/atomic.hpp>
#include <optional>
#include "wheels/intrusive/forward_list.hpp"

namespace exe::support {
template <typename T>
class LockFreeIntrusiveStack {
 public:
  using Node = wheels::IntrusiveForwardListNode<T>;
  LockFreeIntrusiveStack()
      : top_(nullptr) {
  }
  LockFreeIntrusiveStack(LockFreeIntrusiveStack&& other)
      : top_(other.top_.exchange(nullptr)) {
  }

  void operator=(LockFreeIntrusiveStack&& other) {
    top_.store(other.top_.exchange(nullptr));
  }

  Node* AtomicMove() {
    return top_.exchange(nullptr);
  }

  void Push(Node* new_node) {
    new_node->next_ = top_.load();
    Node* node = new_node->Next();
    while (!top_.compare_exchange_weak(node, new_node)) {
      new_node->SetNext(node);
    }
  }

  bool Empty() {
    return top_.load() == nullptr;
  }

  twist::ed::stdlike::atomic<Node*> top_{nullptr};
};
}  // namespace exe::support