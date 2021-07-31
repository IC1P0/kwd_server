#include "accept.hpp"
#include "error_check.hpp"

namespace OB
{
Acceptor::Acceptor(unsigned short port)
    : socket_(),
      addr_(port) {
        SetReuseIP();
        SetReusePort();
        Bind();
        Listen();
      }

Acceptor::Acceptor(const std::string &ip, unsigned short port)
    : socket_(),
      addr_(ip, port) {
        SetReuseIP();
        SetReusePort();
        Bind();
        Listen();
      }

int Acceptor::Accept() {
  int fd = ::accept(get_fd(), NULL, NULL);
  if (!ErrorCheck(-1, fd, "accept: ")) {
    return -1;
  } else {
    return fd;
  }
}

void Acceptor::Bind() {
  int ret = ::bind(socket_.get_fd(), (struct sockaddr*)addr_.get_sock_addr(), sizeof(addr_));
  ErrorCheck(-1, ret, "bind: ");
}

void Acceptor::Listen() {
  int ret = ::listen(socket_.get_fd(), 10);
  ErrorCheck(-1, ret, "listen ");
}

void Acceptor::SetReuseIP(bool on) {
  int optval = 1;
  int ret = ::setsockopt(socket_.get_fd(), SOL_SOCKET,
                         SO_REUSEADDR, &optval, sizeof(optval));
  ErrorCheck(-1, ret, "setsockopt_IP: ");
}

void Acceptor::SetReusePort(bool on) {
  int optval = 1;
  int ret = ::setsockopt(socket_.get_fd(), SOL_SOCKET,
                         SO_REUSEPORT, &optval, sizeof(optval));
  ErrorCheck(-1, ret, "setsockopt_port: ");
}
}//end of namespace OB
