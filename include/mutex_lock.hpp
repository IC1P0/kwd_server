#ifndef __OB_MUTEX_LOCK_H__
#define __OB_MUTEX_LOCK_H__

#include "no_copyable.hpp"
#include <pthread.h>

namespace OB
{
class MutexLock
: public NoCopyable {
public:
  MutexLock();
  ~MutexLock();

  void MutexLockOn();
  void MutexLockOff();
  pthread_mutex_t *get_mutex_lock() {
    return &mutex_lock_;
  }

private:
  pthread_mutex_t mutex_lock_;
};
}//end of namespace OB

#endif
