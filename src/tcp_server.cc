#include "../include/tcp_server.hpp"
#include "../include/error_check.hpp"
#include <sstream>
#include <string.h>

namespace OB
{
TcpServer::TcpServer(int fd, EventLoop *looper)
    : socket_(fd),
      socket_io_(fd),
      loacal_addr_(get_local_addr()),
      peer_addr_(get_peer_addr()),
      looper_(looper),
      is_shut_down_(false) {}

int TcpServer::Receive(char *buf, size_t len) {
  return socket_io_.Readn(buf, len);
}

int TcpServer::ReceiveLine(char *buf, size_t len) {
  return socket_io_.ReadLine(buf, len);
}

int TcpServer::Send(const string &msg) {
  return socket_io_.Writen(msg.c_str(), msg.size());
}

/* int TcpServer::Send(char *buf, size_t len) { */
/*   return socket_io_.Writen(buf, len); */
/* } */

bool TcpServer::IsClose() {
  char buf[1024] = {0};
  int ret = recv(socket_.get_fd(), buf, sizeof(buf), MSG_PEEK|MSG_DONTWAIT);
  return (0 == ret);
}

string TcpServer::ConnectionInfo() {
  std::ostringstream oss;
  oss << loacal_addr_.get_ip() << ":"
      << loacal_addr_.get_port() << " ---->  "
      << peer_addr_.get_ip() << ":"
      <<peer_addr_.get_port();
  return oss.str();
}

void TcpServer::ShutDownWrite() {
  if (!is_shut_down_) {
    is_shut_down_ = true;
  }
  socket_.ShutDownWrite();
}

InetAddr TcpServer::get_peer_addr() {
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  socklen_t len = sizeof(addr);
  int ret = ::getpeername(socket_.get_fd(), (struct sockaddr *)&addr, &len);
  ErrorCheck(-1, ret, "getpeername");
  return InetAddr(addr);
}

InetAddr TcpServer::get_local_addr() {
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  socklen_t len = sizeof(addr);
  int ret = ::getsockname(socket_.get_fd(), (struct sockaddr *)&addr, &len);
  ErrorCheck(-1, ret, "getlocalname");
  return InetAddr(addr);
}

void TcpServer::SendToPending(const string &msg) {
  looper_->AddPendingWork(std::bind(&TcpServer::Send, this, msg));
  looper_->EventWakeUp();
}

void TcpServer::HandleConnectCallBack() {
  connect_(shared_from_this());
}

void TcpServer::HandleMessageCallBack() {
  message_(shared_from_this());
}

void TcpServer::HandleCloseCallBack() {
  close_(shared_from_this());
}
}//end of namespace OB
