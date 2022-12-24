#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "src/chirstmasTree.hh"

using namespace std;


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