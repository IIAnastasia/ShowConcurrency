#pragma once
#include <twist/ed/wait/spin.hpp>
#include <twist/ed/stdlike/atomic.hpp>
namespace exe::threads {

class TASSpinLock {
 public:
  void Lock() {
    while (locked_.exchange(1) == 1) {
      twist::ed::CpuRelax();
    }
  }

  bool TryLock() {
    return locked_.exchange(1) == 0;
  }

  void Unlock() {
    locked_.store(0);
  }

  void lock() {  // NOLINT
    Lock();
  }

  void unlock() {  // NOLINT
    Unlock();
  }

 private:
  twist::ed::stdlike::atomic<uint32_t> locked_{0};
};
// Test-and-TAS spinlock

}  // namespace exe::threads
