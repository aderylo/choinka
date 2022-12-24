#ifndef CHOINKA_SIMPLE_TREE
#define CHOINKA_SIMPLE_TREE

#include <stdlib.h>
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


#endif  // CHOINKA_SIMPLE_TREE
