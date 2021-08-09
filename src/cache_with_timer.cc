#include "../include/cache_with_timer.hpp"
#include "../include/cache.hpp"
#include "../include/thread.hpp"
#include "../include/timer.hpp"

namespace OB
{
using std::bind;

vector<Cache*> Cache::cache_list;

CacheWithTimer::CacheWithTimer(int cache_vec_size)
  : cache_vec_size_(cache_vec_size),
    cache_timer_(nullptr),
    timer_thread_(nullptr) {
    Init();
  }

CacheWithTimer::~CacheWithTimer() {
  cache_timer_->Stop();
  timer_thread_->ThreadJoin();
  delete cache_timer_;
  delete timer_thread_;
  cache_timer_ = nullptr;
  timer_thread_ = nullptr;
}

void CacheWithTimer::Init() {
  for (int i = 0; i < cache_vec_size_; ++i) {
    unique_ptr<Cache> tmp(new Cache(10, "../HandledText/cache"));
    cache_ptr_vec_.push_back(std::move(tmp));
  }
  cache_timer_ = new Timer(20, 10, bind(&Cache::UpdateCache,
                                        cache_ptr_vec_[0].get()));
  timer_thread_ = new Thread(std::bind(&Timer::Start, cache_timer_));
  timer_thread_->ThreadStart();
}
}//end of namespace OB
