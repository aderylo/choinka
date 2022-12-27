#define MULTIPLE 4

#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>


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

bool containsMoreValuesThan(const multiset<size_t>& ms, size_t x) {
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

bool containsLessValuesThan(const multiset<size_t>& ms, size_t x) {
  return (x > 0) && !containsMoreValuesThan(ms, x - 1);
}

bool occursMoreThanOnce(const multiset<size_t>& ms, size_t value) {
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
void removeSingleElem(multiset<size_t>& ms, size_t value) {
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


/** SegmentTreeNode
 * It stores segment it represents as well as the
 * multiset of values stored in the leafs of its subtree.
 */

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


/** Christmas tree data structure
 * Designed to answer all questions in less than nlogn.
 */

class ChristmasTree {
 public:
  explicit ChristmasTree(vector<size_t> treeDescription, vector<size_t> colours);

  bool isSubtreeAlmostSingleColour(size_t node);

  void changeNodeColour(size_t node, size_t colour);

 private:
  SegmentTree segmentTree;
  vector<pair<size_t, size_t>> subtreeRanges;
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
  segmentTree.updateValue(preOrderIdx, colour);
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