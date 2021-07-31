#include "../include/socket.hpp"
#include "../include/error_check.hpp"

#include <sys/types.h>
#include <sys/socket.h>

namespace OB
{
Socket::Socket() : is_close_(false) {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  ErrorCheck(fd_, -1, "Socket: ");
}

Socket::Socket(int fd) : fd_(fd), is_close_(false) {}

void Socket::ShutDownWrite() {
  if (is_close_) {
    ::shutdown(fd_, SHUT_WR);
  }
}

}//end of namespace OB
