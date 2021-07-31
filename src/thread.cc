#include "../include/thread.hpp"
#include <stdlib.h>
#include <iostream>

namespace OB
{
__thread const char *thread_name = "thread";
struct ThreadData {
  ThreadData(const std::string &name, const std::function<void()> &func)
      : func_(func),
        name_(name) {}
  void DataFunc() {
    thread_name = (name_ == std::string()) ? "thread" : name_.c_str();
    if (func_) {
      func_();
    }
  }
  std::function<void()> func_;
  std::string name_;
};

Thread::Thread(const std::function<void()> &process, const std::string &name)
    : thread_id_(0),
      process_((process)),
      is_running(false),
      name_(name) {}

Thread::~Thread() {
  if (is_running) {
    pthread_detach(thread_id_);
  }
  std::cout << "Thread OUT" << std::endl;
}

void Thread::ThreadStart() {
  ThreadData *data_ptr = new ThreadData(name_, process_);
  if (pthread_create(&thread_id_, nullptr, ThreadFunc, data_ptr)) {
    perror("Thread_create: ");
  } else {
    is_running = true;
  }
}

void Thread::ThreadJoin() {
  if (pthread_join(thread_id_, nullptr)) {
    perror("Thread_join: ");
  }
}

void *Thread::ThreadFunc(void *this_ptr) {
  auto tmp = static_cast<ThreadData *>(this_ptr);
  if (tmp) tmp->DataFunc();
  delete tmp;
  tmp = nullptr;
  return nullptr;
}
}//end of namespace OB 
