#ifndef __OB_THREAD_H__
#define __OB_THREAD_H__

#include "no_copyable.hpp"
#include <pthread.h>
#include <functional>
#include <string>

namespace OB
{
extern __thread const char *thread_name;
class Thread
: public NoCopyable {
public:
  Thread(const std::function<void()> &process, const std::string &name = std::string());
  ~Thread();
  void ThreadStart();
  void ThreadJoin();;

private:
  static void *ThreadFunc(void *ptr);

  pthread_t thread_id_;
  std::function<void()> process_;
  bool is_running;
  std::string name_;
};
}//end of namespace OB
#endif
