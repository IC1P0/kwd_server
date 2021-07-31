#include "../include/task.hpp"
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

ThreadPool kthread_pool(10, 4);

void Connect(const TcpConPtr &conn) {
  cout << "Coming Client" << endl;
  cout << conn->ConnectionInfo() << endl;
}

void Message(const TcpConPtr &conn) {
  char buf[1024] = {0};
  conn->Receive(buf, sizeof(buf));
  /* std::unique_ptr<ThreadTask> task(new ThreadTask(string(buf))); */
  /* ThreadTask *task(new ThreadTask(string(buf))); */
  /* kthread_pool.PushTask(std::bind(&ThreadTask::Task, task)); */
  Task *task(new Task(string(buf)));
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

