#include "thread_pool.hpp"
#include "worker.hpp"
#include "unistd.h"

namespace OB
{
ThreadPool::ThreadPool(size_t que_size, size_t thread_num)
    : que_size_(que_size),
      que_(que_size_),
      thread_num_(thread_num) {}

ThreadPool::~ThreadPool() {
  if (!is_exit_) {
    ThreadPoolDestroy();
  }
}

void ThreadPool::ThreadPoolInit() {
  for (size_t i = 0; i < thread_num_; ++i) {
    std::unique_ptr<Thread> tmp(new Thread(std::bind(&Worker::DoWork, Worker(*this)), std::to_string(i)));
    thread_que_.push_back(std::move(tmp));
  }
  for (auto &thread : thread_que_) {
    thread->ThreadStart();
  }
}

void ThreadPool::ThreadPoolDestroy() {
  if (!IsExit()) {
    while (!que_.IsEmpty()) {
      sleep(1);
    }
  }
  is_exit_ = true;
  que_.WakeUp();
  for (auto &thread : thread_que_) {
    thread->ThreadJoin();
  }
}

void ThreadPool::PushTask(Elemtype task) {
  que_.Push(task);
}

Elemtype ThreadPool::GetTask() {
  return que_.Pop();
}

}//end of namespace OB 
