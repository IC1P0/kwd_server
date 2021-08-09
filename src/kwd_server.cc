#include "../include/kwd_server.hpp"
#include <fstream>

namespace OB
{
using std::ifstream;

SearchModule *SearchModule::instance_ptr_ = nullptr;
SearchModule::AutoRelease SearchModule::auto_;

KeyWordServer::KeyWordServer(const string &conf_path)
    : conf_path_(conf_path),
      server_ip_(GetIP()),
      server_port_(GetPort()),
      que_size_(GetQueSize()),
      thread_num_(GetThreadNum()),
      search_module_ptr_(SearchModule::GetInstance()),
      thread_pool_(que_size_, thread_num_),
      acceptor_(server_ip_, server_port_),
      looper_(acceptor_) {
        JsonInit();
      }

void KeyWordServer::StartServer() {
  thread_pool_.ThreadPoolInit();
  looper_.Loop();
}

void KeyWordServer::JsonInit() {
  std::ifstream ifs(conf_path_);

  Json::CharReaderBuilder builder;
  builder["collectComments"] = true;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &root, &errs)) {
    std::cout << errs << std::endl;
    return;
  }
}

string KeyWordServer::GetIP() {
  string ip = root["SERVER_IPV4"].asString();
  if ("" == ip)
    return string("127.0.0.1");
  return ip;
}

int KeyWordServer::GetPort() {
  if (!root["SERVER_PORT"].isNull())
    return root["SERVER_PORT"].asInt();
  return 6688;
}

int KeyWordServer::GetQueSize() {
  if (!root["TASK_QUE_SIZE"].isNull())
    return root["TASK_QUE_SIZE"].asInt();
  return 10;
}

int KeyWordServer::GetThreadNum() {
  if (!root["SERVER_THREADS_NUM"].isNull())
    return root["SERVER_THREADS_NUM"].asInt();
  return 4;
}
}//end of namespace OB
