#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

set<size_t> multisetToSet(multiset<size_t> ms) {
  set<size_t> result;
  auto it = ms.begin();

  while (it != ms.end()) {
    result.insert(*it);
    it = upper_bound(it, ms.end(), *it);
  }

  return result;
}

bool containsMoreValuesThan(const multiset<size_t> &ms, size_t x) {
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

bool containsLessValuesThan(const multiset<size_t> &ms, size_t x) {
  return (x > 0) && !containsMoreValuesThan(ms, x - 1);
}

bool occursMoreThanOnce(const multiset<size_t> &ms, size_t value) {
  size_t counter = 0;
  multiset<size_t>::iterator it;
  it = (value > 0) ? upper_bound(ms.begin(), ms.end(), value - 1) : ms.begin();

  while (it != ms.end() && counter < 3 && *it == value) {
    counter++;
    it++;
  }

  return (counter > 1);
}

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
    almostHomogenous &= !ms.empty();
  }

  return almostHomogenous;
}

vector<size_t> getSubtreeColors(const vector<vector<size_t>> &children,
                                const vector<size_t> &colors, const size_t &node) {
  vector<size_t> subtreeColors;
  subtreeColors.push_back(colors.at(node));

  for (auto child : children.at(node)) {
    auto childColors = getSubtreeColors(children, colors, child);
    subtreeColors.insert(subtreeColors.end(), childColors.begin(), childColors.end());
  }

  return subtreeColors;
}

bool isAlmostSingleColor(const vector<vector<size_t>> &children, const vector<size_t> &colors,
                         const size_t &node) {
  auto treeColors = getSubtreeColors(children, colors, node);
  multiset<size_t> mset(treeColors.begin(), treeColors.end());
  return isAlmostHomogenous(mset);
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

  vector<vector<size_t>> children(n);  // all nodes - 1

  // numbering from zeros
  for (size_t i = 0; i < n - 1; i++) {
    children.at(christmasTreeDsc.at(i) - 1).push_back(i + 1);
  }


  // handling questions

  for (size_t i = 0; i < q; i++) {
    cin >> orderType;

    if (orderType == '?') {
      cin >> v;

      if (isAlmostSingleColor(children, colours, v - 1))
        cout << "TAK\n";
      else
        cout << "NIE\n";
    }

    if (orderType == 'z') {
      cin >> v >> x;
      colours.at(v - 1) = x;
    }
  }


  return 0;
}