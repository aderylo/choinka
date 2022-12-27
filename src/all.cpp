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

struct dominantValues {
  size_t fstValue;
  size_t fstCount;
  size_t sndValue;
  size_t sndCount;
};

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

dominantValues getDominant(dominantValues a, dominantValues b) {
  vector<pair<size_t, size_t>> vec;
  if (a.fstValue == b.fstValue) {
    a.fstCount += b.fstCount;
    b.fstCount = 0;
  }
  if (a.fstValue == b.sndValue) {
    a.fstCount += b.sndCount;
    b.sndCount = 0;
  }
  if (a.sndValue == b.fstValue) {
    a.sndCount += b.fstCount;
    b.fstCount = 0;
  }

  if (a.sndValue == b.sndValue) {
    a.sndCount += b.sndCount;
    a.sndCount = 0;
  }


  vec.push_back({a.fstCount, a.fstValue});
  vec.push_back({a.sndCount, a.sndValue});
  vec.push_back({b.fstCount, b.fstValue});
  vec.push_back({b.sndCount, b.sndValue});


  sort(vec.begin(), vec.end());
  return {
      .fstValue = vec.at(3).second,
      .fstCount = vec.at(3).first,
      .sndValue = vec.at(2).second,
      .sndCount = vec.at(2).first,
  };
}


bool isAlmostHomogenousFaster(dominantValues dominant, size_t subtreeSize) {
  return (subtreeSize - dominant.fstCount <= 1);
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
  void build(vector<size_t> a, size_t v, size_t tl, size_t tr);

  void update(size_t v, size_t tl, size_t tr, size_t pos, size_t new_val);

  dominantValues query(size_t v, size_t tl, size_t tr, size_t l, size_t r);

  vector<size_t> leafs;
  vector<dominantValues> tree;
};


void SegmentTree::build(vector<size_t> a, size_t v, size_t tl, size_t tr) {
  if (tl == tr) {
    tree[v] = {a[tl], 1, 0, 0};
  } else {
    int tm = (tl + tr) / 2;
    build(a, v * 2, tl, tm);
    build(a, v * 2 + 1, tm + 1, tr);
    tree[v] = getDominant(tree[v * 2], tree[v * 2 + 1]);
  }
}

void SegmentTree::update(size_t v, size_t tl, size_t tr, size_t pos, size_t new_val) {
  if (tl != tr) {
    int tm = (tl + tr) / 2;
    if (pos <= tm)
      update(v * 2, tl, tm, pos, new_val);
    else
      update(v * 2 + 1, tm + 1, tr, pos, new_val);

    tree[v] = getDominant(tree[v * 2], tree[v * 2 + 1]);
  } else {
    tree[v] = {new_val, 1};
    leafs[pos] = new_val;
  }
}

dominantValues SegmentTree::query(size_t v, size_t tl, size_t tr, size_t l, size_t r) {
  if (l > r)
    return {};
  if (l == tl && r == tr) {
    return tree[v];  // constant since tree[v] contains at most 4 elems;
  }

  size_t tm = (tl + tr) / 2;
  auto f = query(v * 2, tl, tm, l, min(r, tm));
  auto s = query(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
  return getDominant(f, s);
}

SegmentTree::SegmentTree(vector<size_t> elems) {
  tree = vector<dominantValues>(MULTIPLE * elems.size());
  leafs = elems;
  build(elems, 1, 0, elems.size() - 1);
}

bool SegmentTree::almostHomogenousSegment(size_t start, size_t end) {
  auto dominantColor = query(1, 0, leafs.size() - 1, start, end);
  // assert(representatives.size() <= 4);
  return isAlmostHomogenousFaster(dominantColor, end - start + 1);
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