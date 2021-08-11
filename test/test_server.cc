#include "../include/task.hpp"
#include "../include/kwd_server.hpp"

using namespace OB;

KeyWordServer kwd_server("../config.json");

void Connect(const TcpConPtr &conn) {
  cout << "Coming Client" << endl;
  cout << conn->ConnectionInfo() << endl;
}

void Message(const TcpConPtr &conn) {
  cout << "COMING MESSAGE" << endl;
  char buf[1024] = {0};
  conn->Receive(buf, sizeof(buf));
  string msg(buf);
  /* std::unique_ptr<ThreadTask> task(new ThreadTask(string(buf))); */
  /* ThreadTask *task(new ThreadTask(string(buf))); */
  /* kthread_pool.PushTask(std::bind(&ThreadTask::Task, task)); */
  Task *task(new Task(msg, kwd_server.get_searchmodule_ptr(), conn.get()));
  kwd_server.get_thread_pool()->PushTask(task);
}

void Closed(const TcpConPtr &conn) {
  cout << "Client Leaving" << endl
       << conn->ConnectionInfo() << endl;
}

int main() {
  kwd_server.SetConnectCallBack(Connect);
  kwd_server.SetMessageCallBack(Message);
  kwd_server.SetCloseCallBack(Closed);
  kwd_server.StartServer();
  return 0;
}

