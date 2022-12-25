#ifndef CHOINKA_TREE
#define CHOINKA_TREE

#include <map>
#include <vector>
#include "segmentTree.hh"
#include "simpleTree.hh"
using namespace std;

class ChristmasTree {
 public:
  explicit ChristmasTree(vector<size_t> treeDescription, vector<size_t> colours);

  bool isSubtreeAlmostSingleColour(size_t node);

  void changeNodeColour(size_t node, size_t colour);

 private:
  SegmentTree segmentTree;
  vector<pair<size_t, size_t>> subtreeRanges;
  // vector<size_t> preOrderMap; // key: node, value: idx in preOrder
};

ChristmasTree::ChristmasTree(vector<size_t> treeDescription, vector<size_t> colours) {
  SimpleTree tree(treeDescription);
  auto preOrder = tree.getPreOrder();
  auto subtreeSizes = tree.getSubTreeSizes();
  vector<size_t> leafs;
  subtreeRanges = vector<pair<size_t, size_t>>(preOrder.size() + 1);
  size_t idx = 1;

  for (auto &node : preOrder) {
    leafs.push_back(colours.at(node - 1));
    subtreeRanges.at(node) = {idx, idx + subtreeSizes.at(node) - 1};
    idx++;
  }

  segmentTree = SegmentTree(leafs);
}

bool ChristmasTree::isSubtreeAlmostSingleColour(size_t node) {
  auto range = subtreeRanges.at(node);
  return segmentTree.almostHomogenousSegment(range.first - 1, range.second - 1);
}

void ChristmasTree::changeNodeColour(size_t node, size_t colour) {
  auto preOrderIdx = subtreeRanges.at(node).first - 1;
  segmentTree.setValue(preOrderIdx, colour);
}


#endif  // CHOINKA_TREE
