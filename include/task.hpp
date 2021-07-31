#ifndef __OB_TASK_H__
#define __OB_TASK_H__

#include <string>
#include <iostream>

namespace OB
{
using std::cout;
using std::endl;
class Task {
public:
  Task(const std::string &msg) : msg_(msg) {}
  void Process() {
    cout << "Coming Client Message: " << msg_ << endl;
    delete this;
  }

private:
  std::string msg_;
};
}//enf of namespace OB

#endif
