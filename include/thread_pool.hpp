#ifndef __OB_THREAD_POOL_H__
#define __OB_THREAD_POOL_H__

#include "thread.hpp"
#include "task_que.hpp"
#include "cache.hpp"

#include <vector>
#include <memory>

namespace OB
{
class CacheWithTimer;
using Elemtype = Task *;

extern __thread const char *thread_name;
class ThreadPool {
public:
  ThreadPool(size_t que_size, size_t thread_num);
  ~ThreadPool();
  void ThreadPoolInit();
  void ThreadPoolDestroy();
  void PushTask(Elemtype elem);
  Elemtype GetTask();

  bool IsExit() {
    return is_exit_;
  }

private:
  std::vector<std::unique_ptr<Thread>> thread_que_;
  std::unique_ptr<CacheWithTimer> cache_with_timer_;
  size_t que_size_;
  TaskQue que_;
  size_t thread_num_;
  bool is_exit_;
};
}//end of namespace OB

#endif
