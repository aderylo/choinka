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
  explicit SegmentTree(vector<size_t> leafs);
  explicit SegmentTree(){};

  bool almostHomogenousSegment(size_t start, size_t end);

  void setValue(size_t leaf, size_t value);

 private:
  void build(vector<size_t> a, size_t v, size_t tl, size_t tr);

  vector<SegmentTreeNode> tree;
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


SegmentTree::SegmentTree(vector<size_t> leafs) {
  tree = vector<SegmentTreeNode>(MULTIPLE * leafs.size());
  build(leafs, 1, 0, leafs.size() - 1);
}

#endif  // CHOINKA_SEGMENT_TREE
