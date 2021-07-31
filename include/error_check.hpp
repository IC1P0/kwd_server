#ifndef __OB_ERRORCHEK_H__
#define __OB_ERRORCHEK_H__

#include <stdio.h>
#include <string>

namespace OB
{
using std::string;
template <class T>
  bool ErrorCheck(T para1, T para2, const string &str) {
    if (para1 == para2) {
      perror(str.c_str());
      return false;
    }
    return true;
  }
}//end of namespace OB

#endif
