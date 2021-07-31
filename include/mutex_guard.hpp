#ifndef __OB_MUTEX_GUARD_H__
#define __OB_MUTEX_GUARD_H__

#include "mutex_lock.hpp"

namespace OB
{
class MutexGuard {
public:
  MutexGuard(MutexLock &mutex_lock) : mutex_lock_(mutex_lock) {
    mutex_lock_.MutexLockOn();
  }
  ~MutexGuard() {
    mutex_lock_.MutexLockOff();
  }

private:
  MutexLock &mutex_lock_;
};
}//end of namespace OB 

#endif
