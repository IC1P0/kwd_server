#include "../include/task.hpp"

namespace OB
{
Task::Task(const std::string &msg, SearchModule *search_module, TcpServer *conn) 
  : msg_(msg),
  conn_(conn),
  search_module_(search_module) {}

SearchRetPair Task::CallSearchModule() {
  SearchRetPair ret = search_module_->SearchWord(msg_);
  const string search_ret = WriteJsonString(ret);
  conn_->SendToPending(search_ret);
  return ret;
}

void Task::FoundInCache(const SearchRetPair &ret_pair) {
  const string search_ret = WriteJsonString(ret_pair);
  conn_->SendToPending(search_ret);
}

string Task::WriteJsonString(const SearchRetPair &ret_pair) {
  std::ostringstream oss;
  oss << "{" << "\"" << "SearchRet" << "\"" << ":" << "[";
  bool flag = true;
  for (auto i : ret_pair.second) {
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
  root["SearchWord"] = ret_pair.first;
  Json::StreamWriterBuilder builder1;
  const std::string search_ret = Json::writeString(builder1, root);
  return search_ret;
}
}//end of namespace OB
