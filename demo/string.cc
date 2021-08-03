#include <funcc.h>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

int main() {
  string str = "helloword";
  const char *ptr = "YES";
  str.insert(0, ptr);
  cout << "str: " << str << endl;
  return 0;
}

