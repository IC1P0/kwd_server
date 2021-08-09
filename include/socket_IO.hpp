#ifndef __OB_SOCKETIO_H__
#define __OB_SOCKETIO_H__

#include <iostream>
#include "socket.hpp"

namespace OB
{
class SocketIO {
public:
  explicit SocketIO(int fd);
  int Readn(char *buf, size_t len_max);
  int Writen(const char *buf);
  int ReadLine(char *buf, size_t len);

private:
  int ReadPeek(char *buf, size_t leng);
  Socket socket_;
};
}//end of namespace OB

#endif
