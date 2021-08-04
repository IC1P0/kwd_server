#ifndef __OB_REFERENCE_CREATOR_H__
#define __OB_REFERENCE_CREATOR_H__

#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>

namespace OB
{
using std::set;
using std::pair;
using std::string;
using std::vector;
using std::ifstream;
using std::unordered_map;

class RefFileCreator {
public:
  explicit RefFileCreator(const string &dict_path, 
                          const string &ref_path_);
  void Init();

private:
  void SetDict();
  void SetRef();
  bool IsEnglish(const string &word);
  void WriteRef();
  void ReadRef(ifstream &ifs);

  string dict_path_;
  string ref_path_;
  string out_put_path_;
  vector<pair<string, int>> dict_;
  unordered_map<string, set<int>> ref_;
};
}//end of namespace OB

#endif
