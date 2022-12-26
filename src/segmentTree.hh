#ifndef CHOINKA_SEGMENT_TREE
#define CHOINKA_SEGMENT_TREE

#define MULTIPLE 4

#include <algorithm>
#include <set>
#include <vector>

using namespace std;

struct SegmentTreeNode {
  SegmentTreeNode(size_t start, size_t end, multiset<size_t> values) {
    this->start = start;
    this->end = end;
    this->values = values;
  }

  SegmentTreeNode() {
    this->start = 0;
    this->end = 0;
    this->values = multiset<size_t>();
  }

  multiset<size_t> values;
  size_t start, end;
};


class SegmentTree {
 public:
  explicit SegmentTree(vector<size_t> elems);
  explicit SegmentTree(){};

  bool almostHomogenousSegment(size_t start, size_t end);

  void setValue(size_t elemIdx, size_t value);

 private:
  void updateValue(size_t v, size_t elemIdx, size_t tl, size_t tr, size_t newValue,
                   size_t oldValue);
  void build(vector<size_t> a, size_t v, size_t tl, size_t tr);

  multiset<size_t> query(size_t v, size_t tl, size_t tr, size_t start, size_t end);

  bool checkIfHomogenousSegment(size_t start, size_t end, size_t v, size_t tl, size_t tr);

  vector<SegmentTreeNode> tree;
  vector<size_t> leafs;
};

void SegmentTree::build(vector<size_t> a, size_t v, size_t tl, size_t tr) {
  if (tl == tr) {
    tree.at(v) = SegmentTreeNode(tl, tr, {a.at(tl)});
  } else if (tl < tr) {
    size_t tm = (tl + tr) / 2;

    build(a, v * 2, tl, tm);
    build(a, v * 2 + 1, tm + 1, tr);

    multiset<size_t> values = tree.at(v * 2).values;
    auto it = tree.at(v * 2 + 1).values.begin();
    while (it != tree.at(v * 2 + 1).values.end()) {
      values.insert(*it);
      it++;
    }

    tree.at(v) = SegmentTreeNode(tl, tr, values);
  }
}

SegmentTree::SegmentTree(vector<size_t> elems) {
  tree = vector<SegmentTreeNode>(MULTIPLE * elems.size());
  build(elems, 1, 0, elems.size() - 1);
  leafs = elems;
}

void removeOneElemOfGivenValue(multiset<size_t> &ms, size_t value) {
  multiset<size_t>::iterator hit(ms.find(value));
  if (hit != ms.end())
    ms.erase(hit);
}

void SegmentTree::updateValue(size_t v, size_t elemIdx, size_t tl, size_t tr, size_t newValue,
                              size_t oldValue) {
  removeOneElemOfGivenValue(tree.at(v).values, oldValue);
  tree.at(v).values.insert(newValue);

  if (tl != tr) {
    size_t tm = (tl + tr) / 2;
    if (elemIdx <= tm)
      updateValue(v * 2, elemIdx, tl, tm, newValue, oldValue);
    else
      updateValue(v * 2 + 1, elemIdx, tm + 1, tr, newValue, oldValue);
  }
}

void SegmentTree::setValue(size_t elemIdx, size_t value) {
  updateValue(1, elemIdx, 0, leafs.size() - 1, value, leafs.at(elemIdx));
  leafs.at(elemIdx) = value;
}


bool containsMoreThanTwoValues(multiset<size_t> ms) {
  if (ms.empty())
    return false;

  auto it = ms.begin();
  size_t counter = 0;

  while (it != ms.end()) {
    counter++;
    it = upper_bound(ms.begin(), ms.end(), *it);
    if (counter > 2)
      break;
  }

  return (counter > 2);
}

bool containsOneValue(multiset<size_t> ms) {
  if (ms.empty())
    return false;

  auto it = ms.begin();
  it = upper_bound(ms.begin(), ms.end(), *it);

  return (it == ms.end());
}

bool isAlmostHomogenous(multiset<size_t> ms) {
  if (ms.empty())
    return true;

  if (containsMoreThanTwoValues(ms))
    return false;

  if (containsOneValue(ms))
    return true;

  // now we know that mset contains exactly 2 values;
  auto fstValPtr = ms.begin();
  auto sndValPtr = upper_bound(ms.begin(), ms.end(), *fstValPtr);
  auto sndVal = *sndValPtr;

  bool onlyOneFirstVal = *(++fstValPtr) == sndVal;
  bool onlyOneSecondVal = (++sndValPtr) == ms.end();

  return onlyOneFirstVal || onlyOneSecondVal;
}


multiset<size_t> SegmentTree::query(size_t v, size_t tl, size_t tr, size_t start, size_t end) {
  // possible speed up, return only 3 elems or sth like false if one
  // segment already is not homogenous;

  if (tl == start && tr == end)
    return tree.at(v).values;

  size_t tm = (tl + tr) / 2;
  multiset<size_t> result;

  if (start <= tm)
    result.merge(query(v * 2, tl, tm, start, tm));

  if (end > tm)
    result.merge(query(v * 2 + 1, tm + 1, tr, tm + 1, end));

  return result;
}


bool SegmentTree::almostHomogenousSegment(size_t start, size_t end) {
  auto segment = query(1, 0, leafs.size() - 1, start, end);
  return isAlmostHomogenous(segment);
}

#endif  // CHOINKA_SEGMENT_TREE
