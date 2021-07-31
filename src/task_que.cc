#include "../include/task_que.hpp"
#include "../include/mutex_guard.hpp"

namespace OB
{
TaskQue::TaskQue(size_t que_size)
    : mutex_lock_(),
      cond_full_(mutex_lock_),
      cond_empty_(mutex_lock_),
      que_size_(que_size),
      flag(true) {}

TaskQue::~TaskQue() {

}

void TaskQue::Push(Elemtype elem) {
  MutexGuard auto_unlock(mutex_lock_);
  while (IsFull()) {
    cond_full_.Wait();
  }
  que_.push(elem);
  if (1 == que_.size()) cond_empty_.Notify();
}

Elemtype TaskQue::Pop() {
  MutexGuard auto_unlock(mutex_lock_);
  while (flag && IsEmpty()) {
    cond_empty_.Wait();
  }
  if (flag) {
    auto tmp = que_.front();
    que_.pop();
    if (que_size_ - 1 == que_.size()) cond_full_.Notify();
    return tmp;
  } else {
    return nullptr;
  }
}

void TaskQue::WakeUp() {
  flag = false;
  cond_empty_.NotifyAll();
}
}//end of namespace OB 
