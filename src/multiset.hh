#ifndef CHOINKA_UTILS
#define CHOINKA_UTILS

#include <assert.h>
#include <algorithm>
#include <set>
#include <tuple>
#include <unordered_map>

using namespace std;

struct dominantValues {
  size_t fstValue;
  size_t fstCount;
  size_t sndValue;
  size_t sndCount;
};

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

dominantValues getDominant(dominantValues a, dominantValues b) {
  vector<pair<size_t, size_t>> vec;
  if (a.fstValue == b.fstValue) {
    a.fstCount += b.fstCount;
    b.fstCount = 0;
  }
  if (a.fstValue == b.sndValue) {
    a.fstCount += b.sndCount;
    b.sndCount = 0;
  }
  if (a.sndValue == b.fstValue) {
    a.sndCount += b.fstCount;
    b.fstCount = 0;
  }

  if (a.sndValue == b.sndValue) {
    a.sndCount += b.sndCount;
    a.sndCount = 0;
  }


  vec.push_back({a.fstCount, a.fstValue});
  vec.push_back({a.sndCount, a.sndValue});
  vec.push_back({b.fstCount, b.fstValue});
  vec.push_back({b.sndCount, b.sndValue});


  sort(vec.begin(), vec.end());
  return {
      .fstValue = vec.at(3).second,
      .fstCount = vec.at(3).first,
      .sndValue = vec.at(2).second,
      .sndCount = vec.at(2).first,
  };
}


bool isAlmostHomogenousFaster(dominantValues dominant, size_t subtreeSize) {
  return (subtreeSize - dominant.fstCount <= 1);
}


#endif  // CHOINKA_UTILS
