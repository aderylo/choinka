#ifndef CHOINKA_MUTLISET
#define CHOINKA_MUTLISET

#include <algorithm>
#include <set>

using namespace std;

/** Multiset utils;
 */

set<size_t> multisetToSet(multiset<size_t> ms) {
  set<size_t> result;
  auto it = ms.begin();

  while (it != ms.end()) {
    result.insert(*it);
    it = upper_bound(it, ms.end(), *it);
  }

  return result;
}

bool containsMoreValuesThan(const multiset<size_t> &ms, size_t x) {
  size_t counter = 0;
  auto it = ms.begin();

  while (it != ms.end()) {
    counter++;
    it = upper_bound(it, ms.end(), *it);

    if (counter > x)
      break;
  }

  return (counter > x);
}

bool containsLessValuesThan(const multiset<size_t> &ms, size_t x) {
  return (x > 0) && !containsMoreValuesThan(ms, x - 1);
}

bool occursMoreThanOnce(const multiset<size_t> &ms, size_t value) {
  size_t counter = 0;
  multiset<size_t>::iterator it;
  it = (value > 0) ? upper_bound(ms.begin(), ms.end(), value - 1) : ms.begin();

  while (it != ms.end() && counter < 3 && *it == value) {
    counter++;
    it++;
  }

  return (counter > 1);
}

/** Removes single elem from multiset;
 */
void removeSingleElem(multiset<size_t> &ms, size_t value) {
  multiset<size_t>::iterator hit(ms.find(value));
  if (hit != ms.end())
    ms.erase(hit);
}

/** Multiset is almost homogenous if:
 * - it is not empty
 * - no more than one of it values repeats more than once
 * - contains at maximum 2 values
 */
bool isAlmostHomogenous(multiset<size_t> ms) {
  set<size_t> s;
  size_t nonUniqueValues = 0;
  bool almostHomogenous = false;

  if (containsLessValuesThan(ms, 3)) {
    s = multisetToSet(ms);

    for (size_t value : s) {
      if (occursMoreThanOnce(ms, value))
        nonUniqueValues++;
    }

    almostHomogenous = (nonUniqueValues <= 1);
  }

  return almostHomogenous;
}


/** If set is not almost almost homogenous returns empty set;
 *  Otherwise returns representatives of this set thus making a
 * result a multiset with maximum 3 elems.
 *
 * Complexity: log(n)
 */
multiset<size_t> representativeElems(multiset<size_t> ms) {
  multiset<size_t> result;
  set<size_t> values;

  if (isAlmostHomogenous(ms)) {
    values = multisetToSet(ms);
    for (size_t value : values) {
      result.insert(value);
      if (occursMoreThanOnce(ms, value))
        result.insert(value);
    }

  } else {
    result = {1, 2, 3};
  }

  return result;
}


#endif  // CHOINKA_MUTLISET
