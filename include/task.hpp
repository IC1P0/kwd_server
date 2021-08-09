#ifndef __OB_TASK_H__
#define __OB_TASK_H__

#include "tcp_server.hpp"
#include "SearchModule.hpp"
#include "json/json.h"
#include <string>
#include <iostream>

namespace OB
{
using std::cout;
using std::endl;
class Task {
public:
  Task(const std::string &msg, SearchModule *search_module, TcpServer *conn);
  SearchRetPair CallSearchModule();
  string get_msg() {
    return msg_;
  }
  void FoundInCache(const SearchRetPair &ret);
  string WriteJsonString(const SearchRetPair &ret_pair);
private:
  std::string msg_;
  TcpServer *conn_;
  SearchModule *search_module_;
};
}//enf of namespace OB

#endif
