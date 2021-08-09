#include "../include/reference_creator.hpp"
#include <sstream>

namespace OB
{
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;

RefFileCreator::RefFileCreator(const string &dict_path,
                               const string &ref_path)
    : dict_path_(dict_path),
      ref_path_(ref_path),
      out_put_path_(ref_path_) {}

void RefFileCreator::Init() {
  ifstream ifs(ref_path_);
  SetDict();
  if (!ifs.good()) {
    printf("begin to WriteRef\n");
    WriteRef();
    ifs.close();
  } else {
    printf("begin to ReadRef\n");
    ReadRef(ifs);
    ifs.close();
  }
}

void RefFileCreator::SetDict() {
  ifstream ifs(dict_path_);
  if (!ifs.good()) {
    perror("dict_path_ifs.open: ");
    return;
  }
  string line;
  string word;
  int word_cnt;
  while (getline(ifs, line)) {
    istringstream iss(line);
    iss >> word >> word_cnt;
    dict_.push_back(pair<string, int>(word, word_cnt));
  }
  ifs.close();
}

void RefFileCreator::SetRef() {
  for (size_t i = 0; i < dict_.size(); ++i) {
    string word = dict_[i].first;
    if (IsEnglish(word)) {
      for (auto c : word) {
        char tmp[2] = {c, '\0'};
        ref_[string(tmp)].insert(i);
      }
    } else {
      size_t pos = 0;
      while (pos < word.size()-1) {
        char tmp[3] = {word[pos], word[++pos], word[++pos]};
        ++pos;
        ref_[string(tmp)].insert(i);
      }
    }
  }
  printf("SetRef Done, size = %ld\n", ref_.size());
}

bool RefFileCreator::IsEnglish(const string &word) {
  return !(word[0] >> 7);
}

void RefFileCreator::WriteRef() {
  SetRef();
  ofstream ofs(out_put_path_);
  if (!ofs.good()) {
    perror("output_ofs.open:");
  }
  for (auto line : ref_) {
    ofs << line.first;
    for (auto word_pos : line.second) {
      ofs << " " << word_pos;
    }
    ofs << endl;
  }
  ofs.close();
}

void RefFileCreator::ReadRef(ifstream &ifs) {
  string line;
  string word;
  int word_cnt;
  while (getline(ifs, line)) {
    istringstream iss(line);
    iss >> word;
    while (iss >> word_cnt) {
      ref_[word].insert(word_cnt);
    }
  }
  printf("ReadRef Done, size = %ld\n", ref_.size());
  ifs.close();
}

}//end of namespace OB
