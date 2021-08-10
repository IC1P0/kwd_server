#include "../include/search_module.hpp"
#include "../include/dict_creator.hpp"
#include "../include/reference_creator.hpp"
#include "../include/json/json.h"
#include <queue>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace OB
{
using std::min;
using std::ifstream;
using std::istringstream;
using std::priority_queue;

SearchModule::SearchModule(unsigned int search_word_list_size,
                           const string &text_to_handle_path,
                           const string &dict_path,
                           const string &stop_words_list_path,
                           const string &ref_path)
    : search_word_list_size_(search_word_list_size),
      text_to_handle_path_(text_to_handle_path),
      dict_path_(dict_path),
      stop_words_list_path_(stop_words_list_path),
      ref_path_(ref_path),
      dict_creator_(nullptr),
      ref_creator_(nullptr) {
        Init();
      }

SearchModule::~SearchModule() {
  delete dict_creator_;
  delete ref_creator_;
  dict_creator_ = nullptr;
  ref_creator_ = nullptr;
}

bool operator>(const SearchModule::SearchResult &lhs, const SearchModule::SearchResult &rhs) {
  if (lhs.min_edit_dis == rhs.min_edit_dis) {
    if (lhs.occur_times == rhs.occur_times) {
      return lhs.word > rhs.word;
    } else {
      return lhs.occur_times < rhs.occur_times;
    }
  } else {
    return lhs.min_edit_dis > rhs.min_edit_dis;
  }
}

SearchModule *SearchModule::GetInstance() {
  if (!instance_ptr_) {
    instance_ptr_ = new SearchModule();
  }
  return instance_ptr_;
}

void SearchModule::Destroy() {
  if (instance_ptr_) {
    delete instance_ptr_;
    instance_ptr_ = nullptr;
  }
}

SearchRetPair SearchModule::SearchWord(const string &word) {
  if (IsEnglish(word)) {
    string word_lower(word);
    for (auto &i : word_lower) {
      if (::isupper(i)) i += 32;
    }
    return HandleENG(word_lower);
  } else {
    return HandleCH(word);
  }
}

void SearchModule::Init() {
  Json::Value root;
  string CONF_PATH("../config.json");
  std::ifstream ifs(CONF_PATH);

  Json::CharReaderBuilder builder;
  builder["collectComments"] = true;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &root, &errs)) {
    std::cout << errs << std::endl;
    return;
  }
  if (!root["SEARCH_RET_LIST_SIZE"].isNull())
    search_word_list_size_ = root["SEARCH_RET_LIST_SIZE"].asInt();
  Json::Value file_path = root["FILE_PATH"];
  if ("" != file_path["TEXT_TO_HANDLE"].asString())
    text_to_handle_path_ = file_path["TEXT_TO_HANDLE"].asString();
  if ("" != file_path["STOP_WORDS_LIST"].asString())
    stop_words_list_path_ = file_path["STOP_WORDS_LIST"].asString();
  if ("" != file_path["HANDLED_DICT"].asString())
    dict_path_ = file_path["HANDLED_DICT"].asString();
  if ("" != file_path["HANDLED_REF"].asString())
    ref_path_ = file_path["HANDLED_REF"].asString();
  ifs.close();

  dict_creator_ = new DictCreator(text_to_handle_path_,
                                  dict_path_,
                                  stop_words_list_path_);
  ref_creator_ = new RefFileCreator(dict_path_, ref_path_);
  dict_creator_->CreateDict();
  ref_creator_->Init();
}

SearchRetPair SearchModule::HandleENG(const string &word) {
  const vector<pair<string, int>> &dict_vec = ref_creator_->get_dict();
  const unordered_map<string, set<int>> &ref_umap = ref_creator_->get_ref();
  priority_queue<SearchResult,
    vector<SearchResult>,
    std::greater<SearchResult>> ret_que;
  set<int> word_dict_pos;
  for (auto i : word) {
    char i_tmp[2] = {i, '\0'};
    auto it = ref_umap.find(string(i_tmp));
    if (it == ref_umap.end()) continue;
    for (auto i : it->second) {
      word_dict_pos.insert(i);
    }
  }
  for (auto i : word_dict_pos) {
    string dict_word = dict_vec[i].first;
    int dict_word_cnt = dict_vec[i].second;
    SearchResult ret_tmp;
    ret_tmp.min_edit_dis = MinEdit(word, dict_word);
    if (ret_tmp.min_edit_dis > 3) continue;
    ret_tmp.occur_times = dict_word_cnt;
    ret_tmp.word = dict_word;
    ret_que.push(ret_tmp);
  }
  size_t size = min(search_word_list_size_, ret_que.size());
  vector<string> search_word_list(size);
  for (size_t pos = 0; pos < size; ++pos) {
    search_word_list[pos] = ret_que.top().word;
    ret_que.pop();
  }
  return pair<string, vector<string>>(word, search_word_list);
}

SearchRetPair SearchModule::HandleCH(const string &word) {
  vector<pair<string, int>> &dict_vec(ref_creator_->get_dict());
  unordered_map<string, set<int>> &ref_umap(ref_creator_->get_ref());
  priority_queue<SearchResult,
    vector<SearchResult>,
    std::greater<SearchResult>> ret_que;
  size_t pos = 0;
  set<int> word_dict_pos;
  while (pos < word.size()-1) {
    char char_tmp[3] = {word[pos], word[++pos], word[++pos]};
    ++pos;
    auto it = ref_umap.find(string(char_tmp));
    if (it == ref_umap.end()) continue;
    for (const int &i : it->second) {
      word_dict_pos.insert(i);
    }
  }
  for (auto i : word_dict_pos) {
    string dict_word = dict_vec[i].first;
    int dict_word_cnt = dict_vec[i].second;
    SearchResult ret_tmp;
    ret_tmp.min_edit_dis = MinEdit(word, dict_word);
    if (ret_tmp.min_edit_dis/3 > 3) continue;
    ret_tmp.occur_times = dict_word_cnt;
    ret_tmp.word = dict_word;
    ret_que.push(ret_tmp);
  }
  size_t size = min(search_word_list_size_, ret_que.size());
  vector<string> search_word_list(size);
  for (size_t pos = 0; pos < size; ++pos) {
    search_word_list[pos] = ret_que.top().word;
    ret_que.pop();
  }
  return pair<string, vector<string>>(word, search_word_list);
}

int SearchModule::MinEdit(const string &word1, const string &word2) {
  int max1 = word1.size();
  int max2 = word2.size();

  int **ptr = new int*[max1 + 1];
  for (int i = 0; i < max1 + 1 ;i++) {
    ptr[i] = new int[max2 + 1];
  }

  for (int i = 0 ;i < max1 + 1 ;i++) {
    ptr[i][0] = i;
  }

  for (int i = 0 ;i < max2 + 1;i++) {
    ptr[0][i] = i;
  }

  for (int i = 1 ;i < max1 + 1 ;i++) {
    for (int j = 1 ;j< max2 + 1; j++) {
      ptr[i][j] = Min(ptr[i-1][j] + 1,
                     ptr[i][j-1] + 1,
                     ptr[i-1][j-1] + (word1[i-1] == word2[j-1] ? 0 : 1));
    }
  }
  int dis = ptr[max1][max2];
  for (int i = 0; i < max1 + 1; i++) {
    delete[] ptr[i];
    ptr[i] = NULL;
  }
  delete[] ptr;
  ptr = NULL;
  return dis;
}
}//end of namespace OB
