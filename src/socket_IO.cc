#include "socket_IO.hpp"
#include "error_check.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace OB
{
SocketIO::SocketIO(int fd) : socket_(fd) {}

int SocketIO::Readn(char *buf, size_t len) {
  int ret = 0;
  char *ptr = buf;
  int left = len;
  while (left > 0) {
    do {
      ret = ::read(socket_.get_fd(), ptr, left);
    } while (-1 == ret && errno == EINTR);
    ErrorCheck(-1, ret, "read: ");
    if (0 == ret) {
      std::cout << "Connection Disconnected" << std::endl;
      break;
    } else {
      left -= ret;
      ptr += ret;
    }
  }
  return len-left;
}

int SocketIO::Writen(char *buf, size_t len) {
  int ret = 0;
  do {
    ret = ::write(socket_.get_fd(), buf, len);
  } while (-1 == ret && errno == EINTR);
  return ret;
}

int SocketIO::ReadLine(char *buf, size_t len) {
  int ret = 0;
  char *ptr = buf;
  int left = 0;
  while (left > 0) {
    ret = ReadPeek(buf, left);
    if (0 == ret) {
      std::cout << "Connection Disconnected" << std::endl;
      break;
    } else {
      for (auto i = 0; i < ret; ++i) {
        if (ptr[i] == '\n') {
          Readn(ptr, i+1);
          ptr += ret;
          *ptr = '\0';
          left -= ret;
          return len-left;
        }
      }
      Readn(ptr, ret);
      ptr += ret;
      left -= ret;
    }
  }
  return len - left;
}

int SocketIO::ReadPeek(char *buf, size_t len) {
  int ret = 0;
  do {
    ret = recv(socket_.get_fd(), buf, len, MSG_PEEK);
  } while (-1 == ret && errno == EINTR);
  return ret;
}
}//end of namespace OB
