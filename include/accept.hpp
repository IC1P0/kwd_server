#ifndef __OB_ACCEPT_H__
#define __OB_ACCEPT_H__

#include "socket.hpp"
#include "inet_addr.hpp"

namespace OB
{
class Acceptor {
public:
  explicit Acceptor(unsigned short port);
  Acceptor(const std::string &ip, unsigned short port);
  int Accept();
  int get_fd() {
    return socket_.get_fd();
  }

private:
  void SetReuseIP(bool on = true);
  void SetReusePort(bool on = true);
  void Bind();
  void Listen();

  Socket socket_;
  InetAddr addr_;
};
}//end of namespace OB

#endif
