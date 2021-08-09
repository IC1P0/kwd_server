#ifndef __OB_TIMER_H__
#define __OB_TIMER_H__

#include <sys/timerfd.h>
#include <functional>

namespace OB
{
using CallBackFunc = std::function<void()>;

class Timer {
public:
  Timer(time_t init_time,
        time_t inter_time,
        const CallBackFunc &func,
        int poll_wait_time);
  ~Timer();
  void Start();
  void Stop();

private:
  int ReadFD();
  int CreateTimer();
  void SetTimer(time_t init_time, time_t inter_time);

  int timer_fd_;
  time_t init_time_;
  time_t inter_time_;
  int poll_wait_time_;
  bool is_stop_;
  CallBackFunc func_;
};
}//end of namespace OB

#endif
