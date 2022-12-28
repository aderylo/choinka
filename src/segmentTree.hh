#ifndef CHOINKA_SEGMENT_TREE
#define CHOINKA_SEGMENT_TREE

#define MULTIPLE 4

#include <assert.h>
#include <algorithm>
#include <set>
#include <vector>
#include "utils.hh"

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
  void build(vector<size_t> a);

  void update(size_t pos, size_t new_val);

  vector<size_t> query(size_t v, size_t tl, size_t tr, size_t l, size_t r);

  vector<vector<size_t>> tree;
  size_t base;
};


void SegmentTree::build(vector<size_t> arr) {
  size_t n = base;

  // insert leaf nodes in tree
  for (int i = 0; i < arr.size(); i++)
    tree[n + i] = {arr[i]};

  // build the tree by calculating parents
  for (int i = n - 1; i > 0; --i) {
    tree[i] = representativeElemsFaster(tree[i << 1], tree[i << 1 | 1]);
  }
}

void SegmentTree::update(size_t pos, size_t new_val) {
  // set value at position p
  tree.at(pos + base) = {new_val};
  pos = pos + base;

  // move upward and update parents
  for (int i = pos; i > 1; i >>= 1)
    tree[i >> 1] = representativeElemsFaster(tree[i], tree[i ^ 1]);
}

vector<size_t> SegmentTree::query(size_t v, size_t tl, size_t tr, size_t l, size_t r) {
  if (l > r)
    return {};
  if (l == tl && r == tr) {
    return tree[v];  // constant since tree[v] contains at most 4 elems;
  }

  size_t tm = (tl + tr) / 2;
  auto f = query(v * 2, tl, tm, l, min(r, tm));
  auto s = query(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
  return representativeElemsFaster(f, s);  // same here, max 6 elements
}

size_t calculateBase(vector<size_t> elems) {
  size_t n = elems.size() - 1;
  size_t counter = 0;

  while (n > 0) {
    n /= 2;
    counter++;
  }

  return 1 << counter;
}

SegmentTree::SegmentTree(vector<size_t> elems) {
  base = calculateBase(elems);
  tree = vector<vector<size_t>>(2 * base);
  build(elems);
}

bool SegmentTree::almostHomogenousSegment(size_t start, size_t end) {
  auto representatives = query(1, 0, base - 1, start, end);
  return isAlmostHomogenousFast(representatives);
}

void SegmentTree::updateValue(size_t pos, size_t value) {
  update(pos, value);
}


#endif  // CHOINKA_SEGMENT_TREE
