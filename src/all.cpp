#define MULTIPLE 4

#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>


using namespace std;


/** Simple Tree represents a tree with nodes
 * numbered/labeled from 1 to n inclusive.
 */

class SimpleTree {
 public:
  explicit SimpleTree(vector<size_t> treeDescription);

  vector<size_t> getPreOrder();

  vector<size_t> getSubTreeSizes();

 private:
  void preOrderTraversal(vector<size_t>& order, vector<bool>& visited, size_t currentNode);

  size_t recursiveSubtreeSize(vector<size_t>& sizes, size_t currentNode);

  vector<vector<size_t>> children;
};


SimpleTree::SimpleTree(vector<size_t> treeDescription) {
  size_t numberOfNodes, parent;
  numberOfNodes = treeDescription.size() + 1;

  for (size_t i = 0; i <= numberOfNodes; i++) {
    // we will skip the "0" index
    children.push_back({});
  }

  for (size_t i = 0; i < treeDescription.size(); i++) {
    parent = treeDescription.at(i);
    children.at(parent).push_back(i + 2);
  }
}

void SimpleTree::preOrderTraversal(vector<size_t>& order, vector<bool>& visited,
                                   size_t currentNode) {
  if (visited.at(currentNode))
    return;

  order.push_back(currentNode);
  visited.at(currentNode) = true;

  for (const auto& childNode : children[currentNode]) {
    preOrderTraversal(order, visited, childNode);
  }
}

vector<size_t> SimpleTree::getPreOrder() {
  vector<size_t> order;
  vector<bool> visited(children.size(), false);
  preOrderTraversal(order, visited, 1);
  return order;
}

size_t SimpleTree::recursiveSubtreeSize(vector<size_t>& sizes, size_t currentNode) {
  size_t currentNodeSubtreeSize = 1;
  for (const auto& child : children.at(currentNode)) {
    currentNodeSubtreeSize += recursiveSubtreeSize(sizes, child);
  }

  sizes.at(currentNode) = currentNodeSubtreeSize;
  return currentNodeSubtreeSize;
}


vector<size_t> SimpleTree::getSubTreeSizes() {
  vector<size_t> sizes(children.size(), 0);
  recursiveSubtreeSize(sizes, 1);
  return sizes;
}


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

void SegmentTree::updateValue(size_t v, size_t elemIdx, size_t tl, size_t tr, size_t newValue,
                              size_t oldValue) {
  tree.at(v).values.erase(oldValue);
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
  size_t fstValCounter = 0, sndValCounter = 0;
  bool almostHomogenous = true;

  while (fstValPtr != ms.end() && sndValPtr != ms.end()) {
    fstValCounter++;
    sndValCounter++;

    if (fstValCounter > 1 and sndValCounter > 1) {
      almostHomogenous = false;
      break;
    }

    fstValPtr++;
    if (*fstValPtr == *sndValPtr)
      break;
    sndValPtr++;
  }

  return almostHomogenous;
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

  for (auto& node : preOrder) {
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


int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  char orderType;
  size_t n, q, v, x;
  vector<size_t> christmasTreeDsc, colours;

  cin >> n >> q;

  for (size_t i = 0; i < n - 1; i++) {
    christmasTreeDsc.push_back(0);
    cin >> christmasTreeDsc.at(i);
  }

  for (size_t i = 0; i < n; i++) {
    colours.push_back(0);
    cin >> colours.at(i);
  }
  // preprocessing

  ChristmasTree tree(christmasTreeDsc, colours);

  // handling questions

  for (size_t i = 0; i < q; i++) {
    cin >> orderType;

    if (orderType == '?') {
      cin >> v;

      if (tree.isSubtreeAlmostSingleColour(v))
        cout << "TAK\n";
      else
        cout << "NIE\n";
    }

    if (orderType == 'z') {
      cin >> v >> x;
      tree.changeNodeColour(v, x);
    }
  }


  return 0;
}