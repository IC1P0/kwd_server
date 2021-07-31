#include "../include/mutex_lock.hpp"
#include <stdio.h>

namespace OB
{
MutexLock::MutexLock() {
  if (pthread_mutex_init(&mutex_lock_, nullptr)) {
    perror("Mutex_init: ");
  }
}

MutexLock::~MutexLock() {
  if (pthread_mutex_destroy(&mutex_lock_)) {
    perror("Mutex_destroy: ");
  }
}

void MutexLock::MutexLockOn() {
  if (pthread_mutex_lock(&mutex_lock_)) {
    perror("Mutex_lock_on: ");
  }
}

void MutexLock::MutexLockOff() {
  if (pthread_mutex_unlock(&mutex_lock_)) {
    perror("mutex_lock_off: ");
  }
}
}//end of namespace OB
