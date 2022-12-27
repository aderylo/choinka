#ifndef CHOINKA_SEGMENT_TREE
#define CHOINKA_SEGMENT_TREE

#define MULTIPLE 4

#include <algorithm>
#include <set>
#include <vector>
#include "multiset.hh"

using namespace std;

/** SegmentTree implementation.
 * In each node all subtree leafs values are stored in a multiset.
 */

class SegmentTree {
 public:
  explicit SegmentTree(vector<size_t> elems);
  explicit SegmentTree(){};

  bool almostHomogenousSegment(size_t start, size_t end);

  void updateValue(size_t pos, size_t value);

 private:
  void build(vector<size_t> a, size_t v, size_t tl, size_t tr);

  void update(size_t v, size_t tl, size_t tr, size_t pos, size_t new_val);

  multiset<size_t> query(size_t v, size_t tl, size_t tr, size_t l, size_t r);

  vector<size_t> leafs;
  vector<multiset<size_t>> tree;
};


void SegmentTree::build(vector<size_t> a, size_t v, size_t tl, size_t tr) {
  if (tl == tr) {
    tree[v] = {a[tl]};
  } else {
    int tm = (tl + tr) / 2;
    vector<size_t> vec;

    build(a, v * 2, tl, tm);
    build(a, v * 2 + 1, tm + 1, tr);
    merge(tree[v * 2].begin(), tree[v * 2].end(), tree[v * 2 + 1].begin(), tree[v * 2 + 1].end(),
          back_inserter(vec));
    tree[v] = multiset<size_t>(vec.begin(), vec.end());
    // this should be linear according to
    // https://cplusplus.com/reference/set/multiset/multiset/
  }
}

void SegmentTree::update(size_t v, size_t tl, size_t tr, size_t pos, size_t new_val) {
  tree[v].erase(tree[v].find(leafs[pos]));
  tree[v].insert(new_val);
  if (tl != tr) {
    int tm = (tl + tr) / 2;
    if (pos <= tm)
      update(v * 2, tl, tm, pos, new_val);
    else
      update(v * 2 + 1, tm + 1, tr, pos, new_val);
  } else {
    leafs[pos] = new_val;
  }
}

multiset<size_t> SegmentTree::query(size_t v, size_t tl, size_t tr, size_t l, size_t r) {
  if (l > r)
    return {};
  if (l == tl && r == tr) {
    return representativeElems(tree[v]);  // log n
  }

  size_t tm = (tl + tr) / 2;
  auto f = query(v * 2, tl, tm, l, min(r, tm));
  auto s = query(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
  f.merge(s);                     // this is constant because f and s contain at most 3 elems each
  return representativeElems(f);  // same here, max 6 elements
}

SegmentTree::SegmentTree(vector<size_t> elems) {
  tree = vector<multiset<size_t>>(MULTIPLE * elems.size());
  leafs = elems;
  build(elems, 1, 0, elems.size() - 1);
}

bool SegmentTree::almostHomogenousSegment(size_t start, size_t end) {
  auto representatives = query(1, 0, leafs.size() - 1, start, end);
  return isAlmostHomogenous(representatives);
}

void SegmentTree::updateValue(size_t pos, size_t value) {
  update(1, 0, leafs.size() - 1, pos, value);
}


#endif  // CHOINKA_SEGMENT_TREE
