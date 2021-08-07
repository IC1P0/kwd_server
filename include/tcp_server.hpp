#ifndef __OB_TCPSERVER_H__
#define __OB_TCPSERVER_H__

#include "event_loop.hpp"
#include "socket.hpp"
#include "inet_addr.hpp"
#include "socket_IO.hpp"
#include <memory>
#include <functional>

namespace OB
{
class TcpServer;
using std::string;
using TcpConPtr = std::shared_ptr<TcpServer>;
using TcpFuncCallBack = std::function<void(const TcpConPtr&)>;

class TcpServer 
: public std::enable_shared_from_this<TcpServer> {
public:
  explicit TcpServer(int fd, EventLoop *looper);

  int Receive(char *buf, size_t length);
  int ReceiveLine(char *buf, size_t length);
  int Send(const string &msg);
  /* int Send(char *buf, size_t length); */
  bool IsClose();
  void ShutDownWrite();
  std::string ConnectionInfo();
  void SetConnectCallBack(const TcpFuncCallBack &cb) {
    connect_ = cb;
  }
  void SetMessageCallBack(const TcpFuncCallBack &cb) {
    message_ = cb;
  }
  void SetCloseCallBack(const TcpFuncCallBack &cb) {
    close_ = cb;
  }
  void SendToPending(const string &msg);
  void HandleConnectCallBack();
  void HandleMessageCallBack();
  void HandleCloseCallBack();

private:
  InetAddr get_local_addr();
  InetAddr get_peer_addr();

  Socket socket_;
  SocketIO socket_io_;
  InetAddr loacal_addr_;
  InetAddr peer_addr_;
  EventLoop *looper_;
  bool is_shut_down_;
  TcpFuncCallBack connect_;
  TcpFuncCallBack message_;
  TcpFuncCallBack close_;
};
}//end of namespace OB
#endif
