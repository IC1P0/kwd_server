#ifndef __OB_SOCKET_H__
#define __OB_SOCKET_H__

#include <unistd.h>

namespace OB
{
class Socket {
public:
  Socket();
  ~Socket() {
    ::close(fd_);
  }
  explicit Socket(int fd);
  void ShutDownWrite();
  int get_fd() {
    return fd_;
  }

private:
  int fd_;
  bool is_close_;
};
}//end of namespace OB

#endif
