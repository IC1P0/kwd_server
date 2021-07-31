#ifndef __OB_CONDITION_H__
#define __OB_CONDITION_H__

#include <pthread.h>

namespace OB
{
class MutexLock;
class Condition {
public:
  Condition(MutexLock &mutex);
  ~Condition();

  void Wait();
  void Notify();
  void NotifyAll();

private:
  pthread_cond_t cond_;
  MutexLock &mutex_;
};
}//end of namespace OB 

#endif
