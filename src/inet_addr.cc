#include "../include/inet_addr.hpp"
#include <string.h>

namespace OB
{
InetAddr::InetAddr (unsigned short port) {
  ::memset(&sock_addr_, 0, sizeof(sock_addr_));
  sock_addr_.sin_family = AF_INET;
  sock_addr_.sin_addr.s_addr = ::inet_addr("127.0.0.1");
  sock_addr_.sin_port = ::htons(port);
}

InetAddr::InetAddr(const std::string &ip, unsigned short port) {
  ::memset(&sock_addr_, 0, sizeof(sock_addr_));
  sock_addr_.sin_family = AF_INET;
  sock_addr_.sin_addr.s_addr = ::inet_addr(ip.c_str());
  sock_addr_.sin_port = htons(port);
}

InetAddr::InetAddr(const struct sockaddr_in &addr) {
  sock_addr_ = addr;
}

unsigned short InetAddr::get_port() const {
  return ::ntohs(sock_addr_.sin_port);
}

std::string InetAddr::get_ip() const {
  return std::string(::inet_ntoa(sock_addr_.sin_addr));
}

struct sockaddr_in *InetAddr::get_sock_addr() {
  return &sock_addr_;
}
}//end of namespace OB
