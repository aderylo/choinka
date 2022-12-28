#ifndef CHOINKA_UTILS
#define CHOINKA_UTILS

#include <assert.h>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

/** Choinka utils;
 */

unordered_map<size_t, size_t> getValueCount(vector<size_t> vec) {
  unordered_map<size_t, size_t> valueCount;
  for (const auto &val : vec) {
    valueCount[val]++;
  }

  return valueCount;
}

vector<size_t> join(vector<size_t> a, vector<size_t> b) {
  vector<size_t> result;
  result.reserve(a.size() + b.size());
  result.insert(result.end(), a.begin(), a.end());
  result.insert(result.end(), b.begin(), b.end());
  return result;
}

bool isAlmostHomogenousFast(const vector<size_t> &vec) {
  bool good = true;
  size_t repeatedValues = 0;
  unordered_map<size_t, size_t> valueCount;

  if (!vec.empty() && vec.front() == 0)
    good = false;

  for (const auto &val : vec) {
    valueCount[val]++;
  }

  for (auto const &[k, v] : valueCount) {
    if (valueCount[k] > 1)
      repeatedValues++;
  }

  good &= (valueCount.size() <= 2);
  good &= (repeatedValues <= 1);

  return good;
}

vector<size_t> representativeElemsFast(const vector<size_t> &vec) {
  vector<size_t> result;
  auto valueCount = getValueCount(vec);

  if (valueCount.size() <= 2) {
    for (auto const &[k, v] : valueCount) {
      result.push_back(k);
      if (valueCount[k] > 1)
        result.push_back(k);
    }
  } else {
    result.push_back(0);
  }

  return result;
}

vector<size_t> representativeElemsFaster(const vector<size_t> &a, const vector<size_t> &b) {
  vector<size_t> result;
  if (a.empty() || b.empty())
    result = (a.empty()) ? b : a;
  else if (a.front() == 0 || b.front() == 0)
    result = {0};
  else if (a.size() == 1 and b.size() == 1) {
    result = {a.front(), b.front()};
  } else if (a.size() > 3 || b.size() > 3)
    result = {0};
  else {
    auto vec = join(a, b);
    result = representativeElemsFast(vec);
  }

  return result;
}


#endif  // CHOINKA_UTILS
