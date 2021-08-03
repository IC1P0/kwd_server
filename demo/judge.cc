#include <funcc.h>
#include <bitset>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

int main() {
  string str = "你好";
  const char *ptr = str.c_str();
  if (*ptr > 0x70) {
    cout << "This is Chinese: " << str << endl;
  }
  cout << "strlen = " << strlen(ptr) << endl;
  int i = 0x71;
  /* char c = ptr[0]; */
  char c1 = ptr[0];
  char c2 = ptr[1];
  char c3 = ptr[2];
  cout << (std::bitset<8>)c1 << endl;
  cout << (std::bitset<8>)c2 << endl;
  cout << (std::bitset<8>)c3 << endl;
  cout << (std::bitset<8>)str[0] << endl;

  char tmp = 0xe0;
  char tmp1 = 0x71 & 0xf1;
  if ((c1) > tmp) {
  /* if (tmp == tmp1) { */
    char tmp[4] = {0};
    strncpy(tmp, str.c_str(), 3);
    cout << "it is Chinese: " << tmp << endl;
  }
#if 0
  ptr = arr;
  cout << "sizeof(ptr) = " << sizeof(ptr) << endl;
  cout << ptr << endl;
  cout << (std::bitset<8>)ptr[0] << endl;
  cout << (std::bitset<8>)ptr[1] << endl;
  cout << (std::bitset<8>)ptr[2] << endl;
  if (*ptr == 0x383838) {
    cout << "YES" << endl;
  }
#endif
  return 0;
}

