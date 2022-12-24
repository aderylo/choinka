#ifndef CHOINKA_TREE
#define CHOINKA_TREE

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

 private:
  multiset<size_t> values;
  size_t start, end;
};


class SegmentTree {
 public:
  SegmentTree(vector<size_t> leafs);

  bool almostHomogenous(size_t start, size_t end);

  void setValue(size_t leaf);

 private:
  vector<SegmentTreeNode> tree;
};

SegmentTree::SegmentTree(vector<size_t> leafs) {
  this->tree;
}

#endif  // CHOINKA_TREE
