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
  // assert(vec.size() <= 8);
  bool good = true;
  size_t repeatedValues = 0;
  unordered_map<size_t, size_t> valueCount;
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
  // assert(vec.size() <= 8);
  vector<size_t> result;
  auto valueCount = getValueCount(vec);

  if (valueCount.size() <= 2) {
    for (auto const &[k, v] : valueCount) {
      result.push_back(k);
      if (valueCount[k] > 1)
        result.push_back(k);
    }
  } else {
    int i = 0;
    for (auto const &[k, v] : valueCount) {
      if (i < 3) {
        result.push_back(k);
        result.push_back(k);
        i++;
      } else {
        break;
      }
    }
  }

  return result;
}

vector<size_t> representativeElemsFaster(const vector<size_t> &a, const vector<size_t> &b) {
  vector<size_t> result;
  if (a.size() == 1 and b.size() == 1) {
    result = {a.front(), b.front()};
  } else if (a.size() > 3)
    result = a;
  else if (b.size() > 3)
    result = b;
  else {
    auto vec = join(a, b);
    result = representativeElemsFast(vec);
  }

  return result;
}


#endif  // CHOINKA_UTILS
