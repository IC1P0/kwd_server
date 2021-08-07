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
  Task(const std::string &msg, SearchModule *search_module, TcpServer *conn) 
      : msg_(msg),
      conn_(conn),
      search_module_(search_module) {}

  void Process() {
    cout << "Looking for word: " << endl;
    SearchRetPair ret = search_module_->SearchWord(msg_);
    std::ostringstream oss;
    oss << "{" << "\"" << "SearchRet" << "\"" << ":" << "[";
    bool flag = true;
    for (auto i : ret.second) {
      if (flag) {
        oss << "\"" << i << "\"";
        flag = false;
      } else {
        oss << "," << "\"" << i << "\"";
      }
    }
    oss << "]" << "}";

    Json::Value root;
    JSONCPP_STRING err;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(oss.str().c_str(), oss.str().c_str() + oss.str().length(), &root, &err)) {
      std::cout << "error" << std::endl;
    }
    root["SearchWord"] = ret.first;
    Json::StreamWriterBuilder builder1;
    const std::string search_ret = Json::writeString(builder1, root);
    conn_->SendToPending(search_ret);
    delete this;
  }
  string get_msg() {
    return msg_;
  }
  void UpdateMsg(const string &new_msg) {
    conn_->SendToPending(new_msg);
    delete this;
  }

private:
  std::string msg_;
  TcpServer *conn_;
  SearchModule *search_module_;
};
}//enf of namespace OB

#endif
