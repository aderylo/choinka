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

    auto f = representativeElems(tree[v * 2]);
    auto s = representativeElems(tree[v * 2 + 1]);
    f.merge(s);
    tree[v] = representativeElems(f);
  }
}

void SegmentTree::update(size_t v, size_t tl, size_t tr, size_t pos, size_t new_val) {
  if (tl != tr) {
    int tm = (tl + tr) / 2;
    if (pos <= tm)
      update(v * 2, tl, tm, pos, new_val);
    else
      update(v * 2 + 1, tm + 1, tr, pos, new_val);

    auto f = representativeElems(tree[v * 2]);
    auto s = representativeElems(tree[v * 2 + 1]);
    f.merge(s);
    tree[v] = representativeElems(f);
  } else {
    tree[v] = {new_val};
    leafs[pos] = new_val;
  }
}

multiset<size_t> SegmentTree::query(size_t v, size_t tl, size_t tr, size_t l, size_t r) {
  if (l > r)
    return {};
  if (l == tl && r == tr) {
    return representativeElems(tree[v]);  // constant since tree[v] contains at most 4 elems;
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
