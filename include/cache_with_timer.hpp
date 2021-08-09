#ifndef __OB_CACHE_WITH_TIMER_H__
#define __OB_CACHE_WITH_TIMER_H__

#include "thread.hpp"
#include "timer.hpp"
#include <vector>
#include <memory>

namespace OB
{
class Cache;
class Timer;

using std::vector;
using std::unique_ptr;
using CachePtr = unique_ptr<Cache>;

class CacheWithTimer {
public:
  CacheWithTimer(int cache_vec_size);
  ~CacheWithTimer();
  void Init();
  vector<CachePtr> &get_cache_ptr_vec_() {
    return cache_ptr_vec_;
  }

private:
  int cache_vec_size_;
  Timer *cache_timer_;
  Thread *timer_thread_;
  vector<CachePtr> cache_ptr_vec_;
};
}//end of namespace OB

#endif
