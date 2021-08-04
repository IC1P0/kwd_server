#ifndef __OB_SEARCH_MODULE_H__
#define __OB_SEARCH_MODULE_H__

#include <set>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

namespace OB
{
using std::set;
using std::pair;
using std::string;
using std::vector;
using std::unordered_map;
using SearchRetPair = pair<string, vector<string>>;

class DictCreator;
class RefFileCreator;

class SearchModule {
public:
  static SearchModule* GetInstance();
  SearchRetPair SearchWord(const string &word);

  struct SearchResult {
    int min_edit_dis;
    int occur_times;
    string word;
  };
  friend bool operator<(const SearchResult &lhs, const SearchResult &rhs);
private:
  SearchModule(unsigned int search_word_list_size = 5,
               const string &text_to_handle_path = "../data/",
               const string &dict_path = "../HandledText/dict",
               const string &stop_words_list_path = "../dict/stop_words_list.utf8",
               const string &ref_path = "../HandledText/ref");
  ~SearchModule();
  static void Destroy();
  void Init();
  void ReadConf();
  bool ConfFound(const string &exp) {
    return (conf_.end() != conf_.find(exp));
  }
  bool IsEnglish(const string &word) {
    return !(word[0] >> 7);
  }
  SearchRetPair HandleENG(const string &word);
  SearchRetPair HandleCH(const string &word);
  int Min(int a, int b, int c) {
    return ((a < b ? a : b) < c ? (a < b ? a : b) : c);
  }
  int MinEdit(const string &word1, const string &word2);

private:
  class AutoRelease {
  public:
    ~AutoRelease() {
      Destroy();
    }
  };
  static SearchModule *instance_ptr_;
  static AutoRelease auto_;

  size_t search_word_list_size_;
  string text_to_handle_path_;
  string dict_path_;
  string stop_words_list_path_;
  string ref_path_;
  unordered_map<string, string> conf_;
  DictCreator *dict_creator_;
  RefFileCreator *ref_creator_;
};
}//end of namespace OB

#endif
