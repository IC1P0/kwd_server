#include <funcc.h>
#include <bitset>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

void func(string &str) {
  const char *tmp = str.c_str();
  char c = tmp[0];
  cout << std::bitset<8>(c) << endl;
  c = 0xe0;
  cout << std::bitset<8>(c) << endl;

  int num = 1<<7;
  cout << "num = " << num << endl;
  if (!::isalpha(str[0])) {
    cout << "It is Chinese: " << str << endl;
  } else {
    cout << "It is English:" << str << endl;
  }
}

int main() {
  string str = "中国ol";
  string str1 = "china";
  for (auto i : str) {
    if (i >> 7) {
      cout << "this is Chinese" << endl;
    } else {
      cout << "This is English: " << i << endl;
    }
  }

  for (auto i : str1) {
    cout << i << endl;
  }
  return 0;
}

