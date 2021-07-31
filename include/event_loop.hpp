#ifndef __OB_EVENTLOOP_H__
#define __OB_EVENTLOOP_H__

#include "accept.hpp"
#include "tcp_server.hpp"
#include <sys/epoll.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

namespace OB
{
using TcpConPtr = std::shared_ptr<TcpServer>;
using TcpFunCallBack = std::function<void(const TcpConPtr&)>;

class EventLoop {
public:
  explicit EventLoop(Acceptor &acceptor);
  void Loop();
  void UnLoop();
  void SetConnectCallBack(TcpFunCallBack &&cb) {
    on_connected_ = std::move(cb);
  }
  void SetMessageCallBack(TcpFunCallBack &&cb) {
    on_message_ = std::move(cb);
  }
  void SetCloseCallBack(TcpFunCallBack &&cb) {
    on_close_ = std::move(cb);
  }

private:
  void HandleConnect();
  void HandleClose(int fd);

  int EpollCreate();
  void EpollAdd(int fd);
  void EpollWait();
  void EpollDel(int fd);

  Acceptor &acceptor_;
  int efd_;
  bool is_looping_;
  std::vector<struct epoll_event> event_list_;
  std::unordered_map<int, std::shared_ptr<TcpServer>> conn_list_;
  TcpFunCallBack on_connected_;
  TcpFunCallBack on_message_;
  TcpFunCallBack on_close_;
};
}//end of namespace OB

#endif
