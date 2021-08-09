#include "../include/timer.hpp"
#include "../include/error_check.hpp"
#include <unistd.h>
#include <sys/poll.h>

namespace OB
{
Timer::Timer(time_t init_time,
             time_t inter_time,
             const CallBackFunc &func,
             int poll_wait_time)
    : timer_fd_(CreateTimer()),
      init_time_(init_time),
      inter_time_(inter_time),
      poll_wait_time_(poll_wait_time),
      is_stop_(false),
      func_(func) {}

Timer::~Timer() {
  if (!is_stop_) {
    Stop();
  }
}

void Timer::Start() {
  struct pollfd time_poll;
  time_poll.fd = timer_fd_;
  time_poll.events = POLLIN;
  SetTimer(init_time_, inter_time_);
  while (!is_stop_) {
    int ready_num = poll(&time_poll, 1, poll_wait_time_);
    if (-1 == ready_num && errno == EINTR) {
      continue;
    } else if (-1 == ready_num) {
      perror("poll: ");
      return;
    } else if (0 == ready_num) {
    } else {
      if (time_poll.revents & POLLIN) {
        ReadFD();
        if (func_) func_();
      }
    }
  }
}

void Timer::Stop() {
  if (!is_stop_) {
    is_stop_ = true;
    SetTimer(0, 0);
  }
}

int Timer::ReadFD() {
  uint64_t num;
  int ret = ::read(timer_fd_, &num ,sizeof(num));
  if (ret != sizeof(num)) {
    perror("time_read: ");
    return -1;
  } else {
    if (num == 10) Stop();
  }
  return num;
}

int Timer::CreateTimer() {
  int ret = ::timerfd_create(CLOCK_REALTIME, 0);
  ErrorCheck(-1, ret, "timerfd_create: ");
  return ret;
}

void Timer::SetTimer(time_t init_time, time_t inter_time) {
  struct itimerspec new_value;
  new_value.it_value.tv_sec = init_time;
  new_value.it_value.tv_nsec = 0;
  new_value.it_interval.tv_sec = inter_time;
  new_value.it_interval.tv_nsec = 0;
  int ret = ::timerfd_settime(timer_fd_, 0, &new_value, NULL);
  ErrorCheck(-1, ret, "Settimer: ");
}
}//end of namespace OB
