#include "../include/condition.hpp"
#include "../include/mutex_lock.hpp"
#include <stdio.h>

namespace OB
{
Condition::Condition(MutexLock &mutex) : mutex_(mutex) {
  if (pthread_cond_init(&cond_, nullptr)) {
    perror("condition_init: ");
  }
}

Condition::~Condition() {
  if (pthread_cond_destroy(&cond_)) {
    perror("condition_destroy: ");
  }
} 

void Condition::Wait() {
  if (pthread_cond_wait(&cond_, mutex_.get_mutex_lock())) {
    perror("condition_wait: ");
  }
}

void Condition::Notify() {
  pthread_cond_signal(&cond_);
}

void Condition::NotifyAll() {
  pthread_cond_broadcast(&cond_);
}
}//end of namespace OB 
