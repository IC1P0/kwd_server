#include "../include/cache.hpp"
#include "../include/mutex_guard.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace OB
{
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;

Cache::Cache(uint32_t cache_size, const string &cache_file_path)
    : update_(false),
      is_reading_(false),
      cache_size_(cache_size),
      cache_file_path_(cache_file_path),
      lock_for_update() {
        Init();
      }

void Cache::Init() {
  cache_list.push_back(this);
  ifstream ifs(cache_file_path_);
  if (!ifs.good()) {
    cout << "Cache File DOES Not Exist" << endl;
  } else {
    string line;
    while (getline(ifs, line)) {
      istringstream iss(line);
      string search_word;
      string tmp_ret_word;
      vector<string> search_ret_vec;
      iss >> search_word;
      while (iss >> tmp_ret_word) {
        search_ret_vec.push_back(tmp_ret_word);
      }
      SearchRetPair ret_pair(search_word, search_ret_vec);
      LRU(ret_pair, cache_LRU_list.end());
    }
  }
  ifs.close();
}

SearchRetPair Cache::SearchCache(const string &word) {
  if (!update_) {
    is_reading_ = true;
    if (cache_updater_map_.end() == cache_updater_map_.find(word)) {
      is_reading_ = false;
      return SearchRetPair("", vector<string>());
    } else {
      auto ret_iter = cache_updater_map_.find(word)->second;
      SearchRetPair ret_pair = *ret_iter;
      LRU(ret_pair, ret_iter);
      is_reading_ = false;
      return ret_pair;
    }
  } else {
    MutexGuard auto_unlock(lock_for_update);
    auto ret = cache_nonupdate_.find(word);
    if (cache_nonupdate_.end() == ret) {
      return SearchRetPair("", vector<string>());
    } else {
      return *ret;
    }
  }
}

void Cache::LRU(const SearchRetPair &ret_pair,
                list<SearchRetPair>::iterator ret_iter) {
  if (ret_iter == cache_LRU_list.end()) {
    cache_LRU_list.emplace_front(ret_pair);
    cache_updater_map_[ret_pair.first] = cache_LRU_list.begin();
    if (cache_LRU_list.size() > cache_size_) {
      string del_word = cache_LRU_list.back().first;
      cache_updater_map_.erase(cache_updater_map_.find(del_word));
      cache_LRU_list.pop_back();
    }
  } else {
    cache_LRU_list.emplace_front(*ret_iter);
    cache_LRU_list.erase(ret_iter);
    cache_updater_map_[ret_pair.first] = cache_LRU_list.begin();
  }
}

void Cache::UpdateCache() {
  cout << endl << "=== Time To Update Cache ===" << endl;
  do {
    update_ = true;
  } while (is_reading_);
  for (size_t i = 1; i < cache_list.size(); ++i) {
    cache_list[i]->WriteToFirstCache();
    cache_list[i]->new_search_list_.clear();
  }
  for (size_t i = 1; i < cache_list.size(); ++i) {
    cache_list[i]->ReadFromFirstCache();
  }
  update_ = false;
  {
    MutexGuard auto_unlock(lock_for_update);
    cache_nonupdate_.clear();
    for (SearchRetPair i : cache_LRU_list) {
      cache_nonupdate_[i.first] = i.second;
    }
  }
  ofstream ofs(cache_file_path_);
  if (!ofs.good()) {
    perror("ofs.open(): ");
  } else {
    for (auto ret_pair : cache_nonupdate_) {
      ofs << ret_pair.first << " ";
      for (auto word : ret_pair.second) {
        ofs << word << " ";
      }
      ofs << endl;
    }
  }
  cout << "=== Update Cache Complete, Cache now has " << cache_LRU_list.size()
       << " records ===" << endl << endl;
  ofs.close();
}

void Cache::WriteToFirstCache() {
  for (SearchRetPair new_pair : new_search_list_) {
    string new_word = new_pair.first;
    auto main_cache = cache_list[0];
    auto main_cache_pair_iter = main_cache->cache_updater_map_.find(new_word);
    if (main_cache_pair_iter == main_cache->cache_updater_map_.end()) {
      main_cache->LRU(new_pair, main_cache->cache_LRU_list.end());
    }
  }
}

void Cache::ReadFromFirstCache() {
  auto main_cache = cache_list[0];
  cache_LRU_list = main_cache->cache_LRU_list;
  cache_updater_map_.clear();
  for (auto it = cache_LRU_list.begin(); it != cache_LRU_list.end(); ++it) {
    cache_updater_map_[it->first] = it;
  }
}

void Cache::InsertCache(const SearchRetPair &ret_pair) {
  LRU(ret_pair, cache_LRU_list.end());
  if (this != cache_list[0]) new_search_list_.push_back(ret_pair);
}
}//end of namespace OB
