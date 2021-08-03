#ifndef __OB_CREATE_DICT_H__
#define __OB_CREATE_DICT_H__

#include "cppjieba/Jieba.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

namespace OB
{
using std::vector;
using std::string;
using std::unordered_set;
using std::unordered_map;

const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

class DictCreator {
public:
  explicit DictCreator(const string &text_path,
                       const string &out_put_path,
                       const string &stop_word_path);
  void CreateDict();
private:
  void Init();
  bool IsStopWord(const string &word);
  bool IsTextEnd();
  bool IsEnglish(const string &word);
  void HandleText();
  void HandleWord(vector<string> &words);
  void WriteFile();

  DIR *dir_ptr_;
  struct dirent *dirent_ptr_;
  string text_path_;
  string out_put_path_;
  string stop_word_path_;
  cppjieba::Jieba jieba;
  unordered_map<string, int> dict_;
  unordered_set<string> stop_word_list_;
};
}//end of namespace OB

#endif
