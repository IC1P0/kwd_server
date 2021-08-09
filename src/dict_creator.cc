#include "../include/dict_creator.hpp"
#include <fstream>

namespace OB
{
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

DictCreator::DictCreator(const string &text_path,
                         const string &out_put_path,
                         const string &stop_word_path)
    : text_path_(text_path),
      out_put_path_(out_put_path),
      stop_word_path_(stop_word_path),
      jieba(DICT_PATH,
            HMM_PATH,
            USER_DICT_PATH,
            IDF_PATH,
            STOP_WORD_PATH) {
        dir_ptr_ = ::opendir(text_path_.c_str());
        dirent_ptr_ = NULL;
      }

void DictCreator::Init() {
  ifstream ifs(stop_word_path_);
  if (!ifs.good()) {
    perror("ifs_open: ");
  }
  string word;
  while (getline(ifs, word)) {
    stop_word_list_.insert(word);
  }
  ifs.close();
}

void DictCreator::CreateDict() {
  ifstream ifs_dict(out_put_path_);
  if (ifs_dict.good()) {
    ifs_dict.close();
    return;
  }
  Init();
  HandleText();
  WriteFile();
}

bool DictCreator::IsStopWord(const string &word) {
  return stop_word_list_.count(word);
}

bool DictCreator::IsTextEnd() {
  return (NULL == (dirent_ptr_ = ::readdir(dir_ptr_)));
}

bool DictCreator::IsEnglish(const string &word) {
  return !(word[0] >> 7);
}

void DictCreator::HandleText() {
  while (!IsTextEnd()) {
    string file_name = dirent_ptr_->d_name;
    if (file_name == "." || file_name == "..") continue;
    file_name.insert(0, text_path_);
    ifstream ifs(file_name);
    if (!ifs.good()) {
      perror("ifs_open: ");
    }
    string line;
    while (getline(ifs, line)) {
      vector<string> words;
      jieba.CutForSearch(line, words);
      HandleWord(words);
    }
    ifs.close();
  }
}

void DictCreator::HandleWord(vector<string> &words) {
  for (auto &word : words) {
    if (IsStopWord(word)) continue;
    if (IsEnglish(word)) {
      for (auto &i : word) {
        if (::isupper(i)) i += 32;
      }
    }
    ++dict_[word];
  }
}

void DictCreator::WriteFile() {
  ofstream ofs(out_put_path_);
  for (auto &i : dict_) {
    ofs << i.first << " " << i.second << endl;
  }
  ofs.close();
}
}//end of namespace OB
