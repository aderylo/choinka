#ifndef CHOINKA_SEGMENT_TREE
#define CHOINKA_SEGMENT_TREE

#define MULTIPLE 4

#include <set>
#include <vector>

using namespace std;

class SegmentTreeNode {
 public:
  explicit SegmentTreeNode(size_t start, size_t end, multiset<size_t> values) {
    this->start = start;
    this->end = end;
    this->values = values;
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

  vector<SegmentTreeNode> tree;
  vector<size_t> leafs;
};

void SegmentTree::build(vector<size_t> a, size_t v, size_t tl, size_t tr) {
  if (tl == tr) {
    tree.at(v) = SegmentTreeNode(tl, tr, {a.at(tl)});
  } else {
    size_t tm = (tl + tr) / 2;

    build(a, v * 2, tl, tm);
    build(a, v * 2 + 1, tm + 1, tr);

    multiset<size_t> values = tree.at(v * 2).values;
    values.merge(tree.at(v * 2 + 1).values);

    tree.at(v) = SegmentTreeNode(tl, tr, values);
  }
}

SegmentTree::SegmentTree(vector<size_t> elems) {
  tree = vector<SegmentTreeNode>(MULTIPLE * elems.size());
  build(elems, 1, 0, elems.size() - 1);
  leafs = elems;
}

void SegmentTree::updateValue(size_t v, size_t elemIdx, size_t tl, size_t tr, size_t newValue,
                              size_t oldValue) {
  tree.at(v).values.erase(oldValue);
  tree.at(v).values.insert(newValue);

  if (tl != tr) {
    size_t tm = (tl + tr) / 2;
    if (elemIdx <= tm)
      updateValue(v * 2, elemIdx, tl, tr, newValue, oldValue);
    else
      updateValue(v * 2 + 1, elemIdx, tm + 1, tr, newValue, oldValue);
  }
}

void SegmentTree::setValue(size_t elemIdx, size_t value) {
  updateValue(1, elemIdx, 0, leafs.size() - 1, value, leafs.at(elemIdx));
}

// bool SegmentTree::almostHomogenousSegment(size_t start, size_t end) {}

#endif  // CHOINKA_SEGMENT_TREE
