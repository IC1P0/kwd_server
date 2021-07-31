#include "../include/error_check.hpp"
#include "../include/event_loop.hpp"
#include <string.h>

namespace OB
{
EventLoop::EventLoop(Acceptor &acceptor)
    : acceptor_(acceptor),
      efd_(EpollCreate()),
      is_looping_(false),
      event_list_(1024) {
        EpollAdd(acceptor.get_fd());
      }

void EventLoop::Loop() {
  if (!is_looping_) {
    is_looping_ = true;
  }
  while (is_looping_) {
    EpollWait();
  }
}

void EventLoop::UnLoop() {
  if (is_looping_) {
    is_looping_ = false;
  }
}

void EventLoop::HandleConnect() {
  int new_fd = acceptor_.Accept();
  /* TcpConPtr new_conn = std::make_shared<TcpServer>(new_fd); */
  TcpConPtr new_conn(new TcpServer(new_fd));
  EpollAdd(new_fd);
  new_conn->SetConnectCallBack(on_connected_);
  new_conn->SetMessageCallBack(on_message_);
  new_conn->SetCloseCallBack(on_close_);
  conn_list_.insert(std::make_pair(new_fd, new_conn));
  new_conn->HandleConnectCallBack();
}

void EventLoop::HandleClose(int fd) {
  EpollDel(fd);
  auto iter = conn_list_.find(fd);
  iter->second->HandleCloseCallBack();
  conn_list_.erase(iter);
}

int EventLoop::EpollCreate() {
  int efd = epoll_create1(0);
  if (!ErrorCheck(-1, efd, "epoll_create1: ")) return -1;
  return efd;
}

void EventLoop::EpollWait() {
  int ready_num = ::epoll_wait(efd_, &(*event_list_.begin()),
                               event_list_.size(), 5000);
  if (-1 == ready_num && errno == EINTR) {
  } else if (-1 == ready_num) {
    perror("epoll_wait: ");
  } else if (0 == ready_num) {
    printf("Epoll Wait Time Out\n");
  } else {
    if (ready_num == event_list_.size()) {
      event_list_.resize(event_list_.size() * 2);
    }

    for (auto i = 0; i < ready_num; ++i) {
      int ready_fd = event_list_[i].data.fd;
      if (ready_fd == acceptor_.get_fd()) {
        HandleConnect();
      } else {
        auto iter = conn_list_.find(ready_fd);
        if (iter != conn_list_.end()) {
          if (iter->second->IsClose()) {
            HandleClose(ready_fd);
          } else {
            iter->second->HandleMessageCallBack();
          }
        }
      }
    }//end of for
  }
}

void EventLoop::EpollAdd(int fd) {
  int ret = 0;
  struct ::epoll_event event;
  ::memset(&event, 0, sizeof(event));
  event.data.fd = fd;
  event.events = EPOLLIN;
  ret = ::epoll_ctl(efd_, EPOLL_CTL_ADD, fd, &event);
  ErrorCheck(-1, ret, "epoll_ctl_add: ");
}

void EventLoop::EpollDel(int fd) {
  int ret = 0;
  ret = ::epoll_ctl(efd_, EPOLL_CTL_DEL, fd, nullptr);
  ErrorCheck(-1, ret, "epoll_ctl_del: ");
}
}//end of namespace OB
