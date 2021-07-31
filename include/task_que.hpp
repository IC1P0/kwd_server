#ifndef __OB_TASK_QUE_H__
#define __OB_TASK_QUE_H__

#include "mutex_lock.hpp"
#include "condition.hpp"

#include <queue>
#include <pthread.h>
#include <functional>

namespace OB
{
class Task;
using Elemtype = Task *;
class TaskQue {
public:
  TaskQue(size_t que_size);
  ~TaskQue();

  void Push(Elemtype elem);
  Elemtype Pop();
  void WakeUp();

  bool IsFull() {
    return (que_.size() == que_size_);
  }

  bool IsEmpty() {
    return (0 == que_.size());
  }

private:
  MutexLock mutex_lock_;
  Condition cond_full_;
  Condition cond_empty_;
  size_t que_size_;
  std::queue<Elemtype> que_;
  bool flag;
};
}//end of namespace OB
#endif
