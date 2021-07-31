#ifndef __OB_INETADDR_H__
#define __OB_INETADDR_H__

#include <string>
#include <arpa/inet.h>

namespace OB
{
class InetAddr {
public:
  explicit InetAddr(unsigned short port);
  InetAddr(const std::string &ip, unsigned short port);
  InetAddr(const struct sockaddr_in &addr);
  InetAddr(InetAddr &&inet);
  std::string get_ip () const;
  unsigned short get_port () const;
  struct sockaddr_in *get_sock_addr();

private:
  struct sockaddr_in sock_addr_;
};
}//end of namespace OB

#endif
