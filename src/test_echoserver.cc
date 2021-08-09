#include "../include/task.hpp"
#include "../include/tcp_server.hpp"
#include "../include/thread_pool.hpp"
#include "../include/event_loop.hpp"
#include <funcc.h>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

using namespace OB;

class ThreadTask {
public:
  ThreadTask(const string &msg) : msg_(msg) {}
  void Task() {
    cout << "Coming Client Message: " << msg_ << endl;
  }

private:
  string msg_;
};

SearchModule *SearchModule::instance_ptr_ = nullptr;
SearchModule::AutoRelease SearchModule::auto_;

ThreadPool kthread_pool(10, 4);
SearchModule *search_module_ptr = SearchModule::GetInstance();

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
  Task *task(new Task(msg, search_module_ptr, conn.get()));
  kthread_pool.PushTask(task);
}

void Closed(const TcpConPtr &conn) {
  cout << "Client Leaving" << endl
       << conn->ConnectionInfo() << endl;
}

void test1() {
  Acceptor acceptor(6688);
  EventLoop looper(acceptor);
  looper.SetConnectCallBack(Connect);
  looper.SetMessageCallBack(Message);
  looper.SetCloseCallBack(Closed);
  kthread_pool.ThreadPoolInit();
  looper.Loop();
}

int main() {
  test1();
  return 0;
}

