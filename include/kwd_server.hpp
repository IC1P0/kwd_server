#ifndef __OB_KEY_WORD_SERVER_H__
#define __OB_KEY_WORD_SERVER_H__

#include "../include/task.hpp"
#include "../include/accept.hpp"
#include "../include/json/json.h"
#include "../include/tcp_server.hpp"
#include "../include/thread_pool.hpp"

namespace OB
{
class KeyWordServer {
public:
  KeyWordServer(const string &conf_path_);
  void StartServer();
  ThreadPool *get_thread_pool() {
    return &thread_pool_;
  }
  SearchModule *get_searchmodule_ptr() {
    return search_module_ptr_;
  }
  void SetConnectCallBack(TcpFunCallBack &&cb) {
    looper_.SetConnectCallBack(std::move(cb));
  }
  void SetMessageCallBack(TcpFunCallBack &&cb) {
    looper_.SetMessageCallBack(std::move(cb));
  }
  void SetCloseCallBack(TcpFunCallBack &&cb) {
    looper_.SetCloseCallBack(std::move(cb));
  }

private:
  void JsonInit();
  string GetIP();
  int  GetPort();
  int GetQueSize();
  int GetThreadNum();

  string conf_path_;
  Json::Value root;
  string server_ip_;
  int server_port_;
  int que_size_;
  int thread_num_;
  SearchModule *search_module_ptr_;
  ThreadPool thread_pool_;
  Acceptor acceptor_;
  EventLoop looper_;
};
}//end of namespace OB

#endif
