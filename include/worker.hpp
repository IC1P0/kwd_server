#ifndef __OB_WORKER_H__
#define __OB_WORKER_H__

#include "task.hpp"
#include "thread_pool.hpp"
#include "cache.hpp"
#include <pthread.h>
#include <iostream>

namespace OB
{
class Worker {
public:
  Worker(ThreadPool &thread_pool, Cache *cache)
      : cache_(cache),
        thread_pool_(thread_pool) {}
  void DoWork() {
    while (!thread_pool_.IsExit()) {
      auto task = thread_pool_.GetTask();
      std::cout << "THREAD " << thread_name
                << " DOING JOB" << std::endl;
      SearchRetPair ret_pair = cache_->SearchCache(task->get_msg());
      if ("" == ret_pair.first) {
        //can't find in cache;
        printf("=== Cant find in cache ===\n");
        SearchRetPair ret_pair = task->CallSearchModule();
        cache_->InsertCache(ret_pair);
      } else {
        //find in cache;
        printf("=== Found in cache ===\n");
        task->FoundInCache(ret_pair);
      }
      delete task;
    }
  }

private:
  Cache *cache_;
  ThreadPool &thread_pool_;
};
}//end of namespace OB

#endif
