#include <funcc.h>
#include <bitset>
#include <unordered_set>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;
using std::unordered_set;

int main() {
  unordered_set<string> list_demo;
  string word("hello");
  for (int i = 0; i < 3; ++i) {
    list_demo.insert(word += ('a'+i));
  }
  cout << list_demo.size() << endl;
  for (auto i : list_demo) {
    cout << i  << " ";
  }
  cout << endl;

  std::ifstream ifs("../dict/stop_words_list.utf8");
  std::ifstream ifs1("list");

  string word1;
  while (getline(ifs, word)) {
    cout << "ifs word = " << word 
         << " word size = " << word.size() << endl;
    while (getline(ifs1, word1)) {
      cout << "ifs word1 = " << word1 
           << " word1 size = " << word1.size() << endl;
      if (word == word1) {
        cout << "word match: " << word << endl;
      }
    }
  }

  if (!ifs.good()) {
    perror("ifs.good(): ");
  }
  while (getline(ifs, word)) {
    list_demo.insert(word);
  }
  cout << list_demo.size() << endl;
  for (auto i : list_demo) {
    cout << i  << endl;
  }
  cout << endl;
  cout << list_demo.count("about") << endl;
  return 0;
}

