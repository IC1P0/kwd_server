#include "../include/socket_IO.hpp"
#include "../include/error_check.hpp"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace OB
{
SocketIO::SocketIO(int fd) : socket_(fd) {}

int SocketIO::Readn(char *buf, size_t len_max) {
  int ret = 0;
  char *ptr = buf;
  uint32_t len = 0;
  do {
    ret = recv(socket_.get_fd(), &len, sizeof(len), MSG_WAITALL);
  } while (-1 == ret && errno == EINTR);
  if (0 == ret) {
      std::cout << "Connection Disconnected" << std::endl;
      return 0;
  }
  uint32_t left = len < len_max ? len : len_max;
  ret = 0;
  while (left > 0) {
    do {
      ret = ::recv(socket_.get_fd(), ptr, left, MSG_WAITALL);
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

int SocketIO::Writen(const char *buf) {
  int ret = 0;
  uint32_t len = strlen(buf);
  size_t send_buf_len = strlen(buf) + sizeof(len);
  char *send_buf = new char[send_buf_len]();
  char *ptr = send_buf;
  ::strncpy(send_buf, (char *)&len, sizeof(len));
  ::strncpy(send_buf+sizeof(len), buf, strlen(buf));
  do {
    ret = ::write(socket_.get_fd(), send_buf, send_buf_len);
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
