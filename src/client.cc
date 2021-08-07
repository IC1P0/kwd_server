#include "../include/socket.hpp"
#include "../include/json/json.h"
#include "../include/inet_addr.hpp"
#include <funcc.h>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

using namespace OB;

int main() {
  std::unique_ptr<Socket> socket(new Socket());
  std::unique_ptr<InetAddr> addr(new InetAddr(6688));

  ::connect(socket->get_fd(), (struct sockaddr*)addr->get_sock_addr(),
            sizeof(*addr->get_sock_addr()));
  string str;
  getline(std::cin, str);
  write(socket->get_fd(), str.c_str(), strlen(str.c_str()));
  char buf[1024] = {0};
  cout << "about to read" << endl;
  ::read(socket->get_fd(), buf, sizeof(buf));
  cout << "READ DONE" << endl;
  str = buf;
  Json::Value root;
  JSONCPP_STRING err;
  Json::CharReaderBuilder builder;
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
  if (!reader->parse(str.c_str(), str.c_str()+str.length(), &root, &err)) {
    cout << "error" << endl;
  }
  string word_to_search = root["SearchWord"].asString();
  const Json::Value word_ret_list = root["SearchRet"];
  cout << "Searching word: " << word_to_search << endl
       << "Search result: " << endl;
  for (int i = 0; i < word_ret_list.size(); ++i) {
    string word_search_ret = word_ret_list[i].asString();
    cout << word_search_ret << endl;
  }

  return 0;
}

