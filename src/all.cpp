#define MULTIPLE 4

#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>


using namespace std;

/** Choinka utils;
 */

unordered_map<size_t, size_t> getValueCount(vector<size_t> vec) {
  unordered_map<size_t, size_t> valueCount;
  for (const auto &val : vec) {
    valueCount[val]++;
  }

  return valueCount;
}

vector<size_t> join(vector<size_t> a, vector<size_t> b) {
  vector<size_t> result;
  result.reserve(a.size() + b.size());
  result.insert(result.end(), a.begin(), a.end());
  result.insert(result.end(), b.begin(), b.end());
  return result;
}

bool isAlmostHomogenousFast(const vector<size_t> &vec) {
  // assert(vec.size() <= 8);
  bool good = true;
  size_t repeatedValues = 0;
  unordered_map<size_t, size_t> valueCount;
  for (const auto &val : vec) {
    valueCount[val]++;
  }

  for (auto const &[k, v] : valueCount) {
    if (valueCount[k] > 1)
      repeatedValues++;
  }

  good &= (valueCount.size() <= 2);
  good &= (repeatedValues <= 1);

  return good;
}

vector<size_t> representativeElemsFast(const vector<size_t> &vec) {
  // assert(vec.size() <= 8);
  vector<size_t> result;
  auto valueCount = getValueCount(vec);

  if (valueCount.size() <= 2) {
    for (auto const &[k, v] : valueCount) {
      result.push_back(k);
      if (valueCount[k] > 1)
        result.push_back(k);
    }
  } else {
    int i = 0;
    for (auto const &[k, v] : valueCount) {
      if (i < 3) {
        result.push_back(k);
        result.push_back(k);
        i++;
      } else {
        break;
      }
    }
  }

  return result;
}

vector<size_t> representativeElemsFaster(const vector<size_t> &a, const vector<size_t> &b) {
  vector<size_t> result;
  if (a.size() == 1 and b.size() == 1) {
    result = {a.front(), b.front()};
  } else if (a.size() > 3)
    result = a;
  else if (b.size() > 3)
    result = b;
  else {
    auto vec = join(a, b);
    result = representativeElemsFast(vec);
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
  void preOrderTraversal(vector<size_t> &order, vector<bool> &visited, size_t currentNode);

  size_t recursiveSubtreeSize(vector<size_t> &sizes, size_t currentNode);

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

void SimpleTree::preOrderTraversal(vector<size_t> &order, vector<bool> &visited,
                                   size_t currentNode) {
  if (visited.at(currentNode))
    return;

  order.push_back(currentNode);
  visited.at(currentNode) = true;

  for (const auto &childNode : children[currentNode]) {
    preOrderTraversal(order, visited, childNode);
  }
}

vector<size_t> SimpleTree::getPreOrder() {
  vector<size_t> order;
  vector<bool> visited(children.size(), false);
  preOrderTraversal(order, visited, 1);
  return order;
}

size_t SimpleTree::recursiveSubtreeSize(vector<size_t> &sizes, size_t currentNode) {
  size_t currentNodeSubtreeSize = 1;
  for (const auto &child : children.at(currentNode)) {
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