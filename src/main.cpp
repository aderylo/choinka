#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  char orderType;
  size_t n, q, v, x;
  vector<size_t> christmastTree, colours;

  cin >> n >> q;

  for (size_t i = 0; i < n - 1; i++) {
    christmastTree.push_back(0);
    cin >> christmastTree.at(i);
  }

  for (size_t i = 0; i < n; i++) {
    colours.push_back(0);
    cin >> colours.at(i);
  }

  // preproccessing


  // handling questions

  for (size_t i = 0; i < q; i++) {
    cin >> orderType;

    if (orderType == '?') {
      cin >> v;
      // hanlde question;
    }

    if (orderType == 'z') {
      cin >> v >> x;
      // hanlde color change
    }
  }


  return 0;
}