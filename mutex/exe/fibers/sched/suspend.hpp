#pragma once

#include "exe/fibers/core/awaiter.hpp"
namespace exe::fibers {

void Suspend(fibers::IAwaiter* ptr);

}  // namespace exe::fibers
