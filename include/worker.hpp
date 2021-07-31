#ifndef __OB_WORKER_H__
#define __OB_WORKER_H__

#include "task.hpp"
#include "thread_pool.hpp"
#include <pthread.h>
#include <iostream>

namespace OB
{
class Worker {
public:
  Worker(ThreadPool &thread_pool) : thread_pool_(thread_pool) {}
  void DoWork() {
    while (!thread_pool_.IsExit()) {
      auto task = thread_pool_.GetTask();
      std::cout << "THREAD " << thread_name
                << " DOING JOB" << std::endl;
      task->Process();
    }
  }

private:
  ThreadPool &thread_pool_;
};
}//end of namespace OB

#endif
