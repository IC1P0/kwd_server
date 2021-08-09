#ifndef __OB_CACHE_H__
#define __OB_CACHE_H__

#include "../include/mutex_lock.hpp"
#include <list>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

namespace OB
{
using std::list;
using std::pair;
using std::vector;
using std::string;
using std::unordered_map;
using SearchRetPair = pair<string, vector<string>>;

class Cache {
public:
  Cache(uint32_t cache_size, const string &cache_file_path);
  SearchRetPair SearchCache(const string &word);
  void InsertCache(const SearchRetPair &ret_pair);
  void UpdateCache();

private:
  void Init();
  void LRU(const SearchRetPair &ret_pair,
           list<SearchRetPair>::iterator ret_iter);
  void WriteToFirstCache();
  void ReadFromFirstCache();

  bool update_;
  bool is_reading_;
  uint32_t cache_size_;
  string cache_file_path_;
  MutexLock lock_for_update;
  static vector<Cache*> cache_list;
  list<SearchRetPair> cache_LRU_list;
  unordered_map<string, list<SearchRetPair>::iterator> cache_updater_map_;
  unordered_map<string, vector<string>> cache_nonupdate_;
  vector<SearchRetPair> new_search_list_;
};
}//end of namespace OB

#endif
