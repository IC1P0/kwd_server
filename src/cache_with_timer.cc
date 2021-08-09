#include "../include/cache_with_timer.hpp"
#include "../include/cache.hpp"
#include "../include/thread.hpp"
#include "../include/timer.hpp"
#include "../include/json/json.h"
#include <fstream>
#include <iostream>

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
  Json::Value root;
  string CONF_PATH("../config.json");
  std::ifstream ifs(CONF_PATH);

  Json::CharReaderBuilder builder;
  builder["collectComments"] = true;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &root, &errs)) {
    std::cout << errs << std::endl;
    return;
  }
  const Json::Value file_path = root["FILE_PATH"];
  string cache_path = file_path["CACHE_FILE"].asString();
  int cache_elements_size = root["CACHE_ELEMENTS_SIZE"].asInt();
  int timer_init_time = root["CACHE_TIMER_INIT_TIME"].asInt();
  int timer_inter_time = root["CACHE_TIMER_INTER_TIME"].asInt();
  int poll_wait_time = root["TIMER_POLL_WAIT_TIME"].asInt();
  ifs.close();

  for (int i = 0; i < cache_vec_size_; ++i) {
    unique_ptr<Cache> tmp(new Cache(cache_elements_size, cache_path));
    cache_ptr_vec_.push_back(std::move(tmp));
  }
  cache_timer_ = new Timer(timer_init_time,
                           timer_inter_time,
                           bind(&Cache::UpdateCache,
                                cache_ptr_vec_[0].get()),
                                poll_wait_time);
  timer_thread_ = new Thread(std::bind(&Timer::Start, cache_timer_));
  timer_thread_->ThreadStart();
}
}//end of namespace OB
